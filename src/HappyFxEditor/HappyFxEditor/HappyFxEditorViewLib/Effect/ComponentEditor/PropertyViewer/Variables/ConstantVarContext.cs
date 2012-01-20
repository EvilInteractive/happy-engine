using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using DaeMvvmFramework;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables
{
    public class ConstantVarContext : PropertyChangeSource, IVariableContext
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

        public VariableType VariableType { get { return VariableType.Constant; } }

        public ConstantVarContext(EffectContext effect, List<IType> var)
        {
            Value = new List<IType>(var.Count);
            foreach (var val in var)
            {
                Value.Add(val.Copy());
            }
        }


        public VariableType GetVarType()
        {
            return VariableType;
        }

        public IVariableContext Copy(EffectContext effect)
        {
            ConstantVarContext temp = new ConstantVarContext(effect, Value);
            return temp;
        }

        public override string ToString()
        {
            return "Constant";
        }


        public void Serialize(BinaryWriter stream)
        {
            foreach (var value in Value)
            {
                value.Serialize(stream);
            }
        }
        public void DeSerialize(BinaryReader stream)
        {
            foreach (var value in Value)
            {
                value.DeSerialize(stream);
            }
        }
    }
}
