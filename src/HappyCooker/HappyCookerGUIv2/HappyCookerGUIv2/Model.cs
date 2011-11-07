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
        private string _modelExportPath;
        public string ModelExportPath
        {
            get { return _modelExportPath; }
            set
            {
                if (_modelExportPath != value)
                {
                    _modelExportPath = value;
                    if (PropertyChanged != null)
                        PropertyChanged(this, new PropertyChangedEventArgs("ModelExportPath"));
                }
            }
        }
        private string _physicsExportPath;
        public string PhysicsExportPath
        {
            get { return _physicsExportPath; }
            set
            {
                if (_physicsExportPath != value)
                {
                    _physicsExportPath = value;
                    if (PropertyChanged != null)
                        PropertyChanged(this, new PropertyChangedEventArgs("PhysicsExportPath"));
                }
            }
        }

        private FolderBrowserDialog _OpenFolderImportDialog = new FolderBrowserDialog();
        private FolderBrowserDialog _OpenFolderExportDialog = new FolderBrowserDialog();

        private HashSet<string> _importExtensions = new HashSet<string>() { ".dae" };
        public HashSet<string> ImportExtensions { get { return _importExtensions; } }
        private HashSet<string> _modelExportExtensions = new HashSet<string>() { ".binobj" };
        public HashSet<string> ModelExportExtensions { get { return _modelExportExtensions; } }
        private HashSet<string> _physicsExportExtensions = new HashSet<string>() { ".pxcv", ".pxcc" };
        public HashSet<string> PhysicsExportExtensions { get { return _physicsExportExtensions; } }

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
            
            _OpenFolderImportDialog.ShowNewFolderButton = true;
            _OpenFolderImportDialog.Description = "Set the import directory";

            _OpenFolderExportDialog.ShowNewFolderButton = true;
            _OpenFolderExportDialog.Description = "Set the export directory";

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
        public void GetModelExportPath()
        {
            _OpenFolderExportDialog.SelectedPath = ModelExportPath;
            if (_OpenFolderExportDialog.ShowDialog() != DialogResult.Cancel)
            {
                ModelExportPath = _OpenFolderExportDialog.SelectedPath;
            }
            Refresh();
        }
        public void GetPhysicsExportPath()
        {
            _OpenFolderExportDialog.SelectedPath = PhysicsExportPath;
            if (_OpenFolderExportDialog.ShowDialog() != DialogResult.Cancel)
            {
                PhysicsExportPath = _OpenFolderExportDialog.SelectedPath;
            }
            Refresh();
        }

        public void Refresh()
        {
            ImportItems.Clear();
            try
            {
                DirectoryInfo dirInfo = new DirectoryInfo(ImportPath);
                IEnumerable<FileInfo> files = dirInfo.EnumerateFiles("*", SearchOption.AllDirectories).Where(f => _importExtensions.Contains(f.Extension));
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
                DirectoryInfo dirInfo = new DirectoryInfo(ModelExportPath);
                IEnumerable<FileInfo> files = dirInfo.EnumerateFiles("*", SearchOption.AllDirectories).Where(f => _modelExportExtensions.Contains(f.Extension));
                foreach (FileInfo info in files)
                {
                    ModelItem item = new ModelItem(info.FullName.Substring(ModelExportPath.Length, info.FullName.Length - ModelExportPath.Length - info.Extension.Length), info.Extension, info.LastWriteTime);
                    ExportItems.Add(item);
                }
            }
            catch (Exception) { ExportItems.Add(new ModelItem("Error while trying to read files", "", new DateTime())); }
            try
            {
                DirectoryInfo dirInfo = new DirectoryInfo(PhysicsExportPath);
                IEnumerable<FileInfo> files = dirInfo.EnumerateFiles("*", SearchOption.AllDirectories).Where(f => _physicsExportExtensions.Contains(f.Extension));
                foreach (FileInfo info in files)
                {
                    ModelItem item = new ModelItem(info.FullName.Substring(PhysicsExportPath.Length, info.FullName.Length - PhysicsExportPath.Length - info.Extension.Length), info.Extension, info.LastWriteTime);
                    if (ExportItems.Contains(item) == false)
                        ExportItems.Add(item);
                }
            }
            catch (Exception) { ExportItems.Clear(); ExportItems.Add(new ModelItem("Error while trying to read files", "", new DateTime())); }

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
            cooker.SetImportPath(ImportPath);
            cooker.SetModelExportPath(ModelExportPath);
            cooker.SetPhysicsExportPath(PhysicsExportPath);

            MeshesToCook = UpdateItems.Count;
            CookedMeshes = 0;
            Application.DoEvents();
            foreach (ModelItem item in UpdateItems)
            {
                Info = string.Format("cooking '{0}' - ({1}/{2})", item.ShortPath, CookedMeshes + 1, MeshesToCook);
                string input = item.ShortPath + item.Extension;
                string output = item.ShortPath;
                //if (File.Exists(output) == false)
                //    File.Create(output);
                cooker.Cook(input, output);
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
            stream.WriteLine(_modelExportPath);
            stream.WriteLine(_physicsExportPath);
        }
        internal void Deserialize(StreamReader stream)
        {
            if (stream != null)
            {
                ImportPath = stream.ReadLine();
                ModelExportPath = stream.ReadLine();
                PhysicsExportPath = stream.ReadLine();
            }
            Refresh();
        }
    }
}
