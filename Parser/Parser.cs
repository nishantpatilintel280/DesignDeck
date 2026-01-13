using Microsoft.Identity.Client;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

namespace EP.Parser
{
    public class PanelInfo
    {
        // Panel capabilities
        public string? PanelPortNumber = null;
        public string? EdidVersion = null;
        public string? Resolution = null;
        public string? RR_Min_Hz = null;
        public string? RR_Max_Hz = null;
        public string? Aspect_Ratio = null;
        public string? Htotal = null;
        public string? Vtotal = null;
        public string? Screen_Size_Inch = null;
        public string? Color_Depth = null;
        public string? FRC = null;
        public string? Data_Lane_Count = null;
        public string? Data_Link_Rate = null;

        // Feature support
        public bool PSR1_Supported = false;
        public bool PSR2_Supported = false;
        public bool PSR2_ET_Supported = false;  // ET = ?
        public string? Intel_LRR_Version = null;
        public bool HDR_Supported = false;
        public bool VRR_Supported = false;
        public string? VDSC_Version = null;
        public string? CoG_MSO = null; // e.g. MSO_2x2 etc
        public bool IIDT_UBRR = false;
        public string? Camera_Sensor_For_UBRR = null;
        public bool Seamless_Drrs = false;
        public bool xPST_Supported = false;
        public int xPST_Level = -1;
        public bool OPST_ELP = false;
        public bool DE_Early_Wake = false;
        public bool EPSM60 = false;
        public bool EG_Endurance_Gaming = false;
        public string? EDP1_5_Panel_Replay = null;
        public string? EDP1_5_PR_ET = null;
        public bool EDP1_5_CMRR = false;
        public bool EDP1_5_Periodic_ASSDP = false;
        public bool EDP1_5_LOBF = false;
        public string? Other_Features = null;

        // Panel color gamut and backlight
        public string? Color_Gamut_sRGB = null;
        public string? Color_Gamut_DCI_P3 = null;
        public string? Color_Gamut_AdobeRGB = null;
        public string? Color_Gamut_NTSC = null;
        public string? Max_Brightness_Nits = null;
        public string? Backlight_Light_Source = null;
        public string? Dimming_Control = null;
        public string? Backlight_Control = null;
        public string? Brightness_Precision_Bits = null;

        // Panel card and TCON
        public string? Panel_Vendor = null;
        public string? Panel_Part_Number = null;
        public string? TCon_Vendor = null;
        public string? TCon_Part_Number = null;
        public string? Panel_TCon_Release_Year = null;
        public string? Panel_HW_Version = null;

        // Touch
        public bool Touch_Host_Controller = false;
        public string? Touch_Screen_or_Pad = null;
        public string? Touch_Host_Controller_Vendor = null;
        public bool Touch_Support = false;
        public string? Touch_Type = null;
        public string? Touch_Interface = null;
        public string? Stylus_Protocol = null;

        // Power
        public string? Backlight_Power_Consumption = null;
        public string? Data_Logic_Power_Consumption = null;
        public string? Total_Power_Consumption = null;
        public string? Panel_DC_Voltage = null;
        public string? VBatt_POL = null;

        //extra
        public bool IsContinuousFreqSupported = false;
    }

