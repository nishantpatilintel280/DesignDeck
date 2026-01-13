using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using System.Text.Json;

namespace EP.Pages;

public class DetailsModel : PageModel
{
    private readonly ApplicationDbContext _db;
    public DetailsModel(ApplicationDbContext db) => _db = db;

    [BindProperty(SupportsGet = true)]
    public int? DesignId { get; set; }

    [BindProperty(SupportsGet = true)]
    public string? FilterPlatform { get; set; }
    [BindProperty(SupportsGet = true)]
    public string? FilterSKU { get; set; }

    [BindProperty(SupportsGet = true)]
    public List<string>? FilterPanelFeatures { get; set; }

    public DesignEntry? Design { get; private set; }
    public List<PanelEntry> Panels { get; private set; } = new();
    public List<(PanelEntry Panel, PanelSpec? Spec)> PanelsWithSpecs { get; private set; } = new();
    public Dictionary<int, Dictionary<string, PanelSpecOverride>> PanelOverrides { get; private set; } = new();

    // New: bindable specs used for editing/saving
    [BindProperty]
    public List<PanelSpecEdit> Specs { get; set; } = new();

    // New: bindable design edit model
    [BindProperty]
    public DesignEdit? DesignEditModel { get; set; }

    public List<DesignEntry> Designs { get; set; } = new(); // legacy list (not used when DesignId provided)
    [BindProperty]
    public List<int> SelectedIds { get; set; } = new();

    private string? _currentIdsid;
    private string ExtractIdsid(string? raw)
    {
        if (string.IsNullOrEmpty(raw)) return string.Empty;
        int idx = raw.LastIndexOf('\\');
        return idx >= 0 ? raw[(idx + 1)..] : raw;
    }

    public void OnGet()
    {
        // Resolve current user for ownership filtering if needed later
        _currentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);

