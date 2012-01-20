using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Input;
using System.Windows.Threading;
using DaeMvvmFramework;
using HappyFxEditorBaseLib.Net;

namespace HappyFxEditorContextLib.Browsers
{
    public class AssetManagerContext : PropertyChangeSource
    {
        public ObservableCollection<object> TextureAssets { get; private set; }
        public ObservableCollection<object> AudioAssets { get; private set; }

        private string _texturePath;
        private string _audioPath;
        
        public ICommand RefreshCommand { get; private set; }

        #region SelectedAsset
        private Asset _selectedAsset;
        public const string SelectedAssetProperty = "SelectedAsset";
        public Asset SelectedAsset
        {
            get { return _selectedAsset; }
            set { Change(ref _selectedAsset, value, SelectedAssetProperty); }
        }
        #endregion
        

        private readonly Dispatcher _dispatcher;

        public AssetManagerContext()
        {
            TextureAssets = new ObservableCollection<object>();
            RefreshCommand = CommandFactory.Create(Refresh);
            _dispatcher = Dispatcher.FromThread(Thread.CurrentThread);
            HeConnectionManager.Instance.PacketReceived += (h, p) =>
            {
                if (h == FxInHeader.SetTexturePath)
                {
                    _texturePath = p.ReadString();
                    _dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(FillTextureList));
                }
                else if (h == FxInHeader.SetAudioPath)
                {
                    _audioPath = p.ReadString();
                }
            };
        }

        private void FillTextureList()
        {
            TextureAssets.Clear();
            FillTextureList(_texturePath, null);
        }
        private void FillTextureList(string path, Folder folder)
        {
            IEnumerable<string> files = Directory.EnumerateFiles(path);
            foreach (var file in files)
            {
                if (folder == null)
                    TextureAssets.Add(new Asset(null, Path.GetFileName(file), AssetType.Texture));
                else
                    folder.Childeren.Add(new Asset(folder, Path.GetFileName(file), AssetType.Texture));
            }

            IEnumerable<string> dirs = Directory.EnumerateDirectories(path);
            foreach (string dir in dirs)
            {
                Folder f = new Folder(folder, Path.GetFileName(dir));
                FillTextureList(dir, f);
                if (folder != null)
                    folder.Childeren.Add(f);
                else
                    TextureAssets.Add(f);
            }
        }

        private void FillAudioList()
        {
            
        }

        public void Refresh()
        {
            HeConnectionManager.Instance.SendPacket(new Packet(FxOutHeader.GetTexturePath));
            HeConnectionManager.Instance.SendPacket(new Packet(FxOutHeader.GetAudioPath));
        }
    }
}
