using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;

namespace EP.Pages;

public class CompareModel : PageModel
{
    private readonly ApplicationDbContext _db;

    public CompareModel(ApplicationDbContext db)
    {
        _db = db;
    }

    [BindProperty(SupportsGet = true)]
    public List<int> SelectedDesignIds { get; set; } = new();

    [BindProperty(SupportsGet = true)]
    public List<int> SelectedPanelIds { get; set; } = new();

    [BindProperty(SupportsGet = true)]
    public string? Platform { get; set; }

    [BindProperty(SupportsGet = true)]
    public string? SKU { get; set; }

    // New: panel feature filters (same names as used elsewhere)
    [BindProperty(SupportsGet = true)]
    public List<string>? FilterPanelFeatures { get; set; }

    public List<DesignEntry> AllDesigns { get; set; } = new();
    public List<DesignEntry> SelectedDesigns { get; set; } = new();
    public Dictionary<int, List<PanelEntry>> PanelsByDesign { get; set; } = new();
    public List<(PanelEntry Panel, DesignEntry Design, PanelSpec? Spec)> ComparisonData { get; set; } = new();

    private string? _currentIdsid;
    public string CurrentIdsid => _currentIdsid ?? string.Empty;

    private string ExtractIdsid(string? raw)
    {
        if (string.IsNullOrEmpty(raw)) return string.Empty;
        int idx = raw.LastIndexOf('\\');
        return idx >= 0 ? raw[(idx + 1)..] : raw;
    }

    private bool MatchesPanelFeatureFilters(PanelSpec? spec)
    {
        if (FilterPanelFeatures == null || FilterPanelFeatures.Count == 0) return true;
        if (spec == null) return false;

        foreach (var feature in FilterPanelFeatures)
        {
            var prop = typeof(PanelSpec).GetProperty(feature);
            if (prop == null) return false;
            var valObj = prop.GetValue(spec);
            var val = valObj?.ToString();
            if (string.IsNullOrWhiteSpace(val)) return false;

            // Normalize
            var normalized = val.Trim();
            // Treat common negative/disabled values as not matching
            if (normalized.Equals("No", StringComparison.OrdinalIgnoreCase) ||
                normalized.Equals("False", StringComparison.OrdinalIgnoreCase) ||
                normalized.Equals("0", StringComparison.OrdinalIgnoreCase))
            {
                return false;
            }
        }
        return true;
    }

    public void OnGet()
    {
        if (_currentIdsid == null) _currentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);

        // Set defaults
        if (string.IsNullOrWhiteSpace(Platform)) Platform = "PTL";
        if (string.IsNullOrWhiteSpace(SKU)) SKU = "ALL";

        // Load all designs based on filter
        var query = _db.Designs.AsQueryable();

        if (!string.IsNullOrWhiteSpace(Platform) && Platform != "ALL")
        {
            query = query.Where(d => d.Platform == Platform);
        }

        if (!string.IsNullOrWhiteSpace(SKU) && SKU != "ALL")
        {
            query = query.Where(d => d.SKU == SKU);
        }

        var designs = query
            .AsNoTracking()
            .OrderBy(d => d.DesignName)
            .ToList();

        // Convert to DesignEntry
        foreach (var d in designs)
        {
            AllDesigns.Add(new DesignEntry
            {
                Id = d.DesignId,
                Name = d.DesignName,
                Platform = d.Platform,
                SKU = d.SKU,
                ODM = d.ODM,
                OEM = d.OEM,
                IF_ = d.If_,
                FCS = d.FCS
            });
        }

        // If designs are selected, load their panels
        if (SelectedDesignIds != null && SelectedDesignIds.Count > 0)
        {
            SelectedDesigns = AllDesigns.Where(d => SelectedDesignIds.Contains(d.Id)).ToList();

            var panels = _db.Panels
                .Where(p => SelectedDesignIds.Contains(p.design_id))
                .AsNoTracking()
                .ToList();

            // Preload specs only if we need to filter
            Dictionary<int, PanelSpec> specsByPanelId = new();
            if (FilterPanelFeatures != null && FilterPanelFeatures.Count > 0)
            {
                var pids = panels.Select(p => p.panel_id).ToList();
                specsByPanelId = _db.PanelSpecs
                    .Where(s => pids.Contains(s.panel_id))
                    .AsNoTracking()
                    .ToDictionary(s => s.panel_id, s => s);
            }

            foreach (var designId in SelectedDesignIds)
            {
                var list = panels
                    .Where(p => p.design_id == designId)
                    .Where(p =>
                    {
                        if (FilterPanelFeatures == null || FilterPanelFeatures.Count == 0) return true;
                        specsByPanelId.TryGetValue(p.panel_id, out var spec);
                        return MatchesPanelFeatureFilters(spec);
                    })
                    .Select(p => new PanelEntry
                    {
                        panel_id = p.panel_id,
                        design_id = p.design_id,
                        panel_vendor = p.panel_vendor,
                        tcon_vendor = p.tcon_vendor
                    })
                    .ToList();

                PanelsByDesign[designId] = list;
            }
        }

        // If panels are selected, load their specs for comparison
        if (SelectedPanelIds != null && SelectedPanelIds.Count > 0)
        {
            var panelsList = _db.Panels
                .Where(p => SelectedPanelIds.Contains(p.panel_id))
                .AsNoTracking()
                .ToList();

            var specs = _db.PanelSpecs
                .Where(s => SelectedPanelIds.Contains(s.panel_id))
                .AsNoTracking()
                .ToDictionary(s => s.panel_id, s => s);

            foreach (var panel in panelsList)
            {
                specs.TryGetValue(panel.panel_id, out var specForCompare);
                if (FilterPanelFeatures != null && FilterPanelFeatures.Count > 0 && !MatchesPanelFeatureFilters(specForCompare))
                {
                    // Skip non-matching selections when filter active
                    continue;
                }

                var design = SelectedDesigns.FirstOrDefault(d => d.Id == panel.design_id);
                if (design != null)
                {
                    ComparisonData.Add((
                        new PanelEntry
                        {
                            panel_id = panel.panel_id,
                            design_id = panel.design_id,
                            panel_vendor = panel.panel_vendor,
                            tcon_vendor = panel.tcon_vendor
                        },
                        design,
                        specForCompare
                    ));
                }
            }
        }
    }
}
