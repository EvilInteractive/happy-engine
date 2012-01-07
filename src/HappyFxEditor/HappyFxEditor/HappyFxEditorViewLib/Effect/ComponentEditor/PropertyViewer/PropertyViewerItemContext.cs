using System;
using System.Collections.Generic;
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

        #region Variable
        private IVariableContext _variable;
        public const string VariableProperty = "Variable";
        public IVariableContext Variable
        {
            get { return _variable; }
            set { Change(_variable, value, (newValue) => Swap(ref _variable, newValue, VariableProperty)); }
        }
        #endregion

        public VariableComponentType VariableComponentType { get; private set; }
        public VariableComponentCount VariableComponentCount { get; private set; }

        public PropertyViewerItemContext(PropertyViewerGroupContext group, string name, VariableComponentType type, VariableComponentCount count)
        {
            Group = group;
            Name = name;
            VariableComponentType = type;
            VariableComponentCount = count;
            switch (type)
            {
                case VariableComponentType.Int:
                    Variable = new ConstantVarContext(count, new IntType(group.PropertyViewer.Effect.Evolution));
                    break;
                case VariableComponentType.Float:
                    Variable = new ConstantVarContext(count, new FloatType(group.PropertyViewer.Effect.Evolution));
                    break;
                default:
                    throw new ArgumentOutOfRangeException("type");
            }
        }

        public override Evolution Evolution
        {
            get { return Group.PropertyViewer.Effect.Evolution; }
        }

        internal PropertyViewerItemContext Copy(PropertyViewerGroupContext group)
        {
            PropertyViewerItemContext temp = new PropertyViewerItemContext(group, Name, VariableComponentType, VariableComponentCount);
            temp.Variable = Variable.Copy();
            return temp;
        }
    }
}
