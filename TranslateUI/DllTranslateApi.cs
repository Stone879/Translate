using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TranslateUI
{
    public class DllTranslateApi
    {
        [DllImport("DllTranslate.dll")]
        public static extern void InitDllTranslate();

        [DllImport("DllTranslate.dll")]
        public static extern void ReleaseDllTranslate();

        [DllImport("DllTranslate.dll", CharSet = CharSet.Unicode)]
        public static extern bool Translate(string inputString, IntPtr outputString);
    }
}