        if (DesignId.HasValue && DesignId.Value > 0)
        {
            var d = _db.Designs.FirstOrDefault(x => x.DesignId == DesignId.Value);
            if (d != null)
            {
                Design = new DesignEntry
                {
                    Id = d.DesignId,
                    Name = d.DesignName,
                    Platform = d.Platform,
                    SKU = d.SKU,
                    ODM = d.ODM,
                    OEM = d.OEM,
                    IF_ = d.If_,
                    FCS = d.FCS,
                    NumberOfPanels = _db.Panels.Count(p => p.design_id == d.DesignId),
                    // Add all new fields from database
                    Segment = d.Segment,
                    Config = d.Config,
                    PlValues = d.PlValues,
                    MemoryCapacity = d.MemoryCapacity,
                    FormFactor = d.FormFactor,
                    AthenaEvoCandidate = d.AthenaEvoCandidate,
                    PO = d.PO,
                    POSite = d.POSite,
                    EvtStart = d.EvtStart,
                    EvtEnd = d.EvtEnd,
                    FvtStart = d.FvtStart,
                    FvtEnd = d.FvtEnd,
                    SitStart = d.SitStart,
                    SitEnd = d.SitEnd,
                    PvNeed = d.PvNeed,
                    ShipOutOs = d.ShipOutOs,
                    HwAvailability = d.HwAvailability,
                    Soc = d.Soc,
                    Pch = d.Pch,
                    Uma = d.Uma,
                    HybridGfx = d.HybridGfx,
                    DgpuVendor = d.DgpuVendor,
                    Gaming = d.Gaming,
                    AiFeatures = d.AiFeatures,
                    PostConfig = d.PostConfig,
                    Notes = d.Notes
                };

                // Initialize edit model with current values (excluding immutable fields)
                DesignEditModel = new DesignEdit
                {
                    DesignId = d.DesignId,
                    ODM = d.ODM,
                    IF_ = d.If_,
                    FCS = d.FCS,
                    Segment = d.Segment,
                    Config = d.Config,
                    PlValues = d.PlValues,
                    MemoryCapacity = d.MemoryCapacity,
                    FormFactor = d.FormFactor,
                    AthenaEvoCandidate = d.AthenaEvoCandidate,
                    PO = d.PO,
                    POSite = d.POSite,
                    EvtStart = d.EvtStart,
                    EvtEnd = d.EvtEnd,
                    FvtStart = d.FvtStart,
                    FvtEnd = d.FvtEnd,
                    SitStart = d.SitStart,
                    SitEnd = d.SitEnd,
                    PvNeed = d.PvNeed,
                    ShipOutOs = d.ShipOutOs,
                    HwAvailability = d.HwAvailability,
                    Soc = d.Soc,
                    Pch = d.Pch,
                    Uma = d.Uma,
                    HybridGfx = d.HybridGfx,
                    DgpuVendor = d.DgpuVendor,
                    Gaming = d.Gaming,
                    AiFeatures = d.AiFeatures,
                    PostConfig = d.PostConfig,
                    Notes = d.Notes
                };

                Panels = _db.Panels
                    .Where(p => p.design_id == d.DesignId)
                    .ToList();

                // panel_specs.panel_id is int; use int keys throughout
                var specIds = Panels.Select(p => p.panel_id).ToList();
                var specsById = _db.PanelSpecs
                    .Where(s => specIds.Contains(s.panel_id))
                    .ToDictionary(s => s.panel_id, s => s);

                PanelsWithSpecs = Panels
                    .Select(p => (p, specsById.TryGetValue(p.panel_id, out var spec) ? spec : null))
                    .ToList();

                // Load panel spec overrides for post-FCS modifications
                var panelIds = Panels.Select(p => p.panel_id).ToList();
                var overrides = _db.PanelSpecOverrides
                    .Where(o => panelIds.Contains(o.PanelId) && o.IsActive)
                    .AsNoTracking()
                    .ToList();

                PanelOverrides = overrides
                    .GroupBy(o => o.PanelId)
                    .ToDictionary(
                        g => g.Key,
                        g => g.ToDictionary(o => o.SpecField, o => o)
                    );

                // Apply feature filtering if requested
                if (FilterPanelFeatures != null && FilterPanelFeatures.Count > 0)
                {
                    PanelsWithSpecs = PanelsWithSpecs
                        .Where(ps => FilterPanelFeatures.All(f => HasFeature(ps.Spec, f)))
                        .ToList();
                }

                // Populate bindable Specs for editing (based on filtered panels for UI)
                Specs = PanelsWithSpecs.Select(ps => new PanelSpecEdit
                {
                    panel_id = ps.Spec?.panel_id ?? ps.Panel.panel_id,
                    PanelPortNumber = ps.Spec?.PanelPortNumber,
                    EdidVersion = ps.Spec?.EdidVersion,
                    Resolution = ps.Spec?.Resolution,
                    RR_Min_Hz = ps.Spec?.RR_Min_Hz,
                    RR_Max_Hz = ps.Spec?.RR_Max_Hz,
                    Aspect_Ratio = ps.Spec?.Aspect_Ratio,
                    Htotal = ps.Spec?.Htotal,
                    Vtotal = ps.Spec?.Vtotal,
                    Screen_Size_Inch = ps.Spec?.Screen_Size_Inch,
                    Color_Depth = ps.Spec?.Color_Depth,
                    FRC = ps.Spec?.FRC,
                    Data_Lane_Count = ps.Spec?.Data_Lane_Count,
                    Data_Link_Rate = ps.Spec?.Data_Link_Rate,
                    PSR1_Supported = ps.Spec?.PSR1_Supported,
                    PSR2_Supported = ps.Spec?.PSR2_Supported,
                    PSR2_ET_Supported = ps.Spec?.PSR2_ET_Supported,
                    Intel_LRR_Version = ps.Spec?.Intel_LRR_Version,
                    HDR_Supported = ps.Spec?.HDR_Supported,
                    VRR_Supported = ps.Spec?.VRR_Supported,
                    VDSC_Version = ps.Spec?.VDSC_Version,
                    CoG_MSO = ps.Spec?.CoG_MSO,
                    IIDT_UBRR = ps.Spec?.IIDT_UBRR,
                    Camera_Sensor_For_UBRR = ps.Spec?.Camera_Sensor_For_UBRR,
                    Seamless_Drrs = ps.Spec?.Seamless_Drrs,
                    xPST_Supported = ps.Spec?.xPST_Supported,
                    xPST_Level = ps.Spec?.xPST_Level,
                    OPST_ELP = ps.Spec?.OPST_ELP,
                    DE_Early_Wake = ps.Spec?.DE_Early_Wake,
                    EPSM60 = ps.Spec?.EPSM60,
                    EG_Endurance_Gaming = ps.Spec?.EG_Endurance_Gaming,
                    EDP1_5_Panel_Replay = ps.Spec?.EDP1_5_Panel_Replay,
                    EDP1_5_PR_ET = ps.Spec?.EDP1_5_PR_ET,
                    EDP1_5_CMRR = ps.Spec?.EDP1_5_CMRR,
                    EDP1_5_Periodic_ASSDP = ps.Spec?.EDP1_5_Periodic_ASSDP,
                    EDP1_5_LOBF = ps.Spec?.EDP1_5_LOBF,
                    Other_Features = ps.Spec?.Other_Features,
                    Color_Gamut_sRGB = ps.Spec?.Color_Gamut_sRGB,
                    Color_Gamut_DCI_P3 = ps.Spec?.Color_Gamut_DCI_P3,
                    Color_Gamut_AdobeRGB = ps.Spec?.Color_Gamut_AdobeRGB,
                    Color_Gamut_NTSC = ps.Spec?.Color_Gamut_NTSC,
                    Max_Brightness_Nits = ps.Spec?.Max_Brightness_Nits,
                    Backlight_Light_Source = ps.Spec?.Backlight_Light_Source,
                    Dimming_Control = ps.Spec?.Dimming_Control,
                    Backlight_Control = ps.Spec?.Backlight_Control,
                    Brightness_Precision_Bits = ps.Spec?.Brightness_Precision_Bits,
                    Panel_Part_Number = ps.Spec?.Panel_Part_Number,
                    TCon_Part_Number = ps.Spec?.TCon_Part_Number,
                    Panel_TCon_Release_Year = ps.Spec?.Panel_TCon_Release_Year,
                    Panel_HW_Version = ps.Spec?.Panel_HW_Version,
                    Touch_Host_Controller = ps.Spec?.Touch_Host_Controller,
                    Touch_Screen_or_Pad = ps.Spec?.Touch_Screen_or_Pad,
                    Touch_Host_Controller_Vendor = ps.Spec?.Touch_Host_Controller_Vendor,
                    Touch_Support = ps.Spec?.Touch_Support,
                    Touch_Type = ps.Spec?.Touch_Type,
                    Touch_Interface = ps.Spec?.Touch_Interface,
                    Stylus_Protocol = ps.Spec?.Stylus_Protocol,
                    Backlight_Power_Consumption = ps.Spec?.Backlight_Power_Consumption,
                    Data_Logic_Power_Consumption = ps.Spec?.Data_Logic_Power_Consumption,
                    Total_Power_Consumption = ps.Spec?.Total_Power_Consumption,
                    Panel_DC_Voltage = ps.Spec?.Panel_DC_Voltage,
                    VBatt_POL = ps.Spec?.VBatt_POL,
                }).ToList();
            }
            return; // show single design view
        }

