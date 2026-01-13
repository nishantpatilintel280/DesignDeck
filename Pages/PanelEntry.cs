using System.ComponentModel.DataAnnotations;

public class PanelEntry
{
    [Key]
    public int panel_id { get; set; }
    public int design_id { get; set; }
    public string panel_vendor { get; set; }
    public string tcon_vendor { get; set; }
    // Optional spec linkage (same PK as in panel_specs)
    // public PanelSpec? Spec { get; set; }
}