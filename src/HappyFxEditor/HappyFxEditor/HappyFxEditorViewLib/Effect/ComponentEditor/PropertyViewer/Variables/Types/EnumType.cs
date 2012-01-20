using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using DaeMvvmFramework;
using HappyFxEditorBaseLib;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types
{
    public struct EnumTypeDesc
    {
        private string _name;
        public string Name
        {
            get { return _name; }
            private set { _name = value; }
        }

        private uint _value;
        public uint Value
        {
            get { return _value; }
            private set { _value = value; }
        }

        public EnumTypeDesc(string name, uint value)
        {
            _name = name;
            _value = value;
        }
    }
    public class EnumType : PropertyChangeSource, IType
    {
        #region Value
        private EnumTypeDesc _value;
        public const string ValueProperty = "Value";
        public EnumTypeDesc Value
        {
            get { return _value; }
            set { Change(ref _value, value, ValueProperty); }
        }
        #endregion

        #region Name
        private string _name;
        public const string NameProperty = "Name";
        public string Name
        {
            get { return _name; }
            set { Change(ref _name, value, NameProperty); }
        }
        #endregion
        

        public EnumType(EnumTypeDesc value )
        {
            Value = value;
        }
        
        public IType Copy()
        {
            return new EnumType(Value);
        }

        public VariableComponentType GetVarType()
        {
            return VariableComponentType.Enum;
        }

        public void SetName(string name)
        {
            Name = name;
        }

        public void Serialize(BinaryWriter stream)
        {
        }
        public void DeSerialize(BinaryReader stream)
        {
        }

        public override string ToString()
        {
            return Value.Name;
        }
    }
}
