using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using DaeMvvmFramework;
using HappyFxEditorBaseLib;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types
{
    public class FloatType : UndoableChangeSource, IType
    {
        #region Value
        private float _value;
        public const string ValueProperty = "Value";
        public float Value
        {
            get { return _value; }
            set
            {
                float newValue = MathHelper.Clamp(value, Min, Max);
                Change(_value, newValue, (v) => Swap(ref _value, v, ValueProperty));
            }
        }
        #endregion

        public string Name { get; set; }

        public float Min { get; set; }
        public float Max { get; set; }

        public float Increment { get { return (Max - Min > 10.0f)? 1.0f : 0.01f; } }

        public FloatType(Evolution evolution, float min = 0.0f, float max = 1.0f, float value = 0.0f)
        {
            Min = min;
            Max = max;
            Value = value;
            _evolution = evolution;
        }

        private readonly Evolution _evolution;
        public override Evolution Evolution
        {
            get { return _evolution; }
        }
        
        public IType Copy()
        {
            return new FloatType(_evolution, Min, Max, Value) {Name = this.Name};
        }

        public VariableComponentType GetVarType()
        {
            return VariableComponentType.Float;
        }

        public void Serialize(BinaryWriter stream)
        {
            stream.Write(Value);
        }

        public void DeSerialize(BinaryReader stream)
        {
            Value = stream.ReadSingle();
        }
    }
}
