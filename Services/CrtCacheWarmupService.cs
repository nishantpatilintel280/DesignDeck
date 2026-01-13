using System;
using System.Net.Http;
using System.Text.Json;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.Extensions.Caching.Memory;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;

namespace EP.Services;

public sealed class CrttCacheWarmupService : BackgroundService
{
    private readonly ILogger<CrttCacheWarmupService> _logger;
    private readonly IConfiguration _config;
    private readonly IHttpClientFactory _httpClientFactory;
    private readonly IMemoryCache _cache;
    private readonly TimeSpan _period;

    // Must match PageModel keys so OnGet sees a warm cache
    private const string CacheKeyPtl = "CRTT_AllowedDesignNames_PTL_177_178_181";
    private const string CacheKeyNvl = "CRTT_AllowedDesignNames_NVL_194_205";
    private const string CacheKeyPtlIds = "CRTT_DesignIds_PTL_177_178_181";
    private const string CacheKeyNvlIds = "CRTT_DesignIds_NVL_194_205";
    private const string CacheKeyCustomers = "CRTT_Customers_List";

    private static readonly int[] PtlPids = new[] { 177, 178, 181 };
    private static readonly int[] NvlPids = new[] { 194, 195, 197, 198, 199, 200, 201, 202, 203, 204, 205 };

    public CrttCacheWarmupService(
        ILogger<CrttCacheWarmupService> logger,
        IConfiguration config,
        IHttpClientFactory httpClientFactory,
        IMemoryCache cache)
    {
        _logger = logger;
        _config = config;
        _httpClientFactory = httpClientFactory;
        _cache = cache;

        var hours = _config.GetValue<int?>("Crtt:WarmupPeriodHours") ?? 3;
        _period = TimeSpan.FromHours(hours > 0 ? hours : 3);
    }

    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        if (!_config.GetValue<bool>("Crtt:WarmupEnabled"))
        {
            _logger.LogInformation("CRTT warmup disabled (Crtt:WarmupEnabled=false). Skipping background job.");
            return;
        }

        await RefreshAllAsync(stoppingToken);

        using var timer = new PeriodicTimer(_period);
        while (!stoppingToken.IsCancellationRequested && await timer.WaitForNextTickAsync(stoppingToken))
        {
            await RefreshAllAsync(stoppingToken);
        }
    }

    private async Task RefreshAllAsync(CancellationToken ct)
    {
        try
        {
            // Placeholder: acquire a bearer token after hosting is configured.
            var token = await TryGetBearerTokenAsync(ct);
            if (string.IsNullOrEmpty(token))
            {
                _logger.LogWarning("CRTT warmup skipped: token unavailable. Configure AZURE_* or Azure:* after hosting.");
                return;
            }

            var api = new CrttApi(_httpClientFactory);

            // PTL design names
            var ptlNames = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            var ptlIds = new Dictionary<string, int>(StringComparer.OrdinalIgnoreCase);
            foreach (var pid in PtlPids)
            {
                var doc = await api.FetchAsync($"https://crtt-api.intel.com/designNames/?pID={pid}", token, ct);
                CrttApi.AddDesignNamesFromCrtt(doc, ptlNames, ptlIds);
            }
            _cache.Set(CacheKeyPtl, ptlNames, new MemoryCacheEntryOptions { AbsoluteExpirationRelativeToNow = TimeSpan.FromHours(12) });
            _cache.Set(CacheKeyPtlIds, ptlIds, new MemoryCacheEntryOptions { AbsoluteExpirationRelativeToNow = TimeSpan.FromHours(12) });

            // NVL design names
            var nvlNames = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            var nvlIds = new Dictionary<string, int>(StringComparer.OrdinalIgnoreCase);
            foreach (var pid in NvlPids)
            {
                var doc = await api.FetchAsync($"https://crtt-api.intel.com/designNames/?pID={pid}", token, ct);
                CrttApi.AddDesignNamesFromCrtt(doc, nvlNames, nvlIds);
            }
            _cache.Set(CacheKeyNvl, nvlNames, new MemoryCacheEntryOptions { AbsoluteExpirationRelativeToNow = TimeSpan.FromHours(12) });
            _cache.Set(CacheKeyNvlIds, nvlIds, new MemoryCacheEntryOptions { AbsoluteExpirationRelativeToNow = TimeSpan.FromHours(12) });

            // Customers
            var customersDoc = await api.FetchAsync("https://crtt-api.intel.com/customers", token, ct);
            var customers = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            CrttApi.AddCustomersFromCrtt(customersDoc, customers);
            _cache.Set(CacheKeyCustomers, customers, new MemoryCacheEntryOptions { AbsoluteExpirationRelativeToNow = TimeSpan.FromHours(12) });

            _logger.LogInformation("CRTT warmup completed. PTL={PtlCount}, NVL={NvlCount}, Customers={CustCount}", ptlNames.Count, nvlNames.Count, customers.Count);
        }
        catch (OperationCanceledException) { }
        catch (Exception ex)
        {
            _logger.LogError(ex, "CRTT warmup failed");
        }
    }

    private Task<string?> TryGetBearerTokenAsync(CancellationToken ct)
    {
        var azureAppId = Environment.GetEnvironmentVariable("AZURE_APP_ID") ?? _config["Azure:AppId"];
        var azureTenantId = Environment.GetEnvironmentVariable("AZURE_TENANT_ID") ?? _config["Azure:TenantId"];
        var userEmail = Environment.GetEnvironmentVariable("AZURE_USER_EMAIL") ?? _config["Azure:UserEmail"];
        var userPassword = Environment.GetEnvironmentVariable("AZURE_USER_PASSWORD") ?? _config["Azure:UserPassword"];

        if (string.IsNullOrWhiteSpace(azureAppId) || string.IsNullOrWhiteSpace(azureTenantId) ||
            string.IsNullOrWhiteSpace(userEmail) || string.IsNullOrWhiteSpace(userPassword))
        {
            return Task.FromResult<string?>(null);
        }

        // Placeholder: implement token acquisition in hosted env.
        return Task.FromResult<string?>(null);
    }
}
