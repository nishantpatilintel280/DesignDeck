using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;

namespace EP.Pages
{
    public class First_PageModel : PageModel
    {
        
        public void OnGet()
        {
            
        }

        private readonly ILogger<IndexModel> _logger;
        private readonly IWebHostEnvironment _env;

        // Public property to hold extension info strings for the Razor page
        public EP.Parser.PanelInfo extInfo { get; set; } = new();

        public First_PageModel(ILogger<IndexModel> logger, IWebHostEnvironment env)
        {
            _logger = logger;
            _env = env;
        }

        public void OnPostParse()
        {
            string assetsPath = Path.Combine(_env.WebRootPath, "assets");
            Path.Combine(assetsPath, "myfile.txt");

            string edidPath1 = Path.Combine(assetsPath, "inputEdid.bin");
            string edidPath2 = Path.Combine(assetsPath, "newnew.bin");

            string vbtPath = @"";

            string dpcdPath = Path.Combine(assetsPath, "dpcd.txt");

            var parser = new EP.Parser.Parser();
            extInfo = parser.ParseAll(edidPath2, vbtPath, dpcdPath);
        }
    }
}
