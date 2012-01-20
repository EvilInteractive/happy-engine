using System;
using System.Collections.Generic;
using System.IO;
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

        private ConstantVarContext _template;

        public RandomVarContext(EffectContext effect, ConstantVarContext template)
        {
            _template = template;
            MinVar = (ConstantVarContext)template.Copy(effect);
            MaxVar = (ConstantVarContext)template.Copy(effect);
        }

        public VariableType GetVarType()
        {
            return VariableType;
        }

        public IVariableContext Copy(EffectContext effect)
        {
            RandomVarContext temp = new RandomVarContext(effect, _template);
            temp.MinVar = (ConstantVarContext)MinVar.Copy(effect);
            temp.MaxVar = (ConstantVarContext)MaxVar.Copy(effect);

            return temp;
        }

        public override string ToString()
        {
            return "Random";
        }


        public void Serialize(BinaryWriter stream)
        {
            MinVar.Serialize(stream);
            MaxVar.Serialize(stream);
        }

        public void DeSerialize(BinaryReader stream)
        {
            MinVar.DeSerialize(stream);
            MaxVar.DeSerialize(stream);
        }
    }
}
