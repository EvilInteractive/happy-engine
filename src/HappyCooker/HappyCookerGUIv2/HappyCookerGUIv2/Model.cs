using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;
using Microsoft.Win32;
using System.Windows.Forms;
using System.ComponentModel;
using System.IO;
using System.Threading;
using System.Security.AccessControl;

namespace HappyCookerGUIv2
{
    class ModelItem
    {
        public string ShortPath { get; private set; }
        public string Extension { get; private set; }
        public DateTime LastWrite { get; private set; }

        public int ProcessedItems { get; private set; }

        public ModelItem(string shortPath, string extension, DateTime lastWrite)
        {
            ShortPath = shortPath;
            LastWrite = lastWrite;
            Extension = extension;
        }

        public override bool Equals(object obj)
        {
            ModelItem item = obj as ModelItem;
            if (item != null)
            {
                return item.ShortPath == ShortPath;
            }
            else
            {
                return false;
            }
        }
        public override int GetHashCode()
        {
            return ShortPath.GetHashCode();
        }
        public override string ToString()
        {
            return ShortPath;
        }
    }
    class Model : INotifyPropertyChanged
    {
        public ObservableCollection<ModelItem> ImportItems { get; private set; }
        public ObservableCollection<ModelItem> UpdateItems { get; private set; }
        public ObservableCollection<ModelItem> ExportItems { get; private set; }
        public ObservableCollection<string> OutputInfo { get; private set; }

        private string _importPath; //D:\File Server\Programming\happy-engine\art\models\export
        public string ImportPath 
        {
            get { return _importPath;  }
            set
            {
                if (_importPath != value)
                {
                    _importPath = value;
                    if (PropertyChanged != null)
                        PropertyChanged(this, new PropertyChangedEventArgs("ImportPath"));
                }
            }
        }
        private string _exportPath;
        public string ExportPath
        {
            get { return _exportPath; }
            set
            {
                if (_exportPath != value)
                {
                    _exportPath = value;
                    if (PropertyChanged != null)
                        PropertyChanged(this, new PropertyChangedEventArgs("ExportPath"));
                }
            }
        }

        private FolderBrowserDialog _OpenFolderImportDialog = new FolderBrowserDialog();
        private FolderBrowserDialog _OpenFolderExportDialog = new FolderBrowserDialog();

        private HashSet<string> _ImportExtensions = new HashSet<string>() { ".dae" };
        private string _exportExtension = ".binobj";

        int _meshesToCook = 1;
        int _cookedMeshes = 0;
        public int MeshesToCook
        {
            get { return _meshesToCook; }
            set
            {
                if (_meshesToCook != value)
                {
                    _meshesToCook = value;
                    if (PropertyChanged != null)
                        PropertyChanged(this, new PropertyChangedEventArgs("MeshesToCook"));
                }
            }
        }
        public int CookedMeshes
        {
            get { return _cookedMeshes; }
            set
            {
                if (_cookedMeshes != value)
                {
                    _cookedMeshes = value;
                    if (PropertyChanged != null)
                        PropertyChanged(this, new PropertyChangedEventArgs("CookedMeshes"));
                }
            }
        }
        string _info = "";
        public string Info
        {
            get { return _info; }
            set
            {
                if (_info != value)
                {
                    _info = value;
                    if (PropertyChanged != null)
                        PropertyChanged(this, new PropertyChangedEventArgs("Info"));
                }
            }
        }

        public Model()
        {
            ImportItems = new ObservableCollection<ModelItem>();
            UpdateItems = new ObservableCollection<ModelItem>();
            ExportItems = new ObservableCollection<ModelItem>();
            OutputInfo = new ObservableCollection<string>();

            ImportPath = "";
            
            _OpenFolderImportDialog.SelectedPath = ImportPath;
            _OpenFolderImportDialog.ShowNewFolderButton = true;
            _OpenFolderImportDialog.Description = "Set the import directory";

            _OpenFolderImportDialog.SelectedPath = ExportPath;
            _OpenFolderImportDialog.ShowNewFolderButton = true;
            _OpenFolderImportDialog.Description = "Set the export directory";

            HappyCooker cooker = new HappyCooker(); //check for dll
            cooker.Dispose();
        }

