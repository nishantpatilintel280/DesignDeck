using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;

namespace EP.Pages;

public class RolesModel : PageModel
{
    private readonly ApplicationDbContext _db;
    public RolesModel(ApplicationDbContext db)
    {
        _db = db;
    }

    public List<User> AllUsers { get; private set; } = new();
    public string CurrentIdsid { get; private set; } = string.Empty;
    public int CurrentRole { get; private set; } = 0; // from users.role
    public List<Design> OwnedDesigns { get; private set; } = new();
    public List<int> OwnedDesignIds { get; private set; } = new();
    public List<SecondaryOwner> MySecondaryOwners { get; private set; } = new();

    [BindProperty] public string? SelectedIdsid { get; set; }
    [BindProperty] public int? SelectedDesignId { get; set; }
    [BindProperty] public DateOnly? StartDate { get; set; }
    [BindProperty] public DateOnly? EndDate { get; set; }

    public void OnGet()
    {
        CurrentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);
        var me = _db.Users.FirstOrDefault(u => u.idsid == CurrentIdsid);
        CurrentRole = me?.role ?? 0;

        AllUsers = _db.Users.OrderBy(u => u.idsid).ToList();
        OwnedDesigns = _db.Designs
            .Where(d => _db.DesignOwners.Any(o => o.DesignId == d.DesignId && o.Idsid == CurrentIdsid))
            .OrderBy(d => d.DesignName)
            .ThenBy(d => d.Platform)
            .ThenBy(d => d.SKU)
            .ToList();
        OwnedDesignIds = OwnedDesigns.Select(d => d.DesignId).ToList();
        MySecondaryOwners = _db.SecondaryOwners.Where(s => s.Idsid == CurrentIdsid).OrderBy(s => s.DesignId).ToList();
    }

    public IActionResult OnPost()
    {
        CurrentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);
        var me = _db.Users.FirstOrDefault(u => u.idsid == CurrentIdsid);
        CurrentRole = me?.role ?? 0;

        AllUsers = _db.Users.OrderBy(u => u.idsid).ToList();
        OwnedDesigns = _db.Designs
            .Where(d => _db.DesignOwners.Any(o => o.DesignId == d.DesignId && o.Idsid == CurrentIdsid))
            .OrderBy(d => d.DesignName)
            .ThenBy(d => d.Platform)
            .ThenBy(d => d.SKU)
            .ToList();
        OwnedDesignIds = OwnedDesigns.Select(d => d.DesignId).ToList();
        MySecondaryOwners = _db.SecondaryOwners.Where(s => s.Idsid == CurrentIdsid).OrderBy(s => s.DesignId).ToList();

        var today = DateOnly.FromDateTime(DateTime.Today);
        if (string.IsNullOrWhiteSpace(SelectedIdsid)) ModelState.AddModelError(nameof(SelectedIdsid), "Select a user.");
        else if (SelectedIdsid == CurrentIdsid) ModelState.AddModelError(nameof(SelectedIdsid), "You cannot transfer to yourself.");
        if (!SelectedDesignId.HasValue || !OwnedDesigns.Any(d => d.DesignId == SelectedDesignId.Value)) ModelState.AddModelError(nameof(SelectedDesignId), "Select a valid design you own.");
        if (!StartDate.HasValue) ModelState.AddModelError(nameof(StartDate), "Start date is required.");
        else if (StartDate.Value < today) ModelState.AddModelError(nameof(StartDate), "Start date must be after today.");
        if (!EndDate.HasValue) ModelState.AddModelError(nameof(EndDate), "End date is required.");
        else if (StartDate.HasValue && EndDate.Value < StartDate.Value) ModelState.AddModelError(nameof(EndDate), "End date must be after start date.");

        if (!ModelState.IsValid)
        {
            TempData["WelcomeMessage"] = string.Join("; ", ModelState.Values.SelectMany(v => v.Errors).Select(e => e.ErrorMessage));
            return Page();
        }

        var did = SelectedDesignId.Value;
        var existing = _db.SecondaryOwners.FirstOrDefault(s => s.DesignId == did && s.Idsid == CurrentIdsid);
        if (existing == null)
        {
            var entry = new SecondaryOwner
            {
                DesignId = did,
                Idsid = CurrentIdsid,
                SecondaryIdsid = SelectedIdsid!,
                StartDate = StartDate!.Value,
                EndDate = EndDate!.Value
            };
            _db.SecondaryOwners.Add(entry);
        }
        else
        {
            existing.SecondaryIdsid = SelectedIdsid!;
            existing.StartDate = StartDate!.Value;
            existing.EndDate = EndDate!.Value;
            _db.SecondaryOwners.Update(existing);
        }

        try
        {
            var changed = _db.SaveChanges();
            TempData["WelcomeMessage"] = changed > 0
                ? $"Temporary role transfer saved for design {did} from {StartDate:yyyy-MM-dd} to {EndDate:yyyy-MM-dd} for {SelectedIdsid}."
                : "No changes were saved. Please retry.";
        }
        catch (DbUpdateException ex)
        {
            TempData["WelcomeMessage"] = $"Save failed: {ex.GetBaseException().Message}";
            return Page();
        }

        return RedirectToPage("/Roles");
    }

    public IActionResult OnPostDeleteSecondary(int designId)
    {
        CurrentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);
        var me = _db.Users.FirstOrDefault(u => u.idsid == CurrentIdsid);
        CurrentRole = me?.role ?? 0;

        var existing = _db.SecondaryOwners.FirstOrDefault(s => s.DesignId == designId && s.Idsid == CurrentIdsid);
        if (existing != null)
        {
            _db.SecondaryOwners.Remove(existing);
            _db.SaveChanges();
            TempData["WelcomeMessage"] = "Secondary assignment removed.";
        }
        else
        {
            TempData["WelcomeMessage"] = "Assignment not found.";
        }
        return RedirectToPage("/Roles");
    }

    private static string ExtractIdsid(string? raw)
    {
        if (string.IsNullOrEmpty(raw)) return string.Empty;
        int idx = raw.LastIndexOf('\\');
        return idx >= 0 ? raw[(idx + 1)..] : raw;
    }
}
