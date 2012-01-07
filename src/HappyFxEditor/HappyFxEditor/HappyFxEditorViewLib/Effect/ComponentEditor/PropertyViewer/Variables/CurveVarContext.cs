using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables
{
    class CurveVarContext : IVariableContext
    {
        public VariableType GetVarType()
        {
            return VariableType.Curve;
        }

        public VariableComponentType GetVarComponentType()
        {
            throw new NotImplementedException();
        }

        public VariableComponentCount GetComponentCount()
        {
            throw new NotImplementedException();
        }

        public IVariableContext Copy()
        {
            throw new NotImplementedException();
        }

        public Type GetValueType()
        {
            throw new NotImplementedException();
        }
    }
}
