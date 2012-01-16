using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables
{
    class RandomVarContext : IVariableContext
    {
        public ConstantVarContext MinVar { get; private set; }
        public ConstantVarContext MaxVar { get; private set; }

        public VariableType VariableType { get { return VariableType.Random; } }
        public VariableComponentCount VariableComponentCount { get; private set; }
        public VariableComponentType VariableComponentType { get; private set; }

        public RandomVarContext(VariableComponentCount components, IType min, IType max)
        {
            VariableComponentCount = components;
            MinVar = new ConstantVarContext(components, min);
            MaxVar = new ConstantVarContext(components, max);

            VariableComponentType = min.GetVarType();
        }

        public VariableType GetVarType()
        {
            return VariableType;
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
            RandomVarContext temp = new RandomVarContext(VariableComponentCount, MinVar.Value[0], MaxVar.Value[0]);
            temp.MinVar = MinVar.Copy() as ConstantVarContext;
            temp.MaxVar = MaxVar.Copy() as ConstantVarContext;

            return temp;
        }

        public override string ToString()
        {
            return "Random " + VariableComponentCount;
        }
    }
}
