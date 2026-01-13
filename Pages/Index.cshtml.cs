using ClosedXML.Excel;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.Data.SqlClient;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Caching.Memory;
using Microsoft.Identity.Client;
using System.ComponentModel.DataAnnotations;
using System.DirectoryServices.AccountManagement;
using System.Globalization;
using System.Text.Json;
using EP.Services; // added

namespace EP.Pages;

public class IndexModel : PageModel
{
    private readonly ApplicationDbContext _db;
    private readonly IHttpClientFactory? _httpClientFactory; // optional DI
    private readonly IConfiguration _config;
    private readonly IMemoryCache _cache;

    private static readonly JsonSerializerOptions _jsonOpts = new(JsonSerializerDefaults.Web);

    public IndexModel(ApplicationDbContext db, IConfiguration config, IMemoryCache cache, IHttpClientFactory? httpClientFactory = null)
    {
        _db = db;
        _config = config;
        _httpClientFactory = httpClientFactory;
        _cache = cache;
    }

    public string? TemplateUrl { get; private set; }

    [BindProperty] public IFormFile? ExcelFile { get; set; }
    [BindProperty] public IFormFile? EdidFile { get; set; }
    [BindProperty(SupportsGet = true)] public string? Query { get; set; }
    [BindProperty(SupportsGet = true)] public string? AiQuery { get; set; } // AI search query (uses keyword filter)
    [BindProperty(SupportsGet = true)] public string? Platform { get; set; }
    [BindProperty(SupportsGet = true)] public string? SKU { get; set; }
    [BindProperty(SupportsGet = true)] public string? Scope { get; set; } // "MyDesigns" or "All"

    // Master filter bindings
    [BindProperty(SupportsGet = true)] public string? FilterOEM { get; set; }
    [BindProperty(SupportsGet = true)] public string? FilterFcsStatus { get; set; } // Passed / Upcoming / ALL
    [BindProperty(SupportsGet = true)] public string? FilterPanelFeature { get; set; } // legacy single feature
    [BindProperty(SupportsGet = true)] public string? FilterPanelFeatureSupported { get; set; } // legacy supported flag
    [BindProperty(SupportsGet = true)] public List<string>? FilterPanelFeatures { get; set; } // new: multiple features

    [BindProperty][Required] public string? NewPlatform { get; set; }
    [BindProperty][Required] public string? NewSKU { get; set; }
    [BindProperty][Required] public string? NewOEM { get; set; }
    [BindProperty][Required] public string? NewDesignName { get; set; }

    [BindProperty] public string? NewODM { get; set; }
    [BindProperty] public string? NewIf { get; set; }
    [BindProperty] public string? NewFCS { get; set; }
    [BindProperty] public int NewNumberOfPanels { get; set; } = 1;

    [BindProperty]
    public List<int> SelectedDesignIds { get; set; } = new();

    [BindProperty] public int? DesignId { get; set; }
    [BindProperty] public string? PanelVendor { get; set; }
    [BindProperty] public string? Phase { get; set; }

    public List<DesignEntry> Designs { get; } = new();
    public Dictionary<int, List<PanelEntry>> PanelsByDesign { get; set; } = new();
    public Dictionary<int, PanelSpec> PanelSpecsByPanelId { get; set; } = new();

    public int CurrentRole { get; private set; }
    private string? _currentIdsid;
    public string CurrentIdsid => _currentIdsid ?? string.Empty;
    public string CurrentUserName { get; private set; } = string.Empty;

    public HashSet<string> AllowedDesignNamesPtl { get; private set; } = new(StringComparer.OrdinalIgnoreCase);
    public HashSet<string> AllowedDesignNamesNvl { get; private set; } = new(StringComparer.OrdinalIgnoreCase);
    private Dictionary<string, int> _crttDesignIdsPtl = new(StringComparer.OrdinalIgnoreCase);
    private Dictionary<string, int> _crttDesignIdsNvl = new(StringComparer.OrdinalIgnoreCase);

    public HashSet<string> AllowedCustomers { get; private set; } = new(StringComparer.OrdinalIgnoreCase);

    public HashSet<int> MismatchDesignIds { get; private set; } = new();
    public HashSet<int> MismatchIfDesignIds { get; private set; } = new();
    public HashSet<int> MismatchFcsDesignIds { get; private set; } = new();
    public HashSet<int> MismatchDesignNameIds { get; private set; } = new(); // New: track design names not in CRTT
    public Dictionary<int, string> CrttIfByDesignId { get; } = new();
    public Dictionary<int, string> CrttFcsByDesignId { get; } = new();

    private string ExtractIdsid(string? raw)
    {
        if (string.IsNullOrEmpty(raw)) return string.Empty;
        int idx = raw.LastIndexOf('\\');
        return idx >= 0 ? raw[(idx + 1)..] : raw;
    }

    //domains
    static PrincipalContext context_gar = new PrincipalContext(ContextType.Domain, "gar.corp.intel.com");
    static PrincipalContext context_amr = new PrincipalContext(ContextType.Domain, "amr.corp.intel.com");
    static PrincipalContext context_ccr = new PrincipalContext(ContextType.Domain, "ccr.corp.intel.com");
    static PrincipalContext context_ger = new PrincipalContext(ContextType.Domain, "ger.corp.intel.com");

    GroupPrincipal context_gar1 = new GroupPrincipal(context_gar, "gar.corp.intel.com");
    GroupPrincipal context_amr1 = new GroupPrincipal(context_amr, "amr.corp.intel.com");
    GroupPrincipal context_ccr1 = new GroupPrincipal(context_ccr, "ccr.corp.intel.com");
    GroupPrincipal context_ger1 = new GroupPrincipal(context_ger, "ger.corp.intel.com");

    UserPrincipal? garPrincipal = null;
    UserPrincipal? amrPrincipal = null;
    UserPrincipal? ccrPrincipal = null;

    private string GetATokenForGraph()
    {
        var azureAppId = Environment.GetEnvironmentVariable("AZURE_APP_ID") ?? _config["Azure:AppId"];
        var azureTenantId = Environment.GetEnvironmentVariable("AZURE_TENANT_ID") ?? _config["Azure:TenantId"];
        var userEmail = Environment.GetEnvironmentVariable("AZURE_USER_EMAIL") ?? _config["Azure:UserEmail"];
        var userPassword = Environment.GetEnvironmentVariable("AZURE_USER_PASSWORD") ?? _config["Azure:UserPassword"];

        if (string.IsNullOrWhiteSpace(azureAppId) || string.IsNullOrWhiteSpace(azureTenantId) ||
            string.IsNullOrWhiteSpace(userEmail) || string.IsNullOrWhiteSpace(userPassword))
        {
            throw new InvalidOperationException("Missing Azure credentials. Configure environment variables or appsettings (Azure:AppId, TenantId, UserEmail, UserPassword). Avoid ROPC in production.");
        }

        IPublicClientApplication application = PublicClientApplicationBuilder.Create(azureAppId)
            .WithTenantId(azureTenantId)
            .Build();

        string[] scopes = new string[] { $"{azureAppId}/.default" };
        var res = application.AcquireTokenByUsernamePassword(scopes, userEmail, userPassword);
        var result = res.ExecuteAsync().Result;
        return result.AccessToken;
    }

    private async Task<JsonDocument?> FetchCrttAsync(string url, string bearerToken, int maxRetries = 5, int initialDelayMs = 2)
    {
        // Reuse shared CrttApi for local dev and page calls
        var api = new CrttApi(_httpClientFactory);
        using var cts = new CancellationTokenSource(TimeSpan.FromSeconds(30));
        return await api.FetchAsync(url, bearerToken, cts.Token, maxRetries, Math.Max(initialDelayMs, 2));
    }

    private static void AddDesignNamesFromCrtt(JsonDocument? doc, HashSet<string> namesTarget, Dictionary<string, int> idMapTarget)
    {
        CrttApi.AddDesignNamesFromCrtt(doc, namesTarget, idMapTarget);
    }

    private static void AddCustomersFromCrtt(JsonDocument? doc, HashSet<string> customersTarget)
    {
        CrttApi.AddCustomersFromCrtt(doc, customersTarget);
    }

    private const string CacheKeyPtl = "CRTT_AllowedDesignNames_PTL_177_178_181";
    private const string CacheKeyNvl = "CRTT_AllowedDesignNames_NVL_194_205";
    private const string CacheKeyPtlIds = "CRTT_DesignIds_PTL_177_178_181";
    private const string CacheKeyNvlIds = "CRTT_DesignIds_NVL_194_205";
    private const string CacheKeyCustomers = "CRTT_Customers_List";

    private async Task<(string? IF, string? FCS)> GetCrttScheduleAsync(int designId, string bearerToken)
    {
        string cacheKey = $"CRTT_SCHEDULE_{designId}";
        if (_cache.TryGetValue<(string? IF, string? FCS)>(cacheKey, out var cached) && (cached.IF != null || cached.FCS != null))
            return cached;

        var url = $"https://crtt-api.intel.com/designSchedule/?dID={designId}";
        var doc = await FetchCrttAsync(url, bearerToken);
        string? ifDate = null, fcsDate = null;
        if (doc != null && doc.RootElement.ValueKind == JsonValueKind.Array)
        {
            foreach (var item in doc.RootElement.EnumerateArray())
            {
                if (item.TryGetProperty("Milestone", out var m) && item.TryGetProperty("actual_dt", out var a))
                {
                    var ms = m.GetString();
                    var dt = a.GetString();
                    if (string.Equals(ms, "Software Image Freeze (IF)", StringComparison.OrdinalIgnoreCase)) ifDate = dt;
                    if (string.Equals(ms, "First Customer Shipment (FCS)", StringComparison.OrdinalIgnoreCase)) fcsDate = dt;
                }
            }
        }
        var result = (ifDate, fcsDate);
        _cache.Set(cacheKey, result, new MemoryCacheEntryOptions { AbsoluteExpirationRelativeToNow = TimeSpan.FromHours(12) });
        return result;
    }

