using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using static TranslateUI.DllTranslateApi;

namespace TranslateUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            InitDllTranslate();
            string sendString = "你好";
            string resString = null;


            IntPtr resPtr = Marshal.AllocHGlobal(1024);
            if (Translate(sendString, resPtr))
            {
                resString = Marshal.PtrToStringUni(resPtr);
                MessageBox.Show(resString);
            }
            else
            {
                MessageBox.Show("faile");
            }
            Marshal.FreeHGlobal(resPtr);
            ReleaseDllTranslate();
        }
    }
}
