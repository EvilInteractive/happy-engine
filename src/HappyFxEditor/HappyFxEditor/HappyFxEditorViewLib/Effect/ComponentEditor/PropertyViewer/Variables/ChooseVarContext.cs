using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using DaeMvvmFramework;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables
{
    class ChooseVarContext : PropertyChangeSource, IVariableContext
    {
        public List<EnumType> Possibilities { get; set; }

        #region Variable
        private EnumType _variable;
        public const string VariableProperty = "Variable";
        public EnumType Variable
        {
            get { return _variable; }
            set { Change(ref _variable, value, VariableProperty); }
        }
        #endregion
        

        public ChooseVarContext(List<EnumType> possibilities)
        {
            Possibilities = possibilities;
            Variable = Possibilities[0];
        }

        public VariableType GetVarType()
        {
            return VariableType.Choose;
        }

        public IVariableContext Copy(EffectContext effect)
        {
            return new ChooseVarContext(Possibilities) { Variable = this.Variable }; //we don't take a copy because not necessary
        }

        public void Serialize(BinaryWriter stream)
        {
            stream.Write((UInt32)Variable.Value.Value);
        }

        public void DeSerialize(BinaryReader stream)
        {
            uint value = stream.ReadUInt32();
            foreach (var possibility in Possibilities)
            {
                if (possibility.Value.Value == value)
                {
                    Variable = possibility;
                    break;
                }
            }
        }
    }
}
