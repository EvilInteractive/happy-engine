using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types
{
    interface IType
    {
        IType Copy();

        VariableComponentType GetVarType();

        void SetName(string name);
    }
}
