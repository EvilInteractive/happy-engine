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
        private static extern bool cook([MarshalAs(UnmanagedType.LPStr)]string inputFilename,
                                        [MarshalAs(UnmanagedType.LPStr)]string outputName);

        [DllImport("HappyCookerDll.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
        private static extern void setImportPath([MarshalAs(UnmanagedType.LPStr)]string path);
        [DllImport("HappyCookerDll.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
        private static extern void setModelExportPath([MarshalAs(UnmanagedType.LPStr)]string path);
        [DllImport("HappyCookerDll.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
        private static extern void setPhysicsExportPath([MarshalAs(UnmanagedType.LPStr)]string path);

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
        }

        public void Init()
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
            catch (SEHException e)
            {
                MessageBox.Show(e.Message);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                Application.Exit();
            }          
        }

        public void Dispose()
        {
            disposeHappyCooker();
        }

        public bool Cook(string inputFileName, string outputName)
        {
            try
            {
                bool success = cook(inputFileName, outputName);
                return success;
            }
            catch (SEHException)
            {
                
            }
            return false;
        }

        public void SetImportPath(string path)
        {
            try
            {
                setImportPath(path);
            }
            catch (SEHException)
            {

            }
        }
        public void SetModelExportPath(string path)
        {
            try
            {
                setModelExportPath(path);
            }
            catch (SEHException)
            {

            }
        }
        public void SetPhysicsExportPath(string path)
        {
            try
            {
                setPhysicsExportPath(path);
            }
            catch (SEHException)
            {

            }
        }

    }
}
