using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows;
using Microsoft.Win32;

namespace HappyFxEditorContextLib
{
    static class EffectSaveLoadDialogProvider
    {
        private static string s_LastSavePath = Directory.GetCurrentDirectory();
        private static string s_LastOpenPath = Directory.GetCurrentDirectory();

        private const string FILTER = "HappyFx files|*.hfx|All files|*.*";

        public static string GetSavePath(string saveHint)
        {
            SaveFileDialog dialog = new SaveFileDialog
                                        {
                                            InitialDirectory = Path.GetDirectoryName(string.IsNullOrEmpty(saveHint) ? s_LastSavePath : saveHint),
                                            FileName = Path.GetFileName(string.IsNullOrEmpty(saveHint) ? s_LastSavePath : saveHint),
                                            Filter = FILTER
                                        };

            if (dialog.ShowDialog(Application.Current.MainWindow) == true)
            {
                s_LastSavePath = dialog.FileName;
                return s_LastSavePath;
            }
            return null;
        }
        public static string GetOpenPath(string openHint)
        {
            OpenFileDialog dialog = new OpenFileDialog
            {
                InitialDirectory = Path.GetDirectoryName(string.IsNullOrEmpty(openHint) ? s_LastOpenPath : openHint),
                FileName = Path.GetFileName(string.IsNullOrEmpty(openHint) ? s_LastOpenPath : openHint),
                Filter = FILTER
            };

            if (dialog.ShowDialog(Application.Current.MainWindow) == true)
            {
                s_LastOpenPath = dialog.FileName;
                return s_LastOpenPath;
            }
            return null;
        }
    }
}
