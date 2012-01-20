using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HappyFxEditorContextLib.Browsers
{
    public class Folder
    {
        public string Name { get; private set; }

        public Folder Parent { get; private set; }

        public List<object> Childeren { get; private set; }

        public Folder(Folder parent, string name)
        {
            Name = name;
            Parent = parent;
            Childeren = new List<object>();
        }

        public string Path
        {
            get
            {
                string path;
                if (Parent != null)
                    return Parent.Path + Name + "/";
                else
                    return Name + "/";
            }
        }
    }
}
