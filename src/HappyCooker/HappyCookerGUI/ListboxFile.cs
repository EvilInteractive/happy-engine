using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HappyCookerGUI
{
    class ListboxFile
    {
        public string fullPath;
        public string relativePath;

        public override string ToString()
        {
            return relativePath;
        }
    }
}