        public void GetImportPath()
        {
            _OpenFolderImportDialog.SelectedPath = ImportPath;
            if (_OpenFolderImportDialog.ShowDialog() != DialogResult.Cancel)
            {
                ImportPath = _OpenFolderImportDialog.SelectedPath;
            }
            Refresh();
        }
        public void GetExportPath()
        {
            _OpenFolderExportDialog.SelectedPath = ExportPath;
            if (_OpenFolderExportDialog.ShowDialog() != DialogResult.Cancel)
            {
                ExportPath = _OpenFolderExportDialog.SelectedPath;
            }
            Refresh();
        }

        public void Refresh()
        {
            ImportItems.Clear();
            try
            {
                DirectoryInfo dirInfo = new DirectoryInfo(ImportPath);
                IEnumerable<FileInfo> files = dirInfo.EnumerateFiles("*", SearchOption.AllDirectories).Where(f => _ImportExtensions.Contains(f.Extension));
                foreach (FileInfo file in files)
                {
                    ModelItem item = new ModelItem(file.FullName.Substring(ImportPath.Length, file.FullName.Length - ImportPath.Length - file.Extension.Length), file.Extension, file.LastWriteTime);
                    ImportItems.Add(item);
                }
            }
            catch (Exception) { ImportItems.Add(new ModelItem("Error while trying to read files", "", new DateTime())); }
            ImportItems.OrderBy(item => item.ShortPath);

            ExportItems.Clear();
            try
            {
                IEnumerable<string> files = Directory.EnumerateFiles(ExportPath, "*"+_exportExtension, SearchOption.AllDirectories);
                foreach (string file in files)
                {
                    FileInfo info = new FileInfo(file);
                    ModelItem item = new ModelItem(info.FullName.Substring(ExportPath.Length, info.FullName.Length - ExportPath.Length - info.Extension.Length), info.Extension, File.GetLastWriteTime(file));
                    ExportItems.Add(item);
                }
            }
            catch (Exception) { ExportItems.Add(new ModelItem("Error while trying to read files", "", new DateTime())); }

            UpdateItems.Clear();
            List<ModelItem> tobeRemovedItems = new List<ModelItem>();
            foreach (ModelItem file in ImportItems)
            {
                if (ExportItems.Contains(file))
                {
                    tobeRemovedItems.Add(file);
                    int otherIndex = ExportItems.IndexOf(file);
                    ModelItem other = ExportItems[otherIndex];
                    if (file.LastWrite > other.LastWrite)
                    {
                        UpdateItems.Add(file);
                        ExportItems.RemoveAt(otherIndex);
                    }                  
                }
            }
            foreach (ModelItem m in tobeRemovedItems)
            {
                ImportItems.Remove(m);             
            }
        }

        private void CookerCallback(string info)
        {
            OutputInfo.Add(info);
            Application.DoEvents();
        }
        public void Cook()
        {
            HappyCooker cooker = new HappyCooker();
            OutputInfo.Clear();
            cooker.GetInfo += CookerCallback;

            MeshesToCook = UpdateItems.Count;
            CookedMeshes = 0;
            Application.DoEvents();
            foreach (ModelItem item in UpdateItems)
            {
                Info = string.Format("cooking '{0}' - ({1}/{2})", item.ShortPath, CookedMeshes + 1, MeshesToCook);
                string input = ImportPath + item.ShortPath + item.Extension;
                string output = ExportPath + item.ShortPath + _exportExtension;
                //if (File.Exists(output) == false)
                //    File.Create(output);
                cooker.CookToBinObj(input, output);
                ++CookedMeshes;
                Application.DoEvents();
            }

            cooker.Dispose();
            OutputInfo.Add("----------------");
            OutputInfo.Add("Done");
            OutputInfo.Add("----------------");
            Info = "Done!";

            Refresh();
        }

        public event PropertyChangedEventHandler PropertyChanged;

        internal void Serialize(StreamWriter stream)
        {
            stream.WriteLine(_importPath);
            stream.WriteLine(_exportPath);
        }
        internal void Deserialize(StreamReader stream)
        {
            ImportPath = stream.ReadLine();
            ExportPath = stream.ReadLine();
            Refresh();
        }
    }
}
