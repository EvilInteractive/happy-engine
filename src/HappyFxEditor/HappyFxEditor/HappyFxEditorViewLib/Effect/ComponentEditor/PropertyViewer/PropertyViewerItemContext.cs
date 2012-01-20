using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using DaeMvvmFramework;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer
{
    public class PropertyViewerItemContext : UndoableChangeSource
    {
        public PropertyViewerGroupContext Group { get; private set; }

        public string Name { get; private set; }

        public List<IVariableContext> PossibleVars { get; private set; }

        #region Variable
        private IVariableContext _variable;
        public const string VariableProperty = "Variable";
        public IVariableContext Variable
        {
            get { return _variable; }
            set { Change(_variable, value, (newValue) => Swap(ref _variable, newValue, VariableProperty)); }
        }
        #endregion
        
        public PropertyViewerItemContext(PropertyViewerGroupContext group, string name, IEnumerable<IVariableContext> vars)
        {
            Group = group;
            Name = name;
            PossibleVars = new List<IVariableContext>(vars);
            Variable = PossibleVars[0];
        }

        public override Evolution Evolution
        {
            get { return Group.PropertyViewer.Effect.Evolution; }
        }

        internal PropertyViewerItemContext Copy(PropertyViewerGroupContext group)
        {
            PropertyViewerItemContext temp = new PropertyViewerItemContext(group, Name, PossibleVars);
            for (int i = 0; i < PossibleVars.Count; i++)
            {
                temp.PossibleVars[i] = PossibleVars[i].Copy(group.PropertyViewer.Effect);
                if (Variable == PossibleVars[i])
                    temp.Variable = temp.PossibleVars[i];
            }
            return temp;
        }

        internal void Serialize(BinaryWriter stream)
        {
            stream.Write((UInt32)Variable.GetVarType());
            Variable.Serialize(stream);
        }

        internal void DeSerialize(BinaryReader stream)
        {
            VariableType varType = (VariableType)stream.ReadUInt32();
            foreach (var possibleVar in PossibleVars)
            {
                if (possibleVar.GetVarType() == varType)
                {
                    possibleVar.DeSerialize(stream);
                    Variable = possibleVar;
                    break;
                }
            }
        }
    }
}