    public async Task OnGet()
    {
        if (_currentIdsid == null) _currentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);

        // Try resolving user in all four domains
        UserPrincipal? gerPrincipal = null;
        try { garPrincipal = UserPrincipal.FindByIdentity(context_gar, IdentityType.SamAccountName, CurrentIdsid); } catch { }
        try { amrPrincipal = UserPrincipal.FindByIdentity(context_amr, IdentityType.SamAccountName, CurrentIdsid); } catch { }
        try { ccrPrincipal = UserPrincipal.FindByIdentity(context_ccr, IdentityType.SamAccountName, CurrentIdsid); } catch { }
        try { gerPrincipal = UserPrincipal.FindByIdentity(context_ger, IdentityType.SamAccountName, CurrentIdsid); } catch { }

        // Allowed if present in any domain or member of any corresponding domain group
        bool isAllowed = false;
        try { isAllowed |= garPrincipal != null || (garPrincipal != null && context_gar1 != null && garPrincipal.IsMemberOf(context_gar1)); } catch { }
        try { isAllowed |= amrPrincipal != null || (amrPrincipal != null && context_amr1 != null && amrPrincipal.IsMemberOf(context_amr1)); } catch { }
        try { isAllowed |= ccrPrincipal != null || (ccrPrincipal != null && context_ccr1 != null && ccrPrincipal.IsMemberOf(context_ccr1)); } catch { }
        try { isAllowed |= gerPrincipal != null || (gerPrincipal != null && context_ger1 != null && gerPrincipal.IsMemberOf(context_ger1)); } catch { }

        if (!isAllowed)
        {
            Console.WriteLine("User not found in any allowed domains: " + CurrentIdsid);
            Response.Redirect(Url.Page("/Error") ?? "/Error");
            return;
        }

        // Prefer GAR display name, else fallback to any found principal or idsid
        CurrentUserName = garPrincipal?.DisplayName
            ?? amrPrincipal?.DisplayName
            ?? ccrPrincipal?.DisplayName
            ?? gerPrincipal?.DisplayName
            ?? CurrentIdsid;

        Console.WriteLine("Current user: " + CurrentIdsid);
        Console.WriteLine("Current user name: " + CurrentUserName);

        Console.WriteLine("=== Authentication Debug Info ===");
        Console.WriteLine($"Is Authenticated: {HttpContext.User?.Identity?.IsAuthenticated}");
        Console.WriteLine($"Authentication Type: {HttpContext.User?.Identity?.AuthenticationType}");
        Console.WriteLine($"User Name: '{HttpContext.User?.Identity?.Name}'");
        Console.WriteLine("=== End Debug Info ===");

        // Fetch once and cache per platform; also store name->CRTT DesignID maps
        if (!_cache.TryGetValue<HashSet<string>>(CacheKeyPtl, out var cachedPtl) || cachedPtl == null || cachedPtl.Count == 0
            || !_cache.TryGetValue<Dictionary<string, int>>(CacheKeyPtlIds, out var cachedPtlIds) || cachedPtlIds == null || cachedPtlIds.Count == 0)
        {
            var token = GetATokenForGraph();
            foreach (var pid in new[] { 177, 178, 181 })
            {
                var url = $"https://crtt-api.intel.com/designNames/?pID={pid}";
                var doc = await FetchCrttAsync(url, token);
                AddDesignNamesFromCrtt(doc, AllowedDesignNamesPtl, _crttDesignIdsPtl);
            }
            _cache.Set(CacheKeyPtl, AllowedDesignNamesPtl, new MemoryCacheEntryOptions { AbsoluteExpirationRelativeToNow = TimeSpan.FromHours(12) });
            _cache.Set(CacheKeyPtlIds, _crttDesignIdsPtl, new MemoryCacheEntryOptions { AbsoluteExpirationRelativeToNow = TimeSpan.FromHours(12) });
        }
        else
        {
            AllowedDesignNamesPtl = cachedPtl;
            _crttDesignIdsPtl = cachedPtlIds;
        }

        if (!_cache.TryGetValue<HashSet<string>>(CacheKeyCustomers, out var cachedCustomers) || cachedCustomers == null || cachedCustomers.Count == 0)
        {
            var token = GetATokenForGraph();
            var customerDoc = await FetchCrttAsync("https://crtt-api.intel.com/customers", token);
            AllowedCustomers = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            AddCustomersFromCrtt(customerDoc, AllowedCustomers);
            _cache.Set(CacheKeyCustomers, AllowedCustomers, new MemoryCacheEntryOptions { AbsoluteExpirationRelativeToNow = TimeSpan.FromHours(12) });
        }
        else
        {
            AllowedCustomers = cachedCustomers;
        }

        if (!_cache.TryGetValue<HashSet<string>>(CacheKeyNvl, out var cachedNvl) || cachedNvl == null || cachedNvl.Count == 0
            || !_cache.TryGetValue<Dictionary<string, int>>(CacheKeyNvlIds, out var cachedNvlIds) || cachedNvlIds == null || cachedNvlIds.Count == 0)
        {
            var token = GetATokenForGraph();
            foreach (var pid in new[] { 194, 195, 197, 198, 199, 200, 201, 202, 203, 204, 205 })
            {
                var url = $"https://crtt-api.intel.com/designNames/?pID={pid}";
                var doc = await FetchCrttAsync(url, token);
                AddDesignNamesFromCrtt(doc, AllowedDesignNamesNvl, _crttDesignIdsNvl);
            }
            _cache.Set(CacheKeyNvl, AllowedDesignNamesNvl, new MemoryCacheEntryOptions { AbsoluteExpirationRelativeToNow = TimeSpan.FromHours(12) });
            _cache.Set(CacheKeyNvlIds, _crttDesignIdsNvl, new MemoryCacheEntryOptions { AbsoluteExpirationRelativeToNow = TimeSpan.FromHours(12) });
        }
        else
        {
            AllowedDesignNamesNvl = cachedNvl;
            _crttDesignIdsNvl = cachedNvlIds;
        }

        // Read SharePoint template link once for rendering a direct anchor
        TemplateUrl = _config["SharePoint:TemplateUrl"];




        // Ensure user exists in users table with role=3 (idsid is varchar now)
        int currentRole = 3;
        if (!string.IsNullOrWhiteSpace(CurrentIdsid))
        {
            var existingUser = _db.Users.FirstOrDefault(u => u.idsid == CurrentIdsid);
            if (existingUser == null)
            {
                existingUser = new User { idsid = CurrentIdsid, role = 3 }; // default role 3
                _db.Users.Add(existingUser);
                _db.SaveChanges();
                Console.WriteLine($"Inserted user idsid={CurrentIdsid} role=3");
            }
            currentRole = existingUser.role;
        }
        CurrentRole = currentRole; // expose to Razor view

        // Defaults (Scope)
        if (string.IsNullOrWhiteSpace(Scope))
        {
            switch (currentRole)
            {
                case 1:
                    Scope = "All";
                    break;
                case 2:
                    Scope = "MyDesigns";
                    break;
                case 3:
                    Scope = "All";
                    break;
                case 4:
                    Scope = "All";
                    break;
                default:
                    Scope = "MyDesigns";
                    break;
            }
        }
        if (string.IsNullOrWhiteSpace(Platform)) Platform = "PTL";
        if (string.IsNullOrWhiteSpace(SKU)) SKU = "ALL";

        // Build SQL WHERE clause for designs list
        var whereClauses = new List<string>();
        var parameters = new List<object>();

        if (Scope != "All")
        {
            // Show designs where current user is either a primary owner or a secondary owner
            whereClauses.Add("(d.design_id IN (SELECT design_id FROM design_owners WHERE Idsid = @idsid) OR d.design_id IN (SELECT design_id FROM secondary_owners WHERE secondary_idsid = @idsid))");
            parameters.Add(new SqlParameter("@idsid", CurrentIdsid));
        }
        if (!string.IsNullOrWhiteSpace(Platform) && Platform != "ALL")
        {
            whereClauses.Add("d.platform = @platform");
            parameters.Add(new SqlParameter("@platform", Platform));
        }
        if (!string.IsNullOrWhiteSpace(SKU) && SKU != "ALL")
        {
            whereClauses.Add("d.sku = @sku");
            parameters.Add(new SqlParameter("@sku", SKU))
;
        }
        if (!string.IsNullOrWhiteSpace(Query))
        {
            // Show only the exact searched design name
            whereClauses.Add("d.design_name = @query");
            parameters.Add(new SqlParameter("@query", Query));
        }
        if (!string.IsNullOrWhiteSpace(FilterOEM))
        {
            whereClauses.Add("d.oem = @oem");
            parameters.Add(new SqlParameter("@oem", FilterOEM));
        }

