using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DaeMvvmFramework;
using HappyFxEditorBaseLib;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types
{
    public class IntType : UndoableChangeSource, IType
    {
        #region Value
        private int _value;
        public const string ValueProperty = "Value";
        public int Value
        {
            get { return _value; }
            set
            {
                int newValue = MathHelper.Clamp(value, Min, Max);
                Change(_value, value, (v) => Swap(ref _value, v, ValueProperty));
            }
        }
        #endregion

        public string Name { get; private set; }

        public int Min { get; private set; }
        public int Max { get; private set; }

        public int Increment { get { return 1; } }

        public IntType(Evolution evolution, int min = 0, int max = 100, int value = 0)
        {
            Value = value;
            Min = min;
            Max = max;
            _evolution = evolution;
        }

        private readonly Evolution _evolution;
        public override Evolution Evolution
        {
            get { return _evolution; }
        }

        public IType Copy()
        {
            return new IntType(_evolution, Min, Max, Value) { Name = this.Name };
        }

        public VariableComponentType GetVarType()
        {
            return VariableComponentType.Int;
        }

        public void SetName(string name)
        {
            Name = name;
        }
    }
}
