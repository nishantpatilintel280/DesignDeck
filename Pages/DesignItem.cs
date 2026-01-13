namespace EP.Pages;

public class DesignEntry
{
    public int Id { get; set; }
    public string Platform { get; set; } = string.Empty;
    public string SKU { get; set; } = string.Empty;
    public string ODM { get; set; } = string.Empty;
    public string OEM { get; set; } = string.Empty;
    public string Name { get; set; } = string.Empty;
    public string IF_ { get; set; } = string.Empty;
    public string FCS { get; set; } = string.Empty;
    public int NumberOfPanels { get; set; }

    // Additional properties from database
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
