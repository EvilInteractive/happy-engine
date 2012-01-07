using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using DaeMvvmFramework;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables
{
    class ConstantVarContext : PropertyChangeSource, IVariableContext
    {
        #region Value
        private List<IType> _value;
        public const string ValueProperty = "Value";
        public List<IType> Value
        {
            get { return _value; }
            set { Change(ref _value, value, ValueProperty); }
        }
        #endregion

        public VariableComponentCount VariableComponentCount { get; private set; }
        public VariableComponentType VariableComponentType { get; private set; }

        public ConstantVarContext(VariableComponentCount components, IType defaultValue)
        {
            VariableComponentCount = components;
            Value = new List<IType>((int)components + 1);
            for (int i = 0; i < (int)components + 1; i++)
            {
                Value.Add(defaultValue.Copy());
                Value[i].SetName((int) components + 1 == 4 ? new string[] {"R", "G", "B", "A"}[i]:
                                 new string[] {"X", "Y", "Z"}[i]);
            }

            VariableComponentType = defaultValue.GetVarType();
        }


        public VariableType GetVarType()
        {
            return VariableType.Constant;
        }
        public VariableComponentType GetVarComponentType()
        {
            return VariableComponentType;
        }
        public VariableComponentCount GetComponentCount()
        {
            return VariableComponentCount;
        }

        public IVariableContext Copy()
        {
            ConstantVarContext temp = new ConstantVarContext(VariableComponentCount, Value[0]);

            for (int i = 0; i < Value.Count; ++i)
            {
                temp.Value[i] = Value[i].Copy();
            }

            return temp;
        }
    }
}
