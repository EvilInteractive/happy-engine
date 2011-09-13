using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace HappyCookerGUI
{
    class HappyCooker
    {
        [DllImport("HappyCookerDll.dll")]
        private static extern bool initHappyCooker();

        [DllImport("HappyCookerDll.dll")]
        private static extern void disposeHappyCooker();

        [DllImport("HappyCookerDll.dll", CharSet=CharSet.Auto, CallingConvention=CallingConvention.StdCall)]
        private static extern bool cookObjToBinObj([MarshalAs(UnmanagedType.LPStr)]string inputPath, 
                                                   [MarshalAs(UnmanagedType.LPStr)]string outputPath);

        [DllImport("HappyCookerDll.dll", CharSet=CharSet.Auto, CallingConvention=CallingConvention.StdCall)]
        private static extern bool cookObjLineToBinObj([MarshalAs(UnmanagedType.LPStr)]string inputPath, 
                                                       [MarshalAs(UnmanagedType.LPStr)]string outputPath);

        [DllImport("HappyCookerDll.dll", CharSet=CharSet.Auto, CallingConvention=CallingConvention.StdCall)]
        private static extern bool cookObjToConvex([MarshalAs(UnmanagedType.LPStr)]string inputPath, 
                                                   [MarshalAs(UnmanagedType.LPStr)]string outputPath);

        //private static extern void pollProgress();

        public HappyCooker()
        {
            try
            {
                initHappyCooker();
            }
            catch (DllNotFoundException e)
            {
                MessageBox.Show(e.Message);
            }
        }

        public void Dispose()
        {
            disposeHappyCooker();
        }

        public bool CookObjToConvex(string inputPath, string outputPath)
        {
            try
            {
                bool success = cookObjToConvex(inputPath, outputPath);
                return success;
            }
            catch (SEHException)
            {
                
            }
            return false;
        }
        public bool CookObjToBinObj(string inputPath, string outputPath)
        {
            try
            {
                bool success = cookObjToBinObj(inputPath, outputPath);
                return success;
            }
            catch (SEHException)
            {
                
            }
            return false;
        }
        public bool CookObjLineToBinObj(string inputPath, string outputPath)
        {
            try
            {
                bool success = cookObjLineToBinObj(inputPath, outputPath);
                return success;
            }
            catch (SEHException)
            {
                
            }
            return false;
        }
    }
}
