using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;

namespace EP.Pages
{
    public class InfoModel : PageModel
    {
        public string Title { get; set; } = "Project Information";
        public string Description { get; set; } = "This page shows detailed info about the project.";
        public string Version { get; set; } = "1.0.0";
        public string Author { get; set; } = "Intel";
        public void OnGet()
        {
        }
    }
}