        // Fallback: old behavior list by filters (owned by user optionally)
        if (string.IsNullOrWhiteSpace(FilterPlatform))
            FilterPlatform = "PTL";

        var query = _db.Designs.AsQueryable();
        if (!string.IsNullOrWhiteSpace(FilterPlatform) && FilterPlatform != "ALL")
            query = query.Where(d => d.Platform == FilterPlatform);
        if (!string.IsNullOrWhiteSpace(FilterSKU) && FilterSKU != "ALL" && FilterSKU != "")
            query = query.Where(d => d.SKU == FilterSKU);

        Designs = query
            .OrderBy(d => d.DesignId)
            .Select(d => new DesignEntry
            {
                Id = d.DesignId,
                Name = d.DesignName,
                NumberOfPanels = d.NumberOfPanels,
                IF_ = d.If_,
                FCS = d.FCS
            })
            .ToList();
    }

    private static bool HasFeature(PanelSpec? spec, string feature)
    {
        if (spec == null || string.IsNullOrWhiteSpace(feature)) return false;
        var prop = typeof(PanelSpec).GetProperty(feature);
        if (prop == null) return false;
        var valObj = prop.GetValue(spec);
        var s = valObj?.ToString()?.Trim();
        if (string.IsNullOrEmpty(s)) return false;
        var lower = s.ToLowerInvariant();
        if (lower == "no" || lower == "false" || lower == "0") return false;
        // Consider any non-empty non-negative flag or version/text value as enabled
        return lower == "yes" || lower == "y" || lower == "true" || lower == "1" || lower == "supported" || lower == "present" || lower == "enabled" || lower == "ok" || lower == "available" || lower == "pass" || char.IsLetterOrDigit(lower[0]);
    }

    public IActionResult OnPostUploadFile(int DesignId, string FileType, IFormFile File)
    {
        if (File == null || string.IsNullOrEmpty(FileType)) return new JsonResult(new { success = false });

        var uploadsFolder = Path.Combine("wwwroot", "uploads");
        Directory.CreateDirectory(uploadsFolder);

        var fileName = $"{DesignId}_{FileType}_{Path.GetFileName(File.FileName)}";
        var filePath = Path.Combine(uploadsFolder, fileName);

        using (var stream = new FileStream(filePath, FileMode.Create))
        {
            File.CopyTo(stream);
        }

        // Store file path in session
        var key = $"DesignFiles_{DesignId}";
        Dictionary<string, string> fileDict;
        var sessionData = HttpContext.Session.GetString(key);
        if (sessionData != null)
            fileDict = JsonSerializer.Deserialize<Dictionary<string, string>>(sessionData)!;
        else
            fileDict = new Dictionary<string, string>();

        fileDict[FileType] = filePath;
        HttpContext.Session.SetString(key, JsonSerializer.Serialize(fileDict));

        return new JsonResult(new { success = true, path = filePath });
    }

    public IActionResult OnPost()
    {
        if (SelectedIds == null || !SelectedIds.Any())
        {
            return Redirect($"/AddPanelDetails");
        }
        var query = string.Join("&", SelectedIds.Select(id => $"SelectedIds={id}"));
        return Redirect($"/AddPanelDetails?{query}");
    }

    // Save handler for edited specs
    public IActionResult OnPostSaveSpecs()
    {
        if (!DesignId.HasValue || DesignId.Value <= 0)
            return BadRequest("Missing DesignId");

        // Load panels for design to validate panel ownership
        var panelIds = _db.Panels.Where(p => p.design_id == DesignId.Value).Select(p => p.panel_id).ToHashSet();
        if (Specs == null || Specs.Count == 0)
            return RedirectToPage(new { DesignId });

        foreach (var s in Specs)
        {
            if (!panelIds.Contains(s.panel_id)) continue; // skip mismatched panels

            var spec = _db.PanelSpecs.FirstOrDefault(x => x.panel_id == s.panel_id);
            if (spec == null)
            {
                spec = new PanelSpec { panel_id = s.panel_id };
                _db.PanelSpecs.Add(spec);
            }

            // Map all editable fields
            spec.PanelPortNumber = s.PanelPortNumber;
            spec.EdidVersion = s.EdidVersion;
            spec.Resolution = s.Resolution;
            spec.RR_Min_Hz = s.RR_Min_Hz;
            spec.RR_Max_Hz = s.RR_Max_Hz;
            spec.Aspect_Ratio = s.Aspect_Ratio;
            spec.Htotal = s.Htotal;
            spec.Vtotal = s.Vtotal;
            spec.Screen_Size_Inch = s.Screen_Size_Inch;
            spec.Color_Depth = s.Color_Depth;
            spec.FRC = s.FRC;
            spec.Data_Lane_Count = s.Data_Lane_Count;
            spec.Data_Link_Rate = s.Data_Link_Rate;
            spec.PSR1_Supported = s.PSR1_Supported;
            spec.PSR2_Supported = s.PSR2_Supported;
            spec.PSR2_ET_Supported = s.PSR2_ET_Supported;
            spec.Intel_LRR_Version = s.Intel_LRR_Version;
            spec.HDR_Supported = s.HDR_Supported;
            spec.VRR_Supported = s.VRR_Supported;
            spec.VDSC_Version = s.VDSC_Version;
            spec.CoG_MSO = s.CoG_MSO;
            spec.IIDT_UBRR = s.IIDT_UBRR;
            spec.Camera_Sensor_For_UBRR = s.Camera_Sensor_For_UBRR;
            spec.Seamless_Drrs = s.Seamless_Drrs;
            spec.xPST_Supported = s.xPST_Supported;
            spec.xPST_Level = s.xPST_Level;
            spec.OPST_ELP = s.OPST_ELP;
            spec.DE_Early_Wake = s.DE_Early_Wake;
            spec.EPSM60 = s.EPSM60;
            spec.EG_Endurance_Gaming = s.EG_Endurance_Gaming;
            spec.EDP1_5_Panel_Replay = s.EDP1_5_Panel_Replay;
            spec.EDP1_5_PR_ET = s.EDP1_5_PR_ET;
            spec.EDP1_5_CMRR = s.EDP1_5_CMRR;
            spec.EDP1_5_Periodic_ASSDP = s.EDP1_5_Periodic_ASSDP;
            spec.EDP1_5_LOBF = s.EDP1_5_LOBF;
            spec.Other_Features = s.Other_Features;
            spec.Color_Gamut_sRGB = s.Color_Gamut_sRGB;
            spec.Color_Gamut_DCI_P3 = s.Color_Gamut_DCI_P3;
            spec.Color_Gamut_AdobeRGB = s.Color_Gamut_AdobeRGB;
            spec.Color_Gamut_NTSC = s.Color_Gamut_NTSC;
            spec.Max_Brightness_Nits = s.Max_Brightness_Nits;
            spec.Backlight_Light_Source = s.Backlight_Light_Source;
            spec.Dimming_Control = s.Dimming_Control;
            spec.Backlight_Control = s.Backlight_Control;
            spec.Brightness_Precision_Bits = s.Brightness_Precision_Bits;
            spec.Panel_Part_Number = s.Panel_Part_Number;
            spec.TCon_Part_Number = s.TCon_Part_Number;
            spec.Panel_TCon_Release_Year = s.Panel_TCon_Release_Year;
            spec.Panel_HW_Version = s.Panel_HW_Version;
            spec.Touch_Host_Controller = s.Touch_Host_Controller;
            spec.Touch_Screen_or_Pad = s.Touch_Screen_or_Pad;
            spec.Touch_Host_Controller_Vendor = s.Touch_Host_Controller_Vendor;
            spec.Touch_Support = s.Touch_Support;
            spec.Touch_Type = s.Touch_Type;
            spec.Touch_Interface = s.Touch_Interface;
            spec.Stylus_Protocol = s.Stylus_Protocol;
            spec.Backlight_Power_Consumption = s.Backlight_Power_Consumption;
            spec.Data_Logic_Power_Consumption = s.Data_Logic_Power_Consumption;
            spec.Total_Power_Consumption = s.Total_Power_Consumption;
            spec.Panel_DC_Voltage = s.Panel_DC_Voltage;
            spec.VBatt_POL = s.VBatt_POL;
        }

        _db.SaveChanges();
        TempData["WelcomeMessage"] = "Panel specs saved.";
        return RedirectToPage(new { DesignId });
    }

    // New: Save handler for edited design fields
    public IActionResult OnPostSaveDesign()
    {
        if (DesignEditModel == null || DesignEditModel.DesignId <= 0)
            return BadRequest("Missing design data");

        var d = _db.Designs.FirstOrDefault(x => x.DesignId == DesignEditModel.DesignId);
        if (d == null) return NotFound();

        // Update editable fields only (exclude Name, Platform, SKU, OEM)
        d.ODM = DesignEditModel.ODM;
        d.If_ = DesignEditModel.IF_;
        d.FCS = DesignEditModel.FCS;
        d.Segment = DesignEditModel.Segment;
        d.Config = DesignEditModel.Config;
        d.PlValues = DesignEditModel.PlValues;
        d.MemoryCapacity = DesignEditModel.MemoryCapacity;
        d.FormFactor = DesignEditModel.FormFactor;
        d.AthenaEvoCandidate = DesignEditModel.AthenaEvoCandidate;
        d.PO = DesignEditModel.PO;
        d.POSite = DesignEditModel.POSite;
        d.EvtStart = DesignEditModel.EvtStart;
        d.EvtEnd = DesignEditModel.EvtEnd;
        d.FvtStart = DesignEditModel.FvtStart;
        d.FvtEnd = DesignEditModel.FvtEnd;
        d.SitStart = DesignEditModel.SitStart;
        d.SitEnd = DesignEditModel.SitEnd;
        d.PvNeed = DesignEditModel.PvNeed;
        d.ShipOutOs = DesignEditModel.ShipOutOs;
        d.HwAvailability = DesignEditModel.HwAvailability;
        d.Soc = DesignEditModel.Soc;
        d.Pch = DesignEditModel.Pch;
        d.Uma = DesignEditModel.Uma;
        d.HybridGfx = DesignEditModel.HybridGfx;
        d.DgpuVendor = DesignEditModel.DgpuVendor;
        d.Gaming = DesignEditModel.Gaming;
        d.AiFeatures = DesignEditModel.AiFeatures;
        d.PostConfig = DesignEditModel.PostConfig;
        d.Notes = DesignEditModel.Notes;

        _db.SaveChanges();
        TempData["WelcomeMessage"] = "Design details saved.";
        return RedirectToPage(new { DesignId = d.DesignId });
    }
}

