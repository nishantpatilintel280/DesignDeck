using Microsoft.AspNetCore.Authentication.Negotiate;
using Microsoft.EntityFrameworkCore;

var builder = WebApplication.CreateBuilder(args);

// Increase Kestrel header limits to avoid HTTP 431 (Request Header Fields Too Large)
builder.WebHost.ConfigureKestrel(options =>
{
    // Default is 32 KB; increase to 64 KB (adjust as needed)
    options.Limits.MaxRequestHeadersTotalSize = 64 * 1024;
});

// Add services to the container.
builder.Services.AddRazorPages();
builder.Services.AddSession();
builder.Services.AddMemoryCache(); // Ensure memory cache is registered
builder.Services.AddHttpClient(); // Ensure HTTP client factory is registered

// Use Negotiate authentication (without the problematic option)
builder.Services.AddAuthentication(NegotiateDefaults.AuthenticationScheme)
    .AddNegotiate(); // Remove the options configuration

// Force authentication for all requests
builder.Services.AddAuthorization(options =>
{
    options.FallbackPolicy = options.DefaultPolicy;
});

builder.Services.AddDbContext<ApplicationDbContext>(options =>
    options.UseSqlServer(builder.Configuration.GetConnectionString("DefaultConnection")));

// Register the CRTT warmup background job only when enabled via config (default false for local dev)
if (builder.Configuration.GetValue<bool>("Crtt:WarmupEnabled"))
{
    builder.Services.AddHostedService<EP.Services.CrttCacheWarmupService>();
}

var app = builder.Build();

using (var scope = app.Services.CreateScope())
{
    var db = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();
    try
    {
        db.Database.CanConnect();
        Console.WriteLine("Database connection successful!");
    }
    catch (Exception ex)
    {
        Console.WriteLine($"Database connection failed: {ex.Message}");
    }
}

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Error");
    app.UseHsts();
}

app.UseHttpsRedirection();
app.UseStaticFiles();

app.UseRouting();

// IMPORTANT: Authentication must come before Authorization
app.UseAuthentication();
app.UseAuthorization();

app.UseSession();

app.MapRazorPages();

app.Run();