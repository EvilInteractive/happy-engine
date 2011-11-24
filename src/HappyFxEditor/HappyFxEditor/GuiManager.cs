using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace HappyFxEditor
{
    class GuiManager : INotifyPropertyChanged
    {
        private string _effectPath, _texturePath, _modelPath, _audioPath;
        public string EffectPath 
        {
            get { return _effectPath; } 
            set
            {
                _effectPath = value;
                rebuildEffectBrowser();
            }
        }
        public string TexturePath
        {
            get { return _texturePath; }
            set
            {
                _texturePath = value;
                rebuildFileBrowser();
            }
        }
        public string ModelPath
        {
            get { return _modelPath; }
            set
            {
                _modelPath = value;
                rebuildFileBrowser();
            }
        }
        public string AudioPath
        {
            get { return _audioPath; }
            set
            {
                _audioPath = value;
                rebuildFileBrowser();
            }
        }

        private void rebuildFileBrowser()
        {

        }
        private void rebuildEffectBrowser()
        {

        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