    public class Parser()
    {
        public PanelInfo ParseAll(string edidPath, string vbtPath, string dpcdPath)
        {
            var info = new PanelInfo();

            if (File.Exists(edidPath))
                ParseEdid(edidPath, info);

            if (File.Exists(vbtPath))
                ParseVBT(vbtPath, info);

            if (File.Exists(dpcdPath))
                ParseDPCD(dpcdPath, info);

            return info;
        }
        private int Gcd(int a, int b)
        {
            while (b != 0)
            {
                int temp = b;
                b = a % b;
                a = temp;
            }
            return a;
        }
        private void ParseEdid(string edidPath, PanelInfo info)
        {
            // what we get from edid:
            // EdidVersion
            // Color_Depth
            // Screen_Size_Inch
            // Screen_Size_Inch
            // Htotal
            // Vtotal
            // Aspect_Ratio
            // RR_Min_Hz
            // RR_Max_Hz
            // Color_Gamut_sRGB
            // Panel_Vendor
            // Panel_Part_Number

            byte[] edid = File.ReadAllBytes(edidPath);
            if (edid.Length < 128)
            {
                Console.WriteLine("EDID length less than 128B.");
                return;
            }

            // EDID version
            int version = edid[0x12];
            int revision = edid[0x13];
            info.EdidVersion = $"{version}.{revision}";

            // Color depth & interface/digital input, from byte 20 (index 0x14) in EDID v1.4 etc.
            // EDID basic display parameters: video input params (digital/analog), bit depth etc
            byte inputParams = edid[0x14];
            bool isDigital = (inputParams & 0x80) != 0;
            if (isDigital)
            {
                // Bits 6-4 give bit depth
                int bitDepthCode = (inputParams >> 4) & 0x7; // 3 bits
                string colorDepth = "Unknown";
                switch (bitDepthCode)
                {
                    case 1: colorDepth = "6 bpc"; break;
                    case 2: colorDepth = "8 bpc"; break;
                    case 3: colorDepth = "10 bpc"; break;
                    case 4: colorDepth = "12 bpc"; break;
                    case 5: colorDepth = "14 bpc"; break;
                    case 6: colorDepth = "16 bpc"; break;
                    default: colorDepth = "Not Specified"; break;
                }
                info.Color_Depth = colorDepth;
            }

            // Screen size
            int widthCm = edid[0x15];
            int heightCm = edid[0x16];
            if (widthCm > 0 && heightCm > 0)
            {
                double diagonalInch = Math.Sqrt(widthCm * widthCm + heightCm * heightCm) / 2.54;
                info.Screen_Size_Inch = Math.Round(diagonalInch).ToString();
            }

            // Resolution, Htotal, Vtotal, aspect ratio etc from first Detailed Timing Descriptor
            int dtdOffset = -1;
            for (int i = 0x36; i <= 0x6F; i += 18)
            {
                if (edid[i] != 0x00 || edid[i + 1] != 0x00)
                {
                    dtdOffset = i;
                    break;
                }
            }

            if (dtdOffset != -1 && edid.Length >= dtdOffset + 18)
            {
                int hActive = (((edid[dtdOffset + 4] & 0xF0) >> 4) << 8) | edid[dtdOffset + 2];
                int hBlank = ((edid[dtdOffset + 4] & 0x0F) << 8) | edid[dtdOffset + 3];
                int vActive = (((edid[dtdOffset + 7] & 0xF0) >> 4) << 8) | edid[dtdOffset + 5];
                int vBlank = ((edid[dtdOffset + 7] & 0x0F) << 8) | edid[dtdOffset + 6];

                int hTotal = hActive + hBlank;
                int vTotal = vActive + vBlank;

                info.Resolution = $"{hActive}x{vActive}";
                info.Htotal = hTotal.ToString();
                info.Vtotal = vTotal.ToString();
                if (vActive != 0 && hActive != 0)
                {
                    int gcd = Gcd(hActive, vActive);
                    int aspectWidth = hActive / gcd;
                    int aspectHeight = vActive / gcd;
                    info.Aspect_Ratio = $"{aspectWidth}:{aspectHeight}";
                }
                else
                {
                    info.Aspect_Ratio = "N/A";
                }

                // Pixel clock (in 10 kHz units)
                int pixelClock = edid[dtdOffset] + (edid[dtdOffset + 1] << 8);

                if (pixelClock > 0 && hTotal > 0 && vTotal > 0)
                {
                    double refresh = (pixelClock * 10000.0) / (hTotal * vTotal);
                    info.RR_Min_Hz = refresh.ToString("0.00");
                    info.RR_Max_Hz = refresh.ToString("0.00");
                }
            }
            else
            {
                Console.WriteLine("No valid DTD found. Resolution info may be missing.");
            }

            // Range limits descriptor: min/max refresh rate, IsContinuousFreqSupported
            for (int i = 0x36; i <= 0x6C; i += 18)
            {
                if (edid.Length > i + 3 && edid[i + 3] == 0xFD)
                {
                    byte minV = edid[i + 5];
                    byte maxV = edid[i + 6];
                    info.RR_Min_Hz = minV.ToString();
                    info.RR_Max_Hz = maxV.ToString();
                    if ((edid[i + 10] & 0x02) == 0x02) info.IsContinuousFreqSupported = true;
                    break;
                }
            }

            if (edid.Length > 0x18)
            {
                if ((edid[0x18] & 0x01) != 0)
                    info.Color_Gamut_sRGB = "Yes";
                else
                    info.Color_Gamut_sRGB = "No";
            }

            for (int i = 0x36; i <= 0x6C; i += 18)
            {
                if (edid[i] == 0x00 && edid[i + 1] == 0x00)
                {
                    byte tag = edid[i + 3];
                    if (tag == 0xFF) // Serial Number
                    {
                        string serial = Encoding.ASCII.GetString(edid, i + 5, 13).Trim();
                        if (!string.IsNullOrWhiteSpace(serial))
                            info.Panel_Part_Number = serial; info.PanelPortNumber = serial;
                    }
                    else if (tag == 0xFC) // Display name
                    {
                        string name = Encoding.ASCII.GetString(edid, i + 5, 13).Trim();
                        if (!string.IsNullOrWhiteSpace(name))
                            info.Panel_Part_Number = name; info.PanelPortNumber = name;
                    }
                }
            }

            if(edid.Length > 0x18){
                if (edid[0x18] == 0)
                    info.Intel_LRR_Version = "Not Supported";
                else
                    info.Intel_LRR_Version = "Yes";
            }

            ushort manufacturerCode = (ushort)((edid[0x08] << 8) | edid[0x09]);
            char[] vendor = new char[3];
            vendor[0] = (char)(((manufacturerCode >> 10) & 0x1F) + '@');
            vendor[1] = (char)(((manufacturerCode >> 5) & 0x1F) + '@');
            vendor[2] = (char)((manufacturerCode & 0x1F) + '@');
            info.Panel_Vendor = new string(vendor);

        }

