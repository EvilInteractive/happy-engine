using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables
{
    public enum VariableType
    {
        Curve,
        Random,
        Constant,
        Asset,
        Choose
    }
    public enum VariableComponentType
    {
        Int,
        Float,
        Texture,
        Enum
    }

    public interface IVariableContext
    {
        VariableType GetVarType();

        IVariableContext Copy(EffectContext effect);

        void Serialize(BinaryWriter stream);

        void DeSerialize(BinaryReader stream);
    }
}