        var whereSql = whereClauses.Count > 0 ? "WHERE " + string.Join(" AND ", whereClauses) : "";
        var sql = $@"
            SELECT d.*
            FROM Designs d
            {whereSql}
            ORDER BY d.design_id";

        // Load designs (not tracked for initial display)
        var designRows = _db.Designs
            .FromSqlRaw(sql, parameters.ToArray())
            .AsNoTracking()
            .ToList();

        // Apply FCS status filter in-memory since date is stored as string in different formats
        if (!string.IsNullOrWhiteSpace(FilterFcsStatus) && !string.Equals(FilterFcsStatus, "ALL", StringComparison.OrdinalIgnoreCase))
        {
            bool keepPassed = string.Equals(FilterFcsStatus, "Passed", StringComparison.OrdinalIgnoreCase);
            bool keepUpcoming = string.Equals(FilterFcsStatus, "Upcoming", StringComparison.OrdinalIgnoreCase);
            bool IsPassed(string? fcs)
            {
                if (string.IsNullOrWhiteSpace(fcs)) return false;
                var clean = fcs.Trim();
                var spaceIdx = clean.IndexOf(' ');
                if (spaceIdx > 0) clean = clean[..spaceIdx];
                if (DateTime.TryParseExact(clean,
                    new[] { "dd-MM-yyyy", "MM-dd-yyyy", "yyyy-MM-dd" },
                    CultureInfo.InvariantCulture,
                    DateTimeStyles.None,
                    out var dt))
                {
                    return dt < DateTime.Today;
                }
                return false;
            }
            designRows = designRows.Where(d => (keepPassed && IsPassed(d.FCS)) || (keepUpcoming && !IsPassed(d.FCS))).ToList();
        }

        var designIds = new List<int>();
        foreach (var d in designRows)
        {
            designIds.Add(d.DesignId);
        }

        // Get panel counts per design in one grouped query
        var panelCounts = _db.Panels
            .Where(p => designIds.Contains(p.design_id))
            .GroupBy(p => p.design_id)
            .Select(g => new { DesignId = g.Key, Count = g.Count() })
            .ToDictionary(x => x.DesignId, x => x.Count);

        // Prepare bearer token for CRTT schedule calls, used only if needed
        var bearer = GetATokenForGraph();

        foreach (var d in designRows)
        {
            Designs.Add(new DesignEntry
            {
                Id = d.DesignId,
                Name = d.DesignName,
                Platform = d.Platform,
                OEM = d.OEM,
                SKU = d.SKU,
                ODM = d.ODM,
                IF_ = d.If_,
                FCS = d.FCS,
                NumberOfPanels = panelCounts.TryGetValue(d.DesignId, out var c) ? c : 0,
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
            });

            // Check if design name exists in CRTT for this platform
            bool designNameInCrtt = false;
            if (string.Equals(d.Platform, "PTL", StringComparison.OrdinalIgnoreCase))
            {
                designNameInCrtt = AllowedDesignNamesPtl.Contains(d.DesignName);
            }
            else if (string.Equals(d.Platform, "NVL", StringComparison.OrdinalIgnoreCase))
            {
                designNameInCrtt = AllowedDesignNamesNvl.Contains(d.DesignName);
            }

            // Mark design if name not found in CRTT
            if (!designNameInCrtt)
            {
                MismatchDesignNameIds.Add(d.DesignId);
                Console.WriteLine($"Design {d.DesignId} '{d.DesignName}' not found in CRTT for platform {d.Platform}");
            }

            // Check IF/FCS mismatch with CRTT schedule for the same design name within the platform
            int crttId = 0;
            if (string.Equals(d.Platform, "PTL", StringComparison.OrdinalIgnoreCase))
            {
                _crttDesignIdsPtl.TryGetValue(d.DesignName, out crttId);
            }
            else if (string.Equals(d.Platform, "NVL", StringComparison.OrdinalIgnoreCase))
            {
                _crttDesignIdsNvl.TryGetValue(d.DesignName, out crttId);
            }
            if (crttId > 0)
            {
                var (ifDate, fcsDate) = await GetCrttScheduleAsync(crttId, bearer);
                string Clean(string? s)
                {
                    if (string.IsNullOrWhiteSpace(s)) return string.Empty;
                    var t = s.Trim();
                    var tParts = t.Split('T', 2);
                    if (tParts.Length > 1 && !string.IsNullOrWhiteSpace(tParts[0]))
                    {
                        t = tParts[0];
                      }
                    var spaceIdx = t.IndexOf(' ');
                    if (spaceIdx > 0)
                    {
                        t = t[..spaceIdx];
                    }
                    if (DateTime.TryParseExact(t,
                            new[] { "yyyy-MM-dd", "MM-dd-yyyy", "dd-MM-yyyy" },
                            CultureInfo.InvariantCulture,
                            DateTimeStyles.None,
                            out var parsed))
                    {
                        return parsed.ToString("dd-MM-yyyy");
                    }
                    return t;
                }
                var dbIf = d.If_?.Trim() ?? string.Empty;
                var dbFcs = d.FCS?.Trim() ?? string.Empty;
                var apiIf = Clean(ifDate);
                var apiFcs = Clean(fcsDate);

                if (!string.IsNullOrEmpty(apiIf)) CrttIfByDesignId[d.DesignId] = apiIf;
                if (!string.IsNullOrEmpty(apiFcs)) CrttFcsByDesignId[d.DesignId] = apiFcs;

                bool ifMismatch = !string.IsNullOrEmpty(apiIf) && !string.Equals(dbIf, apiIf, StringComparison.OrdinalIgnoreCase);
                bool fcsMismatch = !string.IsNullOrEmpty(apiFcs) && !string.Equals(dbFcs, apiFcs, StringComparison.OrdinalIgnoreCase);

                if (ifMismatch) MismatchIfDesignIds.Add(d.DesignId);
                if (fcsMismatch) MismatchFcsDesignIds.Add(d.DesignId);

                if (ifMismatch || fcsMismatch)
                {
                    // keep legacy combined set for any mismatch (IF or FCS)
                    MismatchDesignIds.Add(d.DesignId);
                }
            }
        }

        Console.WriteLine("---------------------------------" + MismatchDesignIds.Count());
        // Persist updated panel counts back to Designs table if they differ
        if (designIds.Count > 0)
        {
            var trackedDesigns = _db.Designs.Where(x => designIds.Contains(x.DesignId)).ToList();
            bool anyModified = false;
            foreach (var td in trackedDesigns)
            {
                var actual = panelCounts.TryGetValue(td.DesignId, out var c) ? c : 0;
                if (td.NumberOfPanels != actual)
                {
                    td.NumberOfPanels = actual;
                    anyModified = true;
                }
            }
            if (anyModified)
            {
                _db.SaveChanges();
            }
        }

        // Load panel details for expansion UI
        var panels = _db.Panels
            .Where(p => designIds.Contains(p.design_id))
            .ToList();

        foreach (var id in designIds)
        {
            PanelsByDesign[id] = panels.Where(p => p.design_id == id)
                                       .Select(p => new PanelEntry
                                       {
                                           panel_id = p.panel_id,
                                           design_id = p.design_id,
                                           panel_vendor = p.panel_vendor,
                                           tcon_vendor = p.tcon_vendor
                                       }).ToList();
        }

        // Load specs for these panels to enable part number display
        var allPanelIds = panels.Select(p => p.panel_id).Distinct().ToList();
        if (allPanelIds.Count > 0)
        {
            PanelSpecsByPanelId = _db.PanelSpecs
                .Where(s => allPanelIds.Contains(s.panel_id))
                .AsNoTracking()
                .ToDictionary(s => s.panel_id, s => s);
        }

        // Apply Panel Feature filter if provided: multiple features truthy
        if (FilterPanelFeatures != null && FilterPanelFeatures.Count > 0)
        {
            var panelIds = PanelsByDesign.Values.SelectMany(l => l).Select(p => p.panel_id).Distinct().ToList();
            var specs = _db.PanelSpecs.Where(s => panelIds.Contains(s.panel_id)).AsNoTracking().ToDictionary(s => s.panel_id, s => s);

            bool IsTruthy(string? v)
            {
                var s = (v ?? string.Empty).Trim().ToLowerInvariant();
                return s.Contains("yes") || s.Contains("true") || s.Contains("supported");
            }

            var features = new HashSet<string>(FilterPanelFeatures.Select(f => f.Trim()), StringComparer.OrdinalIgnoreCase);

            var designIdsToKeep = new HashSet<int>();
            foreach (var kv in PanelsByDesign)
            {
                var did = kv.Key;
                bool anyMatch = kv.Value.Any(p =>
                {
                    if (!specs.TryGetValue(p.panel_id, out var s) || s == null) return false;
                    foreach (var f in features)
                    {
                        if (IsTruthy(GetSpecValue(s, f))) return true;
                    }
                    return false;
                });
                if (anyMatch) designIdsToKeep.Add(did);
            }

            Designs.RemoveAll(d => !designIdsToKeep.Contains(d.Id));
            foreach (var did in PanelsByDesign.Keys.ToList())
            {
                if (!designIdsToKeep.Contains(did)) PanelsByDesign.Remove(did);
            }
        }
        else if (!string.IsNullOrWhiteSpace(FilterPanelFeature) && !string.IsNullOrWhiteSpace(FilterPanelFeatureSupported))
        {
            // legacy single-feature path retained for backward compatibility
            var panelIds = PanelsByDesign.Values.SelectMany(l => l).Select(p => p.panel_id).Distinct().ToList();
            var specs = _db.PanelSpecs.Where(s => panelIds.Contains(s.panel_id)).AsNoTracking().ToDictionary(s => s.panel_id, s => s);

            bool IsTruthy(string? v)
            {
                var s = (v ?? string.Empty).Trim().ToLowerInvariant();
                return s.Contains("yes") || s.Contains("true") || s.Contains("supported");
            }
            var designIdsToKeep = new HashSet<int>();
            foreach (var kv in PanelsByDesign)
            {
                var did = kv.Key;
                bool anyMatch = kv.Value.Any(p => specs.TryGetValue(p.panel_id, out var s) && s != null &&
                    (
                        string.Equals(FilterPanelFeatureSupported, "yes", StringComparison.OrdinalIgnoreCase)
                            ? IsTruthy(GetSpecValue(s, FilterPanelFeature!))
                            : !IsTruthy(GetSpecValue(s, FilterPanelFeature!))
                    ));
                if (anyMatch) designIdsToKeep.Add(did);
            }

            Designs.RemoveAll(d => !designIdsToKeep.Contains(d.Id));
            foreach (var did in PanelsByDesign.Keys.ToList())
            {
                if (!designIdsToKeep.Contains(did)) PanelsByDesign.Remove(did);
            }
        }

