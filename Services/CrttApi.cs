using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;

namespace EP.Services;

public class CrttApi
{
    private readonly IHttpClientFactory? _httpClientFactory;

    public CrttApi(IHttpClientFactory? httpClientFactory = null)
    {
        _httpClientFactory = httpClientFactory;
    }

    public async Task<JsonDocument?> FetchAsync(string url, string bearerToken, CancellationToken ct, int maxRetries = 5, int initialDelayMs = 200)
    {
        var http = _httpClientFactory?.CreateClient() ?? new HttpClient();
        http.Timeout = TimeSpan.FromSeconds(30);
        for (int attempt = 1; attempt <= maxRetries; attempt++)
        {
            try
            {
                using var req = new HttpRequestMessage(HttpMethod.Get, url);
                req.Headers.Authorization = new AuthenticationHeaderValue("Bearer", bearerToken);
                using var resp = await http.SendAsync(req, ct);
                if (resp.IsSuccessStatusCode)
                {
                    return await JsonDocument.ParseAsync(await resp.Content.ReadAsStreamAsync(ct), cancellationToken: ct);
                }

                if ((int)resp.StatusCode == 404 || (int)resp.StatusCode == 429 || (int)resp.StatusCode >= 500)
                {
                    int delay = initialDelayMs * (int)Math.Pow(2, attempt - 1);
                    await Task.Delay(delay, ct);
                    continue;
                }
                return null;
            }
            catch (OperationCanceledException) { throw; }
            catch
            {
                int delay = initialDelayMs * (int)Math.Pow(2, attempt - 1);
                await Task.Delay(delay, ct);
            }
        }
        return null;
    }

    public static void AddDesignNamesFromCrtt(JsonDocument? doc, HashSet<string> namesTarget, Dictionary<string, int> idMapTarget)
    {
        if (doc == null) return;
        var root = doc.RootElement;
        if (root.ValueKind != JsonValueKind.Array) return;

        foreach (var item in root.EnumerateArray())
        {
            if (item.TryGetProperty("DesignName", out var dn) && dn.ValueKind == JsonValueKind.String)
            {
                var name = dn.GetString();
                if (!string.IsNullOrWhiteSpace(name))
                {
                    namesTarget.Add(name);
                    if (item.TryGetProperty("DesignID", out var did) && did.ValueKind == JsonValueKind.Number)
                    {
                        idMapTarget[name] = did.GetInt32();
                    }
                }
            }
        }
    }

    public static void AddCustomersFromCrtt(JsonDocument? doc, HashSet<string> customersTarget)
    {
        if (doc == null) return;
        var root = doc.RootElement;
        if (root.ValueKind != JsonValueKind.Array) return;

        foreach (var item in root.EnumerateArray())
        {
            string? value = null;
            if (item.TryGetProperty("CustomerName", out var customerName) && customerName.ValueKind == JsonValueKind.String)
                value = customerName.GetString();
            else if (item.TryGetProperty("Customer", out var customer) && customer.ValueKind == JsonValueKind.String)
                value = customer.GetString();
            else if (item.TryGetProperty("OEM", out var oem) && oem.ValueKind == JsonValueKind.String)
                value = oem.GetString();
            else if (item.TryGetProperty("Name", out var name) && name.ValueKind == JsonValueKind.String)
                value = name.GetString();

            if (!string.IsNullOrWhiteSpace(value))
                customersTarget.Add(value.Trim());
        }
    }
}