        private void ParseVBT(string edidPath, PanelInfo info) 
        {
            // Seamless_Drrs
            // xPST_Supported
            // xPST_Level
            // OPST_ELP
            // EDP1_5_Panel_Replay
            // Backlight_Control
            // Brightness_Precision_Bits
            // Backlight_Light_Source (maybe)
        }

        private void ParseDPCD(string dpcdPath, PanelInfo info)
        {
            // what we get from dpcd:
            // Data_Lane_Count
            // Data_Link_Rate
            // PSR1_Supported
            // PSR2_Supported
            // VRR_Supported
            // VDSC_Version
            // Intel_LRR_Version

            if (!File.Exists(dpcdPath))
                throw new FileNotFoundException("DPCD file not found", dpcdPath);

            Regex lineRegex = new Regex(@"0x([0-9A-Fa-f]+):\s*((?:0x[0-9A-Fa-f]{2},?\s*)+)");
            Regex byteRegex = new Regex(@"0x([0-9A-Fa-f]{2})");

            string[] lines = File.ReadAllLines(dpcdPath);

            // Use dictionary to hold offset -> byte, handles sparse large offsets
            var dpcdDict = new Dictionary<int, byte>();

            int maxOffset = 0;

            foreach (var line in lines)
            {
                var lineMatch = lineRegex.Match(line);
                if (!lineMatch.Success) continue;

                if (!int.TryParse(lineMatch.Groups[1].Value, System.Globalization.NumberStyles.HexNumber, null, out int baseOffset))
                    continue;

                string bytesBlock = lineMatch.Groups[2].Value;
                var byteMatches = byteRegex.Matches(bytesBlock);

                for (int i = 0; i < byteMatches.Count; i++)
                {
                    byte b = Convert.ToByte(byteMatches[i].Groups[1].Value, 16);
                    int offset = baseOffset + i;

                    dpcdDict[offset] = b;

                    if (offset > maxOffset)
                        maxOffset = offset;
                }
            }

            // Allocate byte array large enough for all data, default 0 for missing bytes
            byte[] dpcdBytes = new byte[maxOffset + 1];
            foreach (var kvp in dpcdDict)
            {
                dpcdBytes[kvp.Key] = kvp.Value;
            }

            // Check register 0x70 and bit 1 for PSR2 support
            if (dpcdBytes.Length <= 0x70)
                throw new ArgumentException("DPCD data too short to contain register 0x70");

            byte reg70 = dpcdBytes[0x70];

            if((reg70 & 0x01) != 0)
            {
                info.PSR1_Supported = true;
            }

            if ((reg70 & 0x02) != 0 || (reg70 & 0x03) != 0 || (reg70 & 0x04) != 0)
            {
                info.PSR1_Supported = true;
                info.PSR2_Supported = true;
            }

            info.PSR2_ET_Supported = (reg70 & 0x04) != 0;

            // Register 0x75 - Some feature flags (example check)
            // Make sure we have 0x75
            if (dpcdBytes.Length > 0x75)
            {
                byte reg75 = dpcdBytes[0x75];

                // Example: if bits 0 or 2 are cleared, enable VRR (your original was (reg75 & 0x05) == 0)
                if ((reg75 & 0x05) == 0)
                {
                    info.VRR_Supported = true; // This might override previous VRR flag from reg70
                }
            }

            // Register 0x1A - Static HDR Metadata support (bit 6)
            if (dpcdBytes.Length > 0x1A)
            {
                byte reg1A = dpcdBytes[0x1A];
                info.HDR_Supported = (reg1A & 0x40) != 0; // Bit 6 indicates static HDR metadata support
            }

            // Register 0x01 - Max Link Rate (encoded)
            if (dpcdBytes.Length > 0x01)
            {
                byte maxLinkRate = dpcdBytes[0x01];
                switch (maxLinkRate)
                {
                    case 0x06: info.Data_Link_Rate = "1.6 Gbps"; break;
                    case 0x0A: info.Data_Link_Rate = "2.7 Gbps"; break;
                    case 0x14: info.Data_Link_Rate = "5.4 Gbps"; break;
                    case 0x1E: info.Data_Link_Rate = "8.1 Gbps"; break;
                    default: info.Data_Link_Rate = $"Unknown (0x{maxLinkRate:X2})"; break;
                }
            }

            // Register 0x02 - Lane Count
            if (dpcdBytes.Length > 0x02)
            {
                byte laneCount = dpcdBytes[0x02];
                info.Data_Lane_Count = laneCount.ToString();
            }

            // Registers 0x160 and 0x161 - DSC Major and Minor version
            if (dpcdBytes.Length > 0x161)
            {
                byte dscMajor = dpcdBytes[0x160];
                byte dscMinor = dpcdBytes[0x161];
                if (dscMajor != 0 || dscMinor != 0)
                {
                    info.VDSC_Version = $"{dscMajor}.{dscMinor}";
                }
            }

            //LRR2.5 support
            if(info.Intel_LRR_Version == "Yes")
            {
                if((dpcdBytes[0x314] & 0x06) == 0x06)
                {
                    // Both bit 1 and bit 2 are set
                    info.Intel_LRR_Version = "2.5";
                }
            }

            //EDP1.5 PR + ET:
            if(info.EdidVersion == "1.5")
            {
                if ((dpcdBytes[0x300] & 0x01) == 0x01)
                {
                    info.EDP1_5_Panel_Replay = "Yes";
                }
                if ((dpcdBytes[0x300] & 0x02) == 0x02)
                {
                    info.EDP1_5_PR_ET = "Yes";
                }

            }
            else
            {
                info.EDP1_5_Panel_Replay = "No";
                info.EDP1_5_PR_ET = "No";
            }

        }

    }
}