        // After PanelsByDesign is filled
        // Apply AI query filtering via GNAI only (no rule-based fallback)
        if (!string.IsNullOrWhiteSpace(AiQuery))
        {
            var panelIds = PanelsByDesign.Values.SelectMany(l => l).Select(p => p.panel_id).Distinct().ToList();
            var specs = _db.PanelSpecs.Where(s => panelIds.Contains(s.panel_id)).AsNoTracking().ToDictionary(s => s.panel_id, s => s);

            var filterTree = await TryExtractStructuredFiltersWithGnaiAsync(AiQuery!);
            if (filterTree == null)
            {
                // If model fails to produce filters, keep all (no filtering)
            }
            else
            {
                bool EvalCondition(PanelSpec s, string field, string op, string? value)
                {
                    string norm(string? v) => (v ?? string.Empty).Trim().ToLowerInvariant();
                    var propVal = norm(field switch
                    {
                        "PSR1_Supported" => s.PSR1_Supported,
                        "PSR2_Supported" => s.PSR2_Supported,
                        "PSR2_ET_Supported" => s.PSR2_ET_Supported,
                        "HDR_Supported" => s.HDR_Supported,
                        "VRR_Supported" => s.VRR_Supported,
                        "VDSC_Version" => s.VDSC_Version,
                        "IIDT_UBRR" => s.IIDT_UBRR,
                        "Seamless_Drrs" => s.Seamless_Drrs,
                        "xPST_Supported" => s.xPST_Supported,
                        "xPST_Level" => s.xPST_Level,
                        "OPST_ELP" => s.OPST_ELP,
                        "DE_Early_Wake" => s.DE_Early_Wake,
                        "EPSM60" => s.EPSM60,
                        "EG_Endurance_Gaming" => s.EG_Endurance_Gaming,
                        "EDP1_5_Panel_Replay" => s.EDP1_5_Panel_Replay,
                        "EDP1_5_PR_ET" => s.EDP1_5_PR_ET,
                        "EDP1_5_CMRR" => s.EDP1_5_CMRR,
                        "EDP1_5_Periodic_ASSDP" => s.EDP1_5_Periodic_ASSDP,
                        "EDP1_5_LOBF" => s.EDP1_5_LOBF,
                        "EdidVersion" => s.EdidVersion,
                        "Resolution" => s.Resolution,
                        "RR_Min_Hz" => s.RR_Min_Hz,
                        "RR_Max_Hz" => s.RR_Max_Hz,
                        "Aspect_Ratio" => s.Aspect_Ratio,
                        "Color_Depth" => s.Color_Depth,
                        "Data_Lane_Count" => s.Data_Lane_Count,
                        "Data_Link_Rate" => s.Data_Link_Rate,
                        "Color_Gamut_sRGB" => s.Color_Gamut_sRGB,
                        "Max_Brightness_Nits" => s.Max_Brightness_Nits,
                        _ => null
                    });
                    if (propVal == null) return false;
                    var val = norm(value);
                    switch (op.ToLowerInvariant())
                    {
                        case "exists":
                            return !string.IsNullOrEmpty(propVal);
                        case "equals":
                            return propVal == val;
                        case "contains":
                            return propVal.Contains(val);
                        case "not_contains":
                            return !propVal.Contains(val);
                        case ">":
                        case ">=":
                        case "<":
                        case "<=":
                            // numeric compare when possible
                            if (double.TryParse(propVal, out var pv) && double.TryParse(val, out var vv))
                            {
                                return op switch
                                {
                                    ">" => pv > vv,
                                    ">=" => pv >= vv,
                                    "<" => pv < vv,
                                    "<=" => pv <= vv,
                                    _ => false
                                };
                            }
                            return false;
                        case "truthy":
                            // Treat yes/true/supported as truthy
                            return propVal.Contains("yes") || propVal.Contains("true") || propVal.Contains("supported");
                        default:
                            return false;
                    }
                }

                bool EvalNode(PanelSpec s, FilterNode node)
                {
                    if (node.conditions != null && node.conditions.Count > 0)
                    {
                        bool acc = node.logic?.Equals("AND", StringComparison.OrdinalIgnoreCase) ?? true;
                        foreach (var c in node.conditions)
                        {
                            var res = EvalCondition(s, c.field, c.op, c.value);
                            if (node.logic?.Equals("AND", StringComparison.OrdinalIgnoreCase) ?? true)
                            {
                                if (!res) return false;
                            }
                            else
                            {
                                if (res) return true;
                            }
                        }
                        return acc; // for AND, if we didn't return false, it's true; for OR, if none true, false
                    }
                    if (node.children != null && node.children.Count > 0)
                    {
                        var isAnd = node.logic?.Equals("AND", StringComparison.OrdinalIgnoreCase) ?? true;
                        if (isAnd)
                        {
                            foreach (var child in node.children)
                            {
                                if (!EvalNode(s, child)) return false;
                            }
                            return true;
                        }
                        else
                        {
                            foreach (var child in node.children)
                            {
                                if (EvalNode(s, child)) return true;
                            }
                            return false;
                        }
                    }
                    return true;
                }

                var designIdsToKeep = new HashSet<int>();
                foreach (var kv in PanelsByDesign)
                {
                    var did = kv.Key;
                    var anyMatch = kv.Value.Any(p => specs.TryGetValue(p.panel_id, out var s) && s != null && EvalNode(s, filterTree));
                    if (anyMatch) designIdsToKeep.Add(did);
                }

                Designs.RemoveAll(d => !designIdsToKeep.Contains(d.Id));
                foreach (var did in PanelsByDesign.Keys.ToList())
                {
                    if (!designIdsToKeep.Contains(did)) PanelsByDesign.Remove(did);
                }
            }
        }
    }

    private static string? GetSpecValue(PanelSpec s, string field)
    {
        switch (field)
        {
            case "PSR1_Supported": return s.PSR1_Supported;
            case "PSR2_Supported": return s.PSR2_Supported;
            case "PSR2_ET_Supported": return s.PSR2_ET_Supported;
            case "HDR_Supported": return s.HDR_Supported;
            case "VRR_Supported": return s.VRR_Supported;
            case "VDSC_Version": return s.VDSC_Version;
            case "Seamless_Drrs": return s.Seamless_Drrs;
            case "xPST_Supported": return s.xPST_Supported;
            case "OPST_ELP": return s.OPST_ELP;
            case "DE_Early_Wake": return s.DE_Early_Wake;
            case "EPSM60": return s.EPSM60;
            case "EG_Endurance_Gaming": return s.EG_Endurance_Gaming;
            default: return null;
        }
    }

    // Change to GET handler and redirect browser to SharePoint URL so user authentication can complete (avoids server-side 403)
    public IActionResult OnGetDownloadTemplate()
    {
        var fileUrl = _config["SharePoint:TemplateUrl"];
        if (string.IsNullOrWhiteSpace(fileUrl))
        {
            return BadRequest("SharePoint template URL not configured.");
    }
        // Log for debugging
        Console.WriteLine($"Redirecting to SharePoint template: {fileUrl}");
        return Redirect(fileUrl);
    }

    // New: Export currently listed details to Excel
    public IActionResult OnGetDownloadDetails()
    {
        if (_currentIdsid == null) _currentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);
        
        // Reuse the same filtering logic as OnGet
        var whereClauses = new List<string>();
        var parameters = new List<object>();
        if (Scope != "All")
        {
            whereClauses.Add("(d.design_id IN (SELECT design_id FROM design_owners WHERE Idsid = @idsid) OR d.design_id IN (SELECT design_id FROM secondary_owners WHERE secondary_idsid = @idsid))");
            parameters.Add(new SqlParameter("@idsid", CurrentIdsid));
        }
        if (!string.IsNullOrWhiteSpace(Platform) && Platform != "ALL")
        {
            whereClauses.Add("d.platform = @platform");
            parameters.Add(new SqlParameter("@platform", Platform));
        }
        if (!string.IsNullOrWhiteSpace(SKU) && SKU != "ALL")
        {
            whereClauses.Add("d.sku = @sku");
            parameters.Add(new SqlParameter("@sku", SKU));
        }
        if (!string.IsNullOrWhiteSpace(Query))
        {
            whereClauses.Add("d.design_name = @query");
            parameters.Add(new SqlParameter("@query", Query));
        }
        if (!string.IsNullOrWhiteSpace(FilterOEM))
        {
            whereClauses.Add("d.oem = @oem");
            parameters.Add(new SqlParameter("@oem", FilterOEM));
        }
        var whereSql = whereClauses.Count > 0 ? "WHERE " + string.Join(" AND ", whereClauses) : "";
        var sql = $@"
            SELECT d.*
            FROM Designs d
            {whereSql}
            ORDER BY d.design_id";
        var designRows = _db.Designs
            .FromSqlRaw(sql, parameters.ToArray())
            .AsNoTracking()
            .ToList();
        var designIds = designRows.Select(d => d.DesignId).ToList();
        
        // Load panels and specs for these designs
        var panels = _db.Panels
            .Where(p => designIds.Contains(p.design_id))
            .AsNoTracking()
            .ToList();
        var panelIds = panels.Select(p => p.panel_id).ToList();
        var specsByPanelId = _db.PanelSpecs
            .Where(s => panelIds.Contains(s.panel_id))
            .AsNoTracking()
            .ToDictionary(s => s.panel_id, s => s);

        // Apply Panel Feature filter for export (multiple features truthy)
        if (FilterPanelFeatures != null && FilterPanelFeatures.Count > 0)
        {
            bool IsTruthy(string? v)
            {
                var s = (v ?? string.Empty).Trim().ToLowerInvariant();
                return s.Contains("yes") || s.Contains("true") || s.Contains("supported");
            }
            var features = new HashSet<string>(FilterPanelFeatures.Select(f => f.Trim()), StringComparer.OrdinalIgnoreCase);
            var designIdsToKeep = new HashSet<int>();
            foreach (var did in designIds)
            {
                var anyMatch = panels.Where(p => p.design_id == did).Any(p =>
                {
                    if (!specsByPanelId.TryGetValue(p.panel_id, out var s) || s == null) return false;
                    foreach (var f in features)
                    {
                        if (IsTruthy(GetSpecValue(s, f))) return true;
                    }
                    return false;
                });
                if (anyMatch) designIdsToKeep.Add(did);
            }
            designRows = designRows.Where(d => designIdsToKeep.Contains(d.DesignId)).ToList();
            panels = panels.Where(p => designIdsToKeep.Contains(p.design_id)).ToList();
            panelIds = panels.Select(p => p.panel_id).ToList();
            specsByPanelId = specsByPanelId.Where(kv => panelIds.Contains(kv.Key)).ToDictionary(kv => kv.Key, kv => kv.Value);
        }
        else if (!string.IsNullOrWhiteSpace(FilterPanelFeature) && !string.IsNullOrWhiteSpace(FilterPanelFeatureSupported))
        {
            // legacy single-feature path
            bool IsTruthy(string? v)
            {
                var s = (v ?? string.Empty).Trim().ToLowerInvariant();
                return s.Contains("yes") || s.Contains("true") || s.Contains("supported");
            }
            var designIdsToKeep = new HashSet<int>();
            foreach (var did in designIds)
            {
                var anyMatch = panels.Where(p => p.design_id == did).Any(p => specsByPanelId.TryGetValue(p.panel_id, out var s) && s != null &&
                    (
                        string.Equals(FilterPanelFeatureSupported, "yes", StringComparison.OrdinalIgnoreCase)
                            ? IsTruthy(GetSpecValue(s, FilterPanelFeature!))
                            : !IsTruthy(GetSpecValue(s, FilterPanelFeature!))
                    ));
                if (anyMatch) designIdsToKeep.Add(did);
            }
            designRows = designRows.Where(d => designIdsToKeep.Contains(d.DesignId)).ToList();
            panels = panels.Where(p => designIdsToKeep.Contains(p.design_id)).ToList();
            panelIds = panels.Select(p => p.panel_id).ToList();
            specsByPanelId = specsByPanelId.Where(kv => panelIds.Contains(kv.Key)).ToDictionary(kv => kv.Key, kv => kv.Value);
        }

        using var wb = new XLWorkbook();
        var ws = wb.Worksheets.Add("DesignsAndPanels");

        // Build headers: design columns then panel columns (including spec fields)
        var headers = new List<string>
        {
            // Basic design info
            "DesignId","Platform","SKU","ODM","OEM","DesignName","IF","FCS","NumberOfPanels",
            // Hardware configuration
            "Segment","Config","PlValues","MemoryCapacity","FormFactor","Soc","Pch","Uma","HybridGfx","DgpuVendor",
            // Timeline
            "EvtStart","EvtEnd","FvtStart","FvtEnd","SitStart","SitEnd",
            // Features & logistics
            "AthenaEvoCandidate","Gaming","AiFeatures","PO","POSite","PvNeed","ShipOutOs","HwAvailability","PostConfig","Notes",
            // Panel info
            "PanelId","PanelVendor","TconVendor",
            // Spec columns
            "PanelPortNumber","EdidVersion","Resolution","RR_Min_Hz","RR_Max_Hz","Aspect_Ratio","Htotal","Vtotal","Screen_Size_Inch","Color_Depth","FRC","Data_Lane_Count","Data_Link_Rate",
            "PSR1_Supported","PSR2_Supported","PSR2_ET_Supported","Intel_LRR_Version","HDR_Supported","VRR_Supported","VDSC_Version","CoG_MSO","IIDT_UBRR","Camera_Sensor_For_UBRR","Seamless_Drrs","xPST_Supported","xPST_Level","OPST_ELP","DE_Early_Wake","EPSM60","EG_Endurance_Gaming",
            "EDP1_5_Panel_Replay","EDP1_5_PR_ET","EDP1_5_CMRR","EDP1_5_Periodic_ASSDP","EDP1_5_LOBF","Other_Features",
            "Color_Gamut (sRGB %, DCI-P3 %,Adobe RGB %,NTSC %)", "Max_Brightness_Nits","Backlight_Light_Source","Dimming_Control","Backlight_Control","Brightness_Precision_Bits",
            "Panel_Part_Number","TCon_Part_Number","Panel_TCon_Release_Year","Panel_HW_Version",
            "Touch_Host_Controller","Touch_Screen_or_Pad","Touch_Host_Controller_Vendor","Touch_Support","Touch_Type","Touch_Interface","Stylus_Protocol",
            "Backlight_Power_Consumption","Data_Logic_Power_Consumption","Total_Power_Consumption","Panel_DC_Voltage","VBatt_POL"
        };
        for (int i = 0; i < headers.Count; i++) ws.Cell(1, i + 1).Value = headers[i];
        
        int row = 2;
        foreach (var d in designRows)
        {
            var designPanelList = panels.Where(p => p.design_id == d.DesignId).ToList();
            if (designPanelList.Count == 0)
            {
                // Write design row with no panel - basic info
                ws.Cell(row, 1).Value = d.DesignId;
                ws.Cell(row, 2).Value = d.Platform;
                ws.Cell(row, 3).Value = d.SKU;
                ws.Cell(row, 4).Value = d.ODM;
                ws.Cell(row, 5).Value = d.OEM;
                ws.Cell(row, 6).Value = d.DesignName;
                ws.Cell(row, 7).Value = d.If_;
                ws.Cell(row, 8).Value = d.FCS;
                ws.Cell(row, 9).Value = d.NumberOfPanels;
                // Hardware configuration
                ws.Cell(row, 10).Value = d.Segment;
                ws.Cell(row, 11).Value = d.Config;
                ws.Cell(row, 12).Value = d.PlValues;
                ws.Cell(row, 13).Value = d.MemoryCapacity;
                ws.Cell(row, 14).Value = d.FormFactor;
                ws.Cell(row, 15).Value = d.Soc;
                ws.Cell(row, 16).Value = d.Pch;
                ws.Cell(row, 17).Value = d.Uma;
                ws.Cell(row, 18).Value = d.HybridGfx;
                ws.Cell(row, 19).Value = d.DgpuVendor;
                // Timeline
                ws.Cell(row, 20).Value = d.EvtStart;
                ws.Cell(row, 21).Value = d.EvtEnd;
                ws.Cell(row, 22).Value = d.FvtStart;
                ws.Cell(row, 23).Value = d.FvtEnd;
                ws.Cell(row, 24).Value = d.SitStart;
                ws.Cell(row, 25).Value = d.SitEnd;
                // Features & logistics
                ws.Cell(row, 26).Value = d.AthenaEvoCandidate;
                ws.Cell(row, 27).Value = d.Gaming;
                ws.Cell(row, 28).Value = d.AiFeatures;
                ws.Cell(row, 29).Value = d.PO;
                ws.Cell(row, 30).Value = d.POSite;
                ws.Cell(row, 31).Value = d.PvNeed;
                ws.Cell(row, 32).Value = d.ShipOutOs;
                ws.Cell(row, 33).Value = d.HwAvailability;
                ws.Cell(row, 34).Value = d.PostConfig;
                ws.Cell(row, 35).Value = d.Notes;
                row++;
            }
            else
                { 
                bool first = true;
                foreach (var p in designPanelList)
                {
                    // Write design columns only for first panel row, leave blank for subsequent to visually group
                    if (first)
                    {
                        // Basic info
                        ws.Cell(row, 1).Value = d.DesignId;
                        ws.Cell(row, 2).Value = d.Platform;
                        ws.Cell(row, 3).Value = d.SKU;
                        ws.Cell(row, 4).Value = d.ODM;
                        ws.Cell(row, 5).Value = d.OEM;
                        ws.Cell(row, 6).Value = d.DesignName;
                        ws.Cell(row, 7).Value = d.If_;
                        ws.Cell(row, 8).Value = d.FCS;
                        ws.Cell(row, 9).Value = d.NumberOfPanels;
                        // Hardware configuration
                        ws.Cell(row, 10).Value = d.Segment;
                        ws.Cell(row, 11).Value = d.Config;
                        ws.Cell(row, 12).Value = d.PlValues;
                        ws.Cell(row, 13).Value = d.MemoryCapacity;
                        ws.Cell(row, 14).Value = d.FormFactor;
                        ws.Cell(row, 15).Value = d.Soc;
                        ws.Cell(row, 16).Value = d.Pch;
                        ws.Cell(row, 17).Value = d.Uma;
                        ws.Cell(row, 18).Value = d.HybridGfx;
                        ws.Cell(row, 19).Value = d.DgpuVendor;
                        // Timeline
                        ws.Cell(row, 20).Value = d.EvtStart;
                        ws.Cell(row, 21).Value = d.EvtEnd;
                        ws.Cell(row, 22).Value = d.FvtStart;
                        ws.Cell(row, 23).Value = d.FvtEnd;
                        ws.Cell(row, 24).Value = d.SitStart;
                        ws.Cell(row, 25).Value = d.SitEnd;
                        // Features & logistics
                        ws.Cell(row, 26).Value = d.AthenaEvoCandidate;
                        ws.Cell(row, 27).Value = d.Gaming;
                        ws.Cell(row, 28).Value = d.AiFeatures;
                        ws.Cell(row, 29).Value = d.PO;
                        ws.Cell(row, 30).Value = d.POSite;
                        ws.Cell(row, 31).Value = d.PvNeed;
                        ws.Cell(row, 32).Value = d.ShipOutOs;
                        ws.Cell(row, 33).Value = d.HwAvailability;
                        ws.Cell(row, 34).Value = d.PostConfig;
                        ws.Cell(row, 35).Value = d.Notes;
                        first = false;
            }
                    // Panel base fields (columns shifted by +26 for new design fields)
                    ws.Cell(row, 36).Value = p.panel_id;
                    ws.Cell(row, 37).Value = p.panel_vendor;
                    ws.Cell(row, 38).Value = p.tcon_vendor;

                    // Spec fields (columns shifted by +26)
                    specsByPanelId.TryGetValue(p.panel_id, out var spec);
                    ws.Cell(row, 39).Value = spec?.PanelPortNumber;
                    ws.Cell(row, 40).Value = spec?.EdidVersion;
                    ws.Cell(row, 41).Value = spec?.Resolution;
                    ws.Cell(row, 42).Value = spec?.RR_Min_Hz;
                    ws.Cell(row, 43).Value = spec?.RR_Max_Hz;
                    ws.Cell(row, 44).Value = spec?.Aspect_Ratio;
                    ws.Cell(row, 45).Value = spec?.Htotal;
                    ws.Cell(row, 46).Value = spec?.Vtotal;
                    ws.Cell(row, 47).Value = spec?.Screen_Size_Inch;
                    ws.Cell(row, 48).Value = spec?.Color_Depth;
                    ws.Cell(row, 49).Value = spec?.FRC;
                    ws.Cell(row, 50).Value = spec?.Data_Lane_Count;
                    ws.Cell(row, 51).Value = spec?.Data_Link_Rate;

                    ws.Cell(row, 52).Value = spec?.PSR1_Supported;
                    ws.Cell(row, 53).Value = spec?.PSR2_Supported;
                    ws.Cell(row, 54).Value = spec?.PSR2_ET_Supported;
                    ws.Cell(row, 55).Value = spec?.Intel_LRR_Version;
                    ws.Cell(row, 56).Value = spec?.HDR_Supported;
                    ws.Cell(row, 57).Value = spec?.VRR_Supported;
                    ws.Cell(row, 58).Value = spec?.VDSC_Version;
                    ws.Cell(row, 59).Value = spec?.CoG_MSO;
                    ws.Cell(row, 60).Value = spec?.IIDT_UBRR;
                    ws.Cell(row, 61).Value = spec?.Camera_Sensor_For_UBRR;
                    ws.Cell(row, 62).Value = spec?.Seamless_Drrs;
                    ws.Cell(row, 63).Value = spec?.xPST_Supported;
                    ws.Cell(row, 64).Value = spec?.xPST_Level;
                    ws.Cell(row, 65).Value = spec?.OPST_ELP;
                    ws.Cell(row, 66).Value = spec?.DE_Early_Wake;
                    ws.Cell(row, 67).Value = spec?.EPSM60;
                    ws.Cell(row, 68).Value = spec?.EG_Endurance_Gaming;

                    ws.Cell(row, 69).Value = spec?.EDP1_5_Panel_Replay;
                    ws.Cell(row, 70).Value = spec?.EDP1_5_PR_ET;
                    ws.Cell(row, 71).Value = spec?.EDP1_5_CMRR;
                    ws.Cell(row, 72).Value = spec?.EDP1_5_Periodic_ASSDP;
                    ws.Cell(row, 73).Value = spec?.EDP1_5_LOBF;
                    ws.Cell(row, 74).Value = spec?.Other_Features;

                    ws.Cell(row, 75).Value = spec?.Color_Gamut_sRGB;
                    ws.Cell(row, 76).Value = spec?.Max_Brightness_Nits;
                    ws.Cell(row, 77).Value = spec?.Backlight_Light_Source;
                    ws.Cell(row, 78).Value = spec?.Dimming_Control;
                    ws.Cell(row, 79).Value = spec?.Backlight_Control;
                    ws.Cell(row, 80).Value = spec?.Brightness_Precision_Bits;

                    ws.Cell(row, 81).Value = spec?.Panel_Part_Number;
                    ws.Cell(row, 82).Value = spec?.TCon_Part_Number;
                    ws.Cell(row, 83).Value = spec?.Panel_TCon_Release_Year;
                    ws.Cell(row, 84).Value = spec?.Panel_HW_Version;

                    ws.Cell(row, 85).Value = spec?.Touch_Host_Controller;
                    ws.Cell(row, 86).Value = spec?.Touch_Screen_or_Pad;
                    ws.Cell(row, 87).Value = spec?.Touch_Host_Controller_Vendor;
                    ws.Cell(row, 88).Value = spec?.Touch_Support;
                    ws.Cell(row, 89).Value = spec?.Touch_Type;
                    ws.Cell(row, 90).Value = spec?.Touch_Interface;
                    ws.Cell(row, 91).Value = spec?.Stylus_Protocol;

                    ws.Cell(row, 92).Value = spec?.Backlight_Power_Consumption;
                    ws.Cell(row, 93).Value = spec?.Data_Logic_Power_Consumption;
                    ws.Cell(row, 94).Value = spec?.Total_Power_Consumption;
                    ws.Cell(row, 95).Value = spec?.Panel_DC_Voltage;
                    ws.Cell(row, 96).Value = spec?.VBatt_POL;

                    row++;
        }
            }
        }
        
        ws.Columns().AdjustToContents();

        using var stream = new MemoryStream();
        wb.SaveAs(stream);
        stream.Position = 0;
        var fileName = $"{CurrentIdsid}-{DateTime.UtcNow:dd/MM/yyyy}.xlsx";
        return File(stream.ToArray(), "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", fileName);
    }

    // Normalize query and force scope to All so results are not limited to ownership
    public IActionResult OnGetSearch()
        {
        var q = string.IsNullOrWhiteSpace(Query) ? null : Query?.Trim();
        return RedirectToPage(new { Platform, SKU, Scope = "All", Query = q });
    }

    // New: dedicated AI search handler that filters designs by panel specs intent (e.g., PSR1 support)
    public IActionResult OnGetAiSearch()
            { 
        var aiq = string.IsNullOrWhiteSpace(AiQuery) ? null : AiQuery!.Trim();
        // Redirect to Index with AiQuery bound; OnGet will pick it up and keep normal bootstrapping
        return RedirectToPage(new { Platform, SKU, Scope, AiQuery = aiq });
        }
        
    public IActionResult OnPostCreateDesign()
        {
        if (_currentIdsid == null) _currentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);

        // Note: We no longer validate that NewDesignName must be in CRTT list
        // Users can now create designs with custom names
        // CRTT names are still suggested via datalist for convenience
        
        if (!ModelState.IsValid)
        {
            TempData["WelcomeMessage"] = string.Join("; ", ModelState.Values.SelectMany(v => v.Errors).Select(e => e.ErrorMessage));
            return RedirectToPage(new { Platform, SKU, Scope });
        }

        var design = new Design
        {
            Platform = NewPlatform!,
            SKU = NewSKU!,
            ODM = NewODM ?? string.Empty,
            OEM = NewOEM!,
            DesignName = NewDesignName!,
            If_ = NewIf ?? string.Empty,
            FCS = NewFCS ?? string.Empty,
            NumberOfPanels = NewNumberOfPanels <= 0 ? 1 : NewNumberOfPanels
        };
        _db.Designs.Add(design);
        _db.SaveChanges();
        
        // Add ownership record for current user
        _db.DesignOwners.Add(new DesignOwner { DesignId = design.DesignId, Idsid = CurrentIdsid });
        _db.SaveChanges();

        TempData["WelcomeMessage"] = "Design added.";
        TempData["ShowUploadExcelHint"] = "1"; // flag to show 2s flash card
        return RedirectToPage(new { Platform = NewPlatform, SKU = "ALL", Scope = "MyDesigns" });
    }

    public IActionResult OnPostDelete(int id)
    {
        if (_currentIdsid == null) _currentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);
        var design = _db.Designs.FirstOrDefault(d => d.DesignId == id);
        if (design != null)
        {
            bool isOwner = _db.DesignOwners.Any(o => o.DesignId == id && o.Idsid == CurrentIdsid);
            if (isOwner)
            {
                _db.Designs.Remove(design);
                var ownerships = _db.DesignOwners.Where(o => o.DesignId == id).ToList();
                _db.DesignOwners.RemoveRange(ownerships);
                _db.SaveChanges();
                TempData["WelcomeMessage"] = "Design deleted.";
            }
            else
            {
                TempData["WelcomeMessage"] = "Not authorized to delete this design.";
            }
        }
        return RedirectToPage(new { Platform, SKU, Scope });
    }

    public IActionResult OnPostDeletePanel(int panelId)
    {
        var panel = _db.Panels.FirstOrDefault(p => p.panel_id == panelId);
        if (panel != null)
        {
            _db.Panels.Remove(panel);
            _db.SaveChanges();
            TempData["WelcomeMessage"] = "Panel deleted.";
        }
        return RedirectToPage(new { Platform, SKU, Scope });
    }

    private bool IsOwner(int designId)
    {
        if (_currentIdsid == null) _currentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);
        if (string.IsNullOrWhiteSpace(_currentIdsid)) return false;
        return _db.DesignOwners.Any(o => o.DesignId == designId && o.Idsid == _currentIdsid);
    }

    public async Task<IActionResult> OnPostUploadExcel()
    {
        if (ExcelFile == null || ExcelFile.Length == 0)
        {
            Console.WriteLine("No file uploaded.");
            TempData["WelcomeMessage"] = "No file selected.";
            return RedirectToPage(new { Platform, SKU, Scope });
        }

        if (_currentIdsid == null) _currentIdsid = ExtractIdsid(HttpContext.User?.Identity?.Name);

        try
        {
            using var stream = new MemoryStream();
            await ExcelFile.CopyToAsync(stream);
            stream.Position = 0;
            using var workbook = new XLWorkbook(stream);
            var rows = new List<Dictionary<string, string>>();

            foreach (var ws in workbook.Worksheets)
            {
                var firstRowUsed = ws.FirstRowUsed();
                if (firstRowUsed == null) continue;
                var headerRow = firstRowUsed.RowUsed();
                var headers = headerRow.Cells().Select(c => c.GetString()).ToList();

                var currentRow = headerRow.RowNumber() + 1;
                var lastRow = ws.LastRowUsed()?.RowNumber() ?? headerRow.RowNumber();
                for (; currentRow <= lastRow; currentRow++)
                {
                    var dataRow = ws.Row(currentRow);
                    var rowDict = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
                    for (int i = 1; i <= headers.Count; i++)
                    {
                        var key = headers[i - 1]?.Trim();
                        var val = dataRow.Cell(i).GetString()?.Trim();
                        if (!string.IsNullOrEmpty(key)) rowDict[key] = val ?? string.Empty;
                    }
                    if (rowDict.Values.Any(v => !string.IsNullOrWhiteSpace(v))) rows.Add(rowDict);
                }
            }

            using var tx = await _db.Database.BeginTransactionAsync();

            var excelToDbDesignId = new Dictionary<int, int>(); // Excel DesignID -> DB DesignId

            foreach (var row in rows)
            {
                var firstKey = row.Keys.FirstOrDefault();
                var headerKey = firstKey?.Trim() ?? string.Empty;

                // Designs sheet
                if (string.Equals(headerKey, "Design ID Assign unique number to each design", StringComparison.OrdinalIgnoreCase))
                {
                    if (!row.TryGetValue("Platform", out var platform) || string.IsNullOrWhiteSpace(platform) ||
                        !row.TryGetValue("SKU", out var sku) || string.IsNullOrWhiteSpace(sku) ||
                        !row.TryGetValue("OEM", out var oem) || string.IsNullOrWhiteSpace(oem) ||
                        !row.TryGetValue("Design name", out var designName) || string.IsNullOrWhiteSpace(designName))
                    {
                        continue;
                    }

                    platform = platform.Trim();
                    sku = sku.Trim();
                    oem = oem.Trim();
                    designName = designName.Trim();

                    var excelDesignId = row.TryGetValue("Design ID Assign unique number to each design", out var excelDesignIdStr)
                        ? (int.TryParse(excelDesignIdStr?.Trim(), out var parsed) ? parsed : 0)
                        : 0;

                    var existing = _db.Designs.FirstOrDefault(d => d.Platform == platform && d.SKU == sku && d.OEM == oem && d.DesignName == designName);
                    if (existing == null) continue;

                    // Gate: process ONLY selected designs; if none selected, skip all
                    if (SelectedDesignIds == null || SelectedDesignIds.Count == 0 || !SelectedDesignIds.Contains(existing.DesignId))
                        continue;

                    if (!IsOwner(existing.DesignId))
                        continue;

                    existing.If_ = row.TryGetValue("IF Date", out var ifd) ? CleanDateString(ifd) : existing.If_;
                    existing.FCS = row.TryGetValue("RTS", out var fcsVal) ? CleanDateString(fcsVal) : existing.FCS;

                    _db.Entry(existing).Property(d => d.If_).IsModified = true;
                    _db.Entry(existing).Property(d => d.FCS).IsModified = true;
                    await _db.SaveChangesAsync();

                    if (excelDesignId > 0)
                        excelToDbDesignId[excelDesignId] = existing.DesignId;
                }
                // Panels sheet
                else if (string.Equals(headerKey, "Panels Design ID Assign unique number to each design", StringComparison.OrdinalIgnoreCase))
                {
                    var panelsExcelDesignId = row.TryGetValue("Panels Design ID Assign unique number to each design", out var panelsExcelIdStr)
                        ? (int.TryParse(panelsExcelIdStr?.Trim(), out var parsed) ? parsed : 0)
                        : 0;
                    if (panelsExcelDesignId <= 0) continue;

                    if (!excelToDbDesignId.TryGetValue(panelsExcelDesignId, out var dbDesignId))
                        continue;

                    // Gate: process ONLY selected designs; if none selected, skip all
                    if (SelectedDesignIds == null || SelectedDesignIds.Count == 0 || !SelectedDesignIds.Contains(dbDesignId))
                        continue;

                    if (!IsOwner(dbDesignId)) continue;

                    var panelVendor = row.TryGetValue("Panel Vendor", out var pv) ? (pv?.Trim() ?? string.Empty) : string.Empty;
                    var tconVendor = row.TryGetValue("TCON Vendor", out var tv) ? (tv?.Trim() ?? string.Empty) : string.Empty;

                    var existingPanel = _db.Panels.FirstOrDefault(p => p.design_id == dbDesignId && p.panel_vendor == panelVendor && p.tcon_vendor == tconVendor);
                    if (existingPanel == null)
                    {
                        var panel = new PanelEntry { design_id = dbDesignId, panel_vendor = panelVendor, tcon_vendor = tconVendor };
                        _db.Panels.Add(panel);
                        await _db.SaveChangesAsync();
                        existingPanel = panel;
                    }

                    var spec = _db.PanelSpecs.FirstOrDefault(s => s.panel_id == existingPanel.panel_id) ?? new PanelSpec { panel_id = existingPanel.panel_id };

                    void SetIfPresent(string key, Action<string?> assign)
                    {
                        if (row.TryGetValue(key, out var v)) assign(v?.Trim());
                    }

                    // Map Excel columns -> spec fields (all nvarchar -> string) using provided headers
                    SetIfPresent("Panel port #", v => spec.PanelPortNumber = v);
                    SetIfPresent("Panel_Spec_Version", v => spec.EdidVersion = v);

                    SetIfPresent("Resolution", v => spec.Resolution = v);
                    SetIfPresent("RR_Min_Hz", v => spec.RR_Min_Hz = v);
                    SetIfPresent("RR_Max_Hz", v => spec.RR_Max_Hz = v);
                    SetIfPresent("Aspect_Ratio", v => spec.Aspect_Ratio = v);
                    SetIfPresent("Htotal", v => spec.Htotal = v);
                    SetIfPresent("Vtotal", v => spec.Vtotal = v);
                    SetIfPresent("Screen_Size_Inch", v => spec.Screen_Size_Inch = v);
                    SetIfPresent("Color_Depth", v => spec.Color_Depth = v);
                    SetIfPresent("FRC", v => spec.FRC = v);
                    SetIfPresent("Data_Lane_Count", v => spec.Data_Lane_Count = v);
                    SetIfPresent("Data_Link_Rate", v => spec.Data_Link_Rate = v);

                    SetIfPresent("PSR 1 (Default yes and Panel needs to  support)", v => spec.PSR1_Supported = v);
                    SetIfPresent("PSR 2 (Default yes and Panel needs to  support)", v => spec.PSR2_Supported = v);
                    SetIfPresent("PSR2 + ET", v => spec.PSR2_ET_Supported = v);
                    SetIfPresent("Intel_LRR_Version (2.5) (Default yes and Panel needs to  support)", v => spec.Intel_LRR_Version = v);
                    SetIfPresent("HDR", v => spec.HDR_Supported = v);
                    SetIfPresent("VRR (Default yes and Panel needs to  support)", v => spec.VRR_Supported = v);
                    SetIfPresent("VDSC             (If yes, please note requested support VDSC version. 1.1 or 1.2a)", v => spec.VDSC_Version = v);
                    SetIfPresent("CoG/CoG+ MSO (MSO_2x2,MSO_4x1)", v => spec.CoG_MSO = v);
                    SetIfPresent("IIDT_Support UBRR", v => spec.IIDT_UBRR = v);
                    SetIfPresent("Camera sensor details used for UBRR", v => spec.Camera_Sensor_For_UBRR = v);
                    SetIfPresent("Seamless DRRS (Default yes  and Panel needs to  support Conflict with LRR2.5)", v => spec.Seamless_Drrs = v);
                    SetIfPresent("xPST (Default yes/level 2)", v => spec.xPST_Supported = v);
                    SetIfPresent("xPST Level", v => spec.xPST_Level = v);
                    SetIfPresent("OPST + ELP (Default yes and Panel needs to  support)", v => spec.OPST_ELP = v);
                    SetIfPresent("DE Early Wake", v => spec.DE_Early_Wake = v);
                    SetIfPresent("EPSM60 (Default No)", v => spec.EPSM60 = v);
                    SetIfPresent("EG (Endurance Gaming) (Default No)", v => spec.EG_Endurance_Gaming = v);
                    SetIfPresent("EDP1.5 Panel Replay", v => spec.EDP1_5_Panel_Replay = v);
                    SetIfPresent("EDP1.5 PR + ET", v => spec.EDP1_5_PR_ET = v);
                    SetIfPresent("EDP1.5 CMRR", v => spec.EDP1_5_CMRR = v);
                    SetIfPresent("EDP1.5 Periodic ASSDP", v => spec.EDP1_5_Periodic_ASSDP = v);
                    SetIfPresent("EDP1.5 LOBF", v => spec.EDP1_5_LOBF = v);
                    SetIfPresent("Others (Pleaese note specific feature you need us to take care) ex. companion/dual display, high nits > 500 , adaptive color, ELP", v => spec.Other_Features = v);

                    SetIfPresent("Color Gamut (sRGB %, DCI-P3 %,Adobe RGB %,NTSC %)", v => spec.Color_Gamut_sRGB = v);
                    SetIfPresent("Max_Brightness_Nits", v => spec.Max_Brightness_Nits = v);
                    SetIfPresent("Backlight_Light_Source", v => spec.Backlight_Light_Source = v);
                    SetIfPresent("Dimming_Control", v => spec.Dimming_Control = v);
                    SetIfPresent("Backlight_Control", v => spec.Backlight_Control = v);
                    SetIfPresent("Brightness_Precision_Bits", v => spec.Brightness_Precision_Bits = v);

                    SetIfPresent("Panel part #", v => spec.Panel_Part_Number = v);
                    SetIfPresent("TCon part #", v => spec.TCon_Part_Number = v);
                    SetIfPresent("Panel_Tcon_Release_Year", v => spec.Panel_TCon_Release_Year = v);
                    SetIfPresent("Panel_HW_Version", v => spec.Panel_HW_Version = v);

                    SetIfPresent("Touch Host Controller (THC) for I2C (Yes/No)", v => spec.Touch_Host_Controller = v);
                    SetIfPresent("Touch Screen or Touch Pad or Both (If THC is No, no need to fill out this)", v => spec.Touch_Screen_or_Pad = v);
                    SetIfPresent("Touch Host Controller (THC) Vendor (If THC is No, no need to fill out this)", v => spec.Touch_Host_Controller_Vendor = v);
                    SetIfPresent("Touch_Support", v => spec.Touch_Support = v);
                    SetIfPresent("Touch_Type", v => spec.Touch_Type = v);
                    SetIfPresent("Touch_Interface", v => spec.Touch_Interface = v);
                    SetIfPresent("Stylus_Protocol", v => spec.Stylus_Protocol = v);

                    SetIfPresent("Backlight_Power_Consumption", v => spec.Backlight_Power_Consumption = v);
                    SetIfPresent("Data_logic_Power_Consumption", v => spec.Data_Logic_Power_Consumption = v);
                    SetIfPresent("Total_Power_Consumption", v => spec.Total_Power_Consumption = v);
                    SetIfPresent("Panel_DC_Voltage", v => spec.Panel_DC_Voltage = v);
                    SetIfPresent("VBatt_POL", v => spec.VBatt_POL = v);

                    // Upsert save
                    if (_db.Entry(spec).State == EntityState.Detached)
                        _db.PanelSpecs.Add(spec);
                    else
                        _db.PanelSpecs.Update(spec);

                    await _db.SaveChangesAsync();
                }
            }

            // Commit once after processing all rows
            await tx.CommitAsync();
        }
        catch (Exception ex)
        {
            TempData["WelcomeMessage"] = $"Excel import failed: {ex.Message}";
        }

        return RedirectToPage(new { Platform, SKU, Scope = "MyDesigns" });
    }



    private static string CleanDateString(string? input)
    {
        var t = input?.Trim();
        if (string.IsNullOrWhiteSpace(t)) return string.Empty;
        // If a time portion exists (e.g., "07-11-2025 00:00:00"), keep only the date part before the first space.
        var spaceIdx = t.IndexOf(' ');
        return spaceIdx > 0 ? t[..spaceIdx] : t;
    }

    // DTOs for structured filters produced by GNAI
    private class FilterCondition
    {
        public string field { get; set; } = string.Empty; // PanelSpec property name
        public string op { get; set; } = "exists"; // equals, contains, not_contains, >, >=, <, <=, truthy, exists
        public string? value { get; set; }
    }
    private class FilterNode
    {
        public string? logic { get; set; } // AND / OR, default AND
        public List<FilterCondition> conditions { get; set; } = new();
        public List<FilterNode> children { get; set; } = new();
    }

    private async Task<FilterNode?> TryExtractStructuredFiltersWithGnaiAsync(string query)
    {
        // Use keyword-based filter directly (GNAI removed)
        Console.WriteLine("[AI Search] Using keyword-based filter");
        return GenerateKeywordBasedFilter(query);
    }

    private FilterNode? GenerateKeywordBasedFilter(string query)
    {
        // Simple keyword-based filter generation as fallback
        var queryLower = query.ToLowerInvariant();
        var conditions = new List<FilterCondition>();
        
        Console.WriteLine($"[KEYWORD FILTER] Analyzing query: {query}");
        
        // Map common keywords to fields
        var keywordMappings = new Dictionary<string, (string field, string op)>
        {
            {"psr1", ("PSR1_Supported", "truthy")},
            {"psr 1", ("PSR1_Supported", "truthy")},
            {"psr2", ("PSR2_Supported", "truthy")},
            {"psr 2", ("PSR2_Supported", "truthy")},
            {"hdr", ("HDR_Supported", "truthy")},
            {"vrr", ("VRR_Supported", "truthy")},
            {"vdsc", ("VDSC_Version", "exists")},
            {"seamless drrs", ("Seamless_Drrs", "truthy")},
            {"xpst", ("xPST_Supported", "truthy")},
            {"opst", ("OPST_ELP", "truthy")},
            {"elp", ("OPST_ELP", "truthy")},
        };
        
        foreach (var mapping in keywordMappings)
        {
            if (queryLower.Contains(mapping.Key))
            {
                conditions.Add(new FilterCondition 
                { 
                    field = mapping.Value.field, 
                    op = mapping.Value.op, 
                    value = null 
                });
                Console.WriteLine($"[KEYWORD FILTER] Matched keyword '{mapping.Key}' -> {mapping.Value.field} ({mapping.Value.op})");
            }
        }
        
        // Check for resolution patterns
        var resolutionMatch = System.Text.RegularExpressions.Regex.Match(queryLower, @"(\d{3,4})\s*x\s*(\d{3,4})");
        if (resolutionMatch.Success)
        {
            var resolution = $"{resolutionMatch.Groups[1].Value}x{resolutionMatch.Groups[2].Value}";
            conditions.Add(new FilterCondition 
            { 
                field = "Resolution", 
                op = "contains", 
                value = resolution 
            });
            Console.WriteLine($"[KEYWORD FILTER] Matched resolution pattern -> {resolution}");
        }
        
        if (conditions.Count == 0)
        {
            Console.WriteLine("[KEYWORD FILTER] No keywords matched. Returning null to show all designs.");
            return null;
        }
        
        var filterNode = new FilterNode
        {
            logic = "AND",
            conditions = conditions
        };
        
        Console.WriteLine($"[KEYWORD FILTER] Generated filter with {conditions.Count} condition(s)");
        return filterNode;
    }
}