// Bindable DTO mirroring PanelSpec for form binding
public class PanelSpecEdit
{
    public int panel_id { get; set; }
    public string? PanelPortNumber { get; set; }
    public string? EdidVersion { get; set; }
    public string? Resolution { get; set; }
    public string? RR_Min_Hz { get; set; }
    public string? RR_Max_Hz { get; set; }
    public string? Aspect_Ratio { get; set; }
    public string? Htotal { get; set; }
    public string? Vtotal { get; set; }
    public string? Screen_Size_Inch { get; set; }
    public string? Color_Depth { get; set; }
    public string? FRC { get; set; }
    public string? Data_Lane_Count { get; set; }
    public string? Data_Link_Rate { get; set; }
    public string? PSR1_Supported { get; set; }
    public string? PSR2_Supported { get; set; }
    public string? PSR2_ET_Supported { get; set; }
    public string? Intel_LRR_Version { get; set; }
    public string? HDR_Supported { get; set; }
    public string? VRR_Supported { get; set; }
    public string? VDSC_Version { get; set; }
    public string? CoG_MSO { get; set; }
    public string? IIDT_UBRR { get; set; }
    public string? Camera_Sensor_For_UBRR { get; set; }
    public string? Seamless_Drrs { get; set; }
    public string? xPST_Supported { get; set; }
    public string? xPST_Level { get; set; }
    public string? OPST_ELP { get; set; }
    public string? DE_Early_Wake { get; set; }
    public string? EPSM60 { get; set; }
    public string? EG_Endurance_Gaming { get; set; }
    public string? EDP1_5_Panel_Replay { get; set; }
    public string? EDP1_5_PR_ET { get; set; }
    public string? EDP1_5_CMRR { get; set; }
    public string? EDP1_5_Periodic_ASSDP { get; set; }
    public string? EDP1_5_LOBF { get; set; }
    public string? Other_Features { get; set; }
    public string? Color_Gamut_sRGB { get; set; }
    public string? Color_Gamut_DCI_P3 { get; set; }
    public string? Color_Gamut_AdobeRGB { get; set; }
    public string? Color_Gamut_NTSC { get; set; }
    public string? Max_Brightness_Nits { get; set; }
    public string? Backlight_Light_Source { get; set; }
    public string? Dimming_Control { get; set; }
    public string? Backlight_Control { get; set; }
    public string? Brightness_Precision_Bits { get; set; }
    public string? Panel_Part_Number { get; set; }
    public string? TCon_Part_Number { get; set; }
    public string? Panel_TCon_Release_Year { get; set; }
    public string? Panel_HW_Version { get; set; }
    public string? Touch_Host_Controller { get; set; }
    public string? Touch_Screen_or_Pad { get; set; }
    public string? Touch_Host_Controller_Vendor { get; set; }
    public string? Touch_Support { get; set; }
    public string? Touch_Type { get; set; }
    public string? Touch_Interface { get; set; }
    public string? Stylus_Protocol { get; set; }
    public string? Backlight_Power_Consumption { get; set; }
    public string? Data_Logic_Power_Consumption { get; set; }
    public string? Total_Power_Consumption { get; set; }
    public string? Panel_DC_Voltage { get; set; }
    public string? VBatt_POL { get; set; }
}

// Bindable DTO for editing design (excluding immutable fields)
public class DesignEdit
{
    public int DesignId { get; set; }
    public string? ODM { get; set; }
    public string? IF_ { get; set; }
    public string? FCS { get; set; }
    public string? Segment { get; set; }
    public string? Config { get; set; }
    public string? PlValues { get; set; }
    public string? MemoryCapacity { get; set; }
    public string? FormFactor { get; set; }
    public string? AthenaEvoCandidate { get; set; }
    public string? PO { get; set; }
    public string? POSite { get; set; }
    public string? EvtStart { get; set; }
    public string? EvtEnd { get; set; }
    public string? FvtStart { get; set; }
    public string? FvtEnd { get; set; }
    public string? SitStart { get; set; }
    public string? SitEnd { get; set; }
    public string? PvNeed { get; set; }
    public string? ShipOutOs { get; set; }
    public string? HwAvailability { get; set; }
    public string? Soc { get; set; }
    public string? Pch { get; set; }
    public string? Uma { get; set; }
    public string? HybridGfx { get; set; }
    public string? DgpuVendor { get; set; }
    public string? Gaming { get; set; }
    public string? AiFeatures { get; set; }
    public string? PostConfig { get; set; }
    public string? Notes { get; set; }
}

