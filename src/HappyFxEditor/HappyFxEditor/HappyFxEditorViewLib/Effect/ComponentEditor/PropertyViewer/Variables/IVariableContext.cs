using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables
{
    public enum VariableType
    {
        Curve,
        Random,
        Constant
    }
    public enum VariableComponentCount
    {
        X,
        Xy,
        Xyz,
        Rgba
    }
    public enum VariableComponentType
    {
        Int,
        Float
    }

    public interface IVariableContext
    {
        VariableType GetVarType();
        VariableComponentType GetVarComponentType();
        VariableComponentCount GetComponentCount();

        IVariableContext Copy();
    }
}
