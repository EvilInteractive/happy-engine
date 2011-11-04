using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace HappyCookerGUIv2
{
    class HappyCooker
    {
        [DllImport("HappyCookerDll.dll", CharSet=CharSet.Auto, CallingConvention=CallingConvention.StdCall)]
        private static extern bool initHappyCooker();

        [DllImport("HappyCookerDll.dll", CharSet=CharSet.Auto, CallingConvention=CallingConvention.StdCall)]
        private static extern void disposeHappyCooker();

        [DllImport("HappyCookerDll.dll", CharSet=CharSet.Auto, CallingConvention=CallingConvention.StdCall)]
        private static extern bool cookToBinObj([MarshalAs(UnmanagedType.LPStr)]string inputPath, 
                                                   [MarshalAs(UnmanagedType.LPStr)]string outputPath);

        [DllImport("HappyCookerDll.dll", CharSet=CharSet.Auto, CallingConvention=CallingConvention.StdCall)]
        private static extern bool cookLineToBinObj([MarshalAs(UnmanagedType.LPStr)]string inputPath, 
                                                       [MarshalAs(UnmanagedType.LPStr)]string outputPath);

        [DllImport("HappyCookerDll.dll", CharSet=CharSet.Auto, CallingConvention=CallingConvention.StdCall)]
        private static extern bool cookToConvex([MarshalAs(UnmanagedType.LPStr)]string inputPath, 
                                                   [MarshalAs(UnmanagedType.LPStr)]string outputPath);


        private delegate bool GetInfoDelegateDll([MarshalAs(UnmanagedType.LPStr)]string info);
        [DllImport("HappyCookerDll.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
        private static extern void setInfoCallback(GetInfoDelegateDll callback);
        
        private bool getInfo([MarshalAs(UnmanagedType.LPStr)]string info)
        {
            if (GetInfo != null)
                GetInfo(info);
            return true;
        }
        public delegate void GetInfoDelegate(string info);
        public event GetInfoDelegate GetInfo;

        private GetInfoDelegateDll _infoCallback;
        public HappyCooker()
        {
            try
            {
                initHappyCooker();
                _infoCallback = new GetInfoDelegateDll(getInfo);
                setInfoCallback(_infoCallback);
            }
            catch (DllNotFoundException e)
            {
                MessageBox.Show(e.Message);
                Application.Exit();
            }
        }

        public void Dispose()
        {
            disposeHappyCooker();
        }

        public bool CookToConvex(string inputPath, string outputPath)
        {
            try
            {
                bool success = cookToConvex(inputPath, outputPath);
                return success;
            }
            catch (SEHException)
            {
                
            }
            return false;
        }
        public bool CookToBinObj(string inputPath, string outputPath)
        {
            try
            {
                bool success = cookToBinObj(inputPath, outputPath);
                return success;
            }
            catch (SEHException)
            {
                
            }
            return false;
        }
        public bool CookLineToBinObj(string inputPath, string outputPath)
        {
            try
            {
                bool success = cookLineToBinObj(inputPath, outputPath);
                return success;
            }
            catch (SEHException)
            {
                
            }
            return false;
        }
    }
}
