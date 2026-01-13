using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

public class ApplicationDbContext : DbContext
{
    // Constructor that accepts options
    public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options)
        : base(options)
    {
    }

    // Define your tables here
    public DbSet<User> Users { get; set; }
    public DbSet<Design> Designs { get; set; }
    public DbSet<DesignOwner> DesignOwners { get; set; }
    public DbSet<PanelEntry> Panels { get; set; }
    public DbSet<PanelSpec> PanelSpecs { get; set; }
    public DbSet<SecondaryOwner> SecondaryOwners { get; set; }
    public DbSet<PanelSpecOverride> PanelSpecOverrides { get; set; }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);
        // Composite key for design_owners
        modelBuilder.Entity<DesignOwner>().HasKey(o => new { o.DesignId, o.Idsid });
        // Explicitly configure primary key for User (idsid varchar)
        modelBuilder.Entity<User>().HasKey(u => u.idsid);
        // PanelSpecs primary key (int)
        modelBuilder.Entity<PanelSpec>().HasKey(p => p.panel_id);
        // Secondary owners mapping
        modelBuilder.Entity<SecondaryOwner>()
            .ToTable("secondary_owners")
            .HasKey(so => new { so.DesignId, so.Idsid });

        // Ensure DateOnly maps to SQL DATE
        var dateOnlyConverter = new ValueConverter<DateOnly, DateTime>(
            d => d.ToDateTime(TimeOnly.MinValue),
            dt => DateOnly.FromDateTime(dt)
        );
        modelBuilder.Entity<SecondaryOwner>()
            .Property(p => p.StartDate)
            .HasConversion(dateOnlyConverter);
        modelBuilder.Entity<SecondaryOwner>()
            .Property(p => p.EndDate)
            .HasConversion(dateOnlyConverter);

        // Configure PanelSpecOverride table
        modelBuilder.Entity<PanelSpecOverride>()
            .ToTable("panel_spec_overrides")
            .HasKey(o => o.OverrideId);

        modelBuilder.Entity<PanelSpecOverride>()
            .HasIndex(o => new { o.PanelId, o.SpecField, o.IsActive })
            .HasDatabaseName("IX_panel_spec_overrides_panel_field");
    }
}

// Your User model class
[Table("users")]
public class User
{
    [Key]
    [Column("idsid")]
    public string idsid { get; set; } = string.Empty; // varchar PK

    [Column("role_id")]
    public int role { get; set; }
}

[Table("designs")] // maps to existing table name
public class Design
{
    [Key]
    [Column("design_id")]
    public int DesignId { get; set; }

    [Column("platform")]
    public string Platform { get; set; } = string.Empty;

    [Column("sku")]
    public string SKU { get; set; } = string.Empty;

    [Column("odm")]
    public string ODM { get; set; } = string.Empty;

    [Column("oem")]
    public string OEM { get; set; } = string.Empty;

    [Column("design_name")]
    public string DesignName { get; set; } = string.Empty;

    [Column("if_")]
    public string If_ { get; set; } = string.Empty; // interface / qualifier

    [Column("fcs")]
    public string FCS { get; set; } = string.Empty; // functional / feature code

    [Column("number_of_panels")]
    public int NumberOfPanels { get; set; }

    // Additional columns from database
    [Column("segment")]
    public string? Segment { get; set; }

    [Column("config")]
    public string? Config { get; set; }

    [Column("pl_values")]
    public string? PlValues { get; set; }

    [Column("memory_capacity")]
    public string? MemoryCapacity { get; set; }

    [Column("form_factor")]
    public string? FormFactor { get; set; }

    [Column("athena_evo_candidate")]
    public string? AthenaEvoCandidate { get; set; }

    [Column("po")]
    public string? PO { get; set; }

    [Column("po_site")]
    public string? POSite { get; set; }

    [Column("evt_start")]
    public string? EvtStart { get; set; }

    [Column("evt_end")]
    public string? EvtEnd { get; set; }

    [Column("fvt_start")]
    public string? FvtStart { get; set; }

    [Column("fvt_end")]
    public string? FvtEnd { get; set; }

    [Column("sit_start")]
    public string? SitStart { get; set; }

    [Column("sit_end")]
    public string? SitEnd { get; set; }

    [Column("pv_need")]
    public string? PvNeed { get; set; }

    [Column("ship_out_os")]
    public string? ShipOutOs { get; set; }

    [Column("hw_availability")]
    public string? HwAvailability { get; set; }

    [Column("soc")]
    public string? Soc { get; set; }

    [Column("pch")]
    public string? Pch { get; set; }

    [Column("uma")]
    public string? Uma { get; set; }

    [Column("hybrid_gfx")]
    public string? HybridGfx { get; set; }

    [Column("dgpu_vendor")]
    public string? DgpuVendor { get; set; }

    [Column("gaming")]
    public string? Gaming { get; set; }

    [Column("ai_features")]
    public string? AiFeatures { get; set; }

    [Column("post_config")]
    public string? PostConfig { get; set; }

    [Column("notes")]
    public string? Notes { get; set; }
}

[Table("design_owners")] // maps to existing table name
public class DesignOwner
{
    [Column("design_id")]
    public int DesignId { get; set; }

    [Column("idsid")]
    public string Idsid { get; set; } = string.Empty;
}

[Table("panel_specs")]
public class PanelSpec
{
    [Key]
    [Column("panel_id")]
    public int panel_id { get; set; } // int PK

    // All columns nvarchar
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
    public string? IsContinuousFreqSupported { get; set; }
}

[Table("secondary_owners")]
public class SecondaryOwner
{
    [Column("design_id")]
    public int DesignId { get; set; }

    [Column("idsid")]
    public string Idsid { get; set; } = string.Empty;

    [Column("secondary_idsid")]
    public string SecondaryIdsid { get; set; } = string.Empty;

    [Column("start_date")]
    public DateOnly StartDate { get; set; }

    [Column("end_date")]
    public DateOnly EndDate { get; set; }
}

[Table("panel_spec_overrides")]
public class PanelSpecOverride
{
    [Key]
    [Column("override_id")]
    public int OverrideId { get; set; }

    [Column("panel_id")]
    public int PanelId { get; set; }

    [Column("spec_field")]
    [Required]
    [MaxLength(100)]
    public string SpecField { get; set; } = string.Empty;

    [Column("original_value")]
    public string? OriginalValue { get; set; }

    [Column("override_value")]
    [Required]
    public string OverrideValue { get; set; } = string.Empty;

    [Column("modified_date")]
    public DateTime ModifiedDate { get; set; } = DateTime.Now;

    [Column("modified_by")]
    [Required]
    [MaxLength(50)]
    public string ModifiedBy { get; set; } = string.Empty;

    [Column("design_fcs_date")]
    [MaxLength(50)]
    public string? DesignFcsDate { get; set; }

    [Column("is_active")]
    public bool IsActive { get; set; } = true;
}
