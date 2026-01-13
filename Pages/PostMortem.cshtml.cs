using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using System.Globalization;

namespace EP.Pages;

public class PostMortemModel : PageModel
{
    private readonly ApplicationDbContext _db;

    public PostMortemModel(ApplicationDbContext db)
    {
        _db = db;
    }

    [BindProperty(SupportsGet = true)]
    public int DesignId { get; set; }

    public DesignEntry? Design { get; set; }
    public List<(PanelEntry Panel, PanelSpec? Spec, Dictionary<string, string?> Overrides)> PanelsData { get; set; } = new();

    public bool IsFcsPassed { get; set; }
    public string? FcsDate { get; set; }

    private string? _currentIdsid;
    public string CurrentIdsid => _currentIdsid ?? string.Empty;

    // List of spec fields that can be enabled/disabled
    public static readonly List<string> EditableSpecFields = new()
    {
        "PSR1_Supported", "PSR2_Supported", "PSR2_ET_Supported",
        "HDR_Supported", "VRR_Supported", "VDSC_Version",
        "Intel_LRR_Version", "Seamless_Drrs", "xPST_Supported",
        "OPST_ELP", "DE_Early_Wake", "EPSM60", "EG_Endurance_Gaming",
        "EDP1_5_Panel_Replay", "EDP1_5_PR_ET", "EDP1_5_CMRR",
        "EDP1_5_Periodic_ASSDP", "EDP1_5_LOBF", "IIDT_UBRR",
        "CoG_MSO", "xPST_Level"
    };

    private string ExtractIdsid(string? raw)
    {
        if (string.IsNullOrEmpty(raw)) return string.Empty;
        int idx = raw.LastIndexOf('\\');
        return idx >= 0 ? raw[(idx + 1)..] : raw;
    }

    public void OnGet()
    {
        if (_currentIdsid == null) _currentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);

        // Load design
        var d = _db.Designs.FirstOrDefault(x => x.DesignId == DesignId);
        if (d == null)
        {
            return;
        }

        Design = new DesignEntry
        {
            Id = d.DesignId,
            Name = d.DesignName,
            Platform = d.Platform,
            SKU = d.SKU,
            ODM = d.ODM,
            OEM = d.OEM,
            IF_ = d.If_,
            FCS = d.FCS
        };

        FcsDate = d.FCS;

        // Check if FCS date has passed
        IsFcsPassed = CheckIfFcsPassed(d.FCS);

        // Load all panels for this design
        var panels = _db.Panels
            .Where(p => p.design_id == DesignId)
            .AsNoTracking()
            .ToList();

        var panelIds = panels.Select(p => p.panel_id).ToList();

        // Load specs
        var specs = _db.PanelSpecs
            .Where(s => panelIds.Contains(s.panel_id))
            .AsNoTracking()
            .ToDictionary(s => s.panel_id, s => s);

        // Load overrides
        var overrides = _db.PanelSpecOverrides
            .Where(o => panelIds.Contains(o.PanelId) && o.IsActive)
            .AsNoTracking()
            .GroupBy(o => o.PanelId)
            .ToDictionary(
                g => g.Key,
                g => g.ToDictionary(o => o.SpecField, o => o.OverrideValue)
            );

