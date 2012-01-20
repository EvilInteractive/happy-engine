using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HappyFxEditorContextLib.Browsers
{
    public enum AssetType
    {
        Audio,
        Texture
    }
    public class Asset
    {
        public string Name { get; private set; }
        public AssetType Type { get; private set; }

        public Folder Parent { get; private set; }

        public Asset(Folder parent, string name, AssetType type)
        {
            Parent = parent;
            Name = name;
            Type = type;
        }

        public string Path
        {
            get
            {
                if (Parent != null)
                    return Parent.Path + Name;
                else
                    return Name;
            }
        }
    }
}