        // Combine data
        foreach (var panel in panels)
        {
            specs.TryGetValue(panel.panel_id, out var spec);
            overrides.TryGetValue(panel.panel_id, out var panelOverrides);

            PanelsData.Add((panel, spec, panelOverrides ?? new Dictionary<string, string?>()));
        }
    }

    public IActionResult OnPostSaveOverrides(
        [FromForm] List<int> PanelIds,
        [FromForm] List<string> SpecFields,
        [FromForm] List<string> EnabledValues)
    {
        if (_currentIdsid == null) _currentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);

        // Debug: Log what we received
        Console.WriteLine("=== POST SAVE OVERRIDES CALLED ===");
        Console.WriteLine($"PanelIds: {PanelIds?.Count ?? 0} items");
        Console.WriteLine($"SpecFields: {SpecFields?.Count ?? 0} items");
        Console.WriteLine($"EnabledValues: {EnabledValues?.Count ?? 0} items");
        Console.WriteLine($"DesignId: {DesignId}");
        
        if (PanelIds != null && PanelIds.Count > 0)
        {
            Console.WriteLine("First 5 entries:");
            for (int i = 0; i < Math.Min(5, PanelIds.Count); i++)
            {
                Console.WriteLine($"  [{i}] PanelId={PanelIds[i]}, Field={SpecFields?[i]}, Value={EnabledValues?[i]}");
            }
        }

        if (PanelIds == null || SpecFields == null || EnabledValues == null ||
            PanelIds.Count != SpecFields.Count || PanelIds.Count != EnabledValues.Count)
        {
            Console.WriteLine($"ERROR: Array mismatch - PanelIds:{PanelIds?.Count}, SpecFields:{SpecFields?.Count}, EnabledValues:{EnabledValues?.Count}");
            return BadRequest("Invalid form data");
        }

        // Load design to get FCS date
        var design = _db.Designs.FirstOrDefault(d => d.DesignId == DesignId);
        if (design == null)
        {
            return NotFound("Design not found");
        }

        // Check if FCS has passed
        if (!CheckIfFcsPassed(design.FCS))
        {
            TempData["ErrorMessage"] = "Cannot modify specs before FCS date has passed.";
            return RedirectToPage(new { DesignId });
        }

        try
        {
            // Process each override
            for (int i = 0; i < PanelIds.Count; i++)
            {
                var panelId = PanelIds[i];
                var specField = SpecFields[i];
                var enabledValue = EnabledValues[i]?.Trim() ?? string.Empty;

                // Get original value from panel_specs (with tracking)
                var spec = _db.PanelSpecs.FirstOrDefault(s => s.panel_id == panelId);
                if (spec == null)
                {
                    Console.WriteLine($"Panel spec not found for panel_id: {panelId}");
                    continue;
                }

                var originalValue = GetSpecFieldValue(spec, specField);

                Console.WriteLine($"Processing panel {panelId}, field {specField}: original='{originalValue}', enabled='{enabledValue}'");

                // Check if value actually changed
                if (string.Equals(originalValue?.Trim(), enabledValue, StringComparison.OrdinalIgnoreCase))
                {
                    Console.WriteLine($"Value unchanged for panel {panelId}, field {specField} - deactivating overrides");
                    
                    // Value matches original - deactivate any existing override
                    var existingOverrides = _db.PanelSpecOverrides
                        .Where(o => o.PanelId == panelId && o.SpecField == specField && o.IsActive)
                        .ToList();

                    foreach (var ex in existingOverrides)
                    {
                        ex.IsActive = false;
                        _db.Entry(ex).State = EntityState.Modified;
                        Console.WriteLine($"Deactivated override {ex.OverrideId}");
                    }
                }
                else
                {
                    Console.WriteLine($"Value changed for panel {panelId}, field {specField} - creating/updating override");
                    
                    // Value changed - create or update override
                    var existingOverride = _db.PanelSpecOverrides
                        .FirstOrDefault(o => o.PanelId == panelId && o.SpecField == specField && o.IsActive);

                    if (existingOverride != null)
                    {
                        Console.WriteLine($"Updating existing override {existingOverride.OverrideId}");
                        
                        // Update existing override
                        existingOverride.OverrideValue = enabledValue;
                        existingOverride.ModifiedDate = DateTime.Now;
                        existingOverride.ModifiedBy = CurrentIdsid;
                        _db.Entry(existingOverride).State = EntityState.Modified;
                    }
                    else
                    {
                        Console.WriteLine($"Creating new override for panel {panelId}, field {specField}");
                        
                        // Create new override
                        var newOverride = new PanelSpecOverride
                        {
                            PanelId = panelId,
                            SpecField = specField,
                            OriginalValue = originalValue,
                            OverrideValue = enabledValue,
                            ModifiedDate = DateTime.Now,
                            ModifiedBy = CurrentIdsid,
                            DesignFcsDate = design.FCS,
                            IsActive = true
                        };
                        _db.PanelSpecOverrides.Add(newOverride);
                        Console.WriteLine($"Added new override: PanelId={panelId}, Field={specField}, Value={enabledValue}");
                    }
                }
            }

            // Save all changes
            var changeCount = _db.SaveChanges();
            Console.WriteLine($"SaveChanges completed: {changeCount} records affected");
            
            TempData["SuccessMessage"] = $"Panel specifications updated successfully. {changeCount} changes saved.";
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error saving overrides: {ex.Message}");
            Console.WriteLine($"Stack trace: {ex.StackTrace}");
            TempData["ErrorMessage"] = $"Error saving changes: {ex.Message}";
        }
        
        return RedirectToPage(new { DesignId });
    }

    private bool CheckIfFcsPassed(string? fcsDate)
    {
        if (string.IsNullOrWhiteSpace(fcsDate)) return false;

        // Clean the date string (remove time component if present)
        var cleanDate = fcsDate.Trim();
        var spaceIdx = cleanDate.IndexOf(' ');
        if (spaceIdx > 0)
        {
            cleanDate = cleanDate[..spaceIdx];
        }

        // Try to parse the date
        if (DateTime.TryParseExact(cleanDate,
            new[] { "dd-MM-yyyy", "MM-dd-yyyy", "yyyy-MM-dd" },
            CultureInfo.InvariantCulture,
            DateTimeStyles.None,
            out var parsedDate))
        {
            return parsedDate < DateTime.Today;
        }

        return false;
    }

    private string? GetSpecFieldValue(PanelSpec spec, string fieldName)
    {
        return fieldName switch
        {
            "PSR1_Supported" => spec.PSR1_Supported,
            "PSR2_Supported" => spec.PSR2_Supported,
            "PSR2_ET_Supported" => spec.PSR2_ET_Supported,
            "HDR_Supported" => spec.HDR_Supported,
            "VRR_Supported" => spec.VRR_Supported,
            "VDSC_Version" => spec.VDSC_Version,
            "Intel_LRR_Version" => spec.Intel_LRR_Version,
            "Seamless_Drrs" => spec.Seamless_Drrs,
            "xPST_Supported" => spec.xPST_Supported,
            "xPST_Level" => spec.xPST_Level,
            "OPST_ELP" => spec.OPST_ELP,
            "DE_Early_Wake" => spec.DE_Early_Wake,
            "EPSM60" => spec.EPSM60,
            "EG_Endurance_Gaming" => spec.EG_Endurance_Gaming,
            "EDP1_5_Panel_Replay" => spec.EDP1_5_Panel_Replay,
            "EDP1_5_PR_ET" => spec.EDP1_5_PR_ET,
            "EDP1_5_CMRR" => spec.EDP1_5_CMRR,
            "EDP1_5_Periodic_ASSDP" => spec.EDP1_5_Periodic_ASSDP,
            "EDP1_5_LOBF" => spec.EDP1_5_LOBF,
            "IIDT_UBRR" => spec.IIDT_UBRR,
            "CoG_MSO" => spec.CoG_MSO,
            _ => null
        };
    }
}
