using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DaeMvvmFramework;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer
{
    public class PropertyViewerItemContext : UndoableChangeSource
    {
        public PropertyViewerGroupContext Group { get; private set; }

        public string Name { get; private set; }

        public List<IVariableContext> PossibleVars { get; private set; }

        #region Variable
        private IVariableContext _variable;
        public const string VariableProperty = "Variable";
        public IVariableContext Variable
        {
            get { return _variable; }
            set { Change(_variable, value, (newValue) => Swap(ref _variable, newValue, VariableProperty)); }
        }
        #endregion

        public VariableComponentType VariableComponentType { get; private set; }
        public VariableComponentCount VariableComponentCount { get; private set; }

        public PropertyViewerItemContext(PropertyViewerGroupContext group, string name, VariableComponentType type, VariableComponentCount count)
        {
            Group = group;
            Name = name;
            VariableComponentType = type;
            VariableComponentCount = count;
            PossibleVars = new List<IVariableContext>();
            
            switch (type)
            {
                case VariableComponentType.Int:
                    PossibleVars.Add(new ConstantVarContext(count, new IntType(group.PropertyViewer.Effect.Evolution)));
                    PossibleVars.Add(new RandomVarContext(count, new IntType(group.PropertyViewer.Effect.Evolution), new IntType(group.PropertyViewer.Effect.Evolution)));
                    Variable = PossibleVars[0];
                    break;
                case VariableComponentType.Float:
                    PossibleVars.Add(new ConstantVarContext(count, new FloatType(group.PropertyViewer.Effect.Evolution)));
                    PossibleVars.Add(new RandomVarContext(count, new FloatType(group.PropertyViewer.Effect.Evolution), new FloatType(group.PropertyViewer.Effect.Evolution)));
                    if (count == VariableComponentCount.Rgba)
                    {
                        PossibleVars.Add(new CurveVarContext(Group.PropertyViewer.Effect.Evolution, count,
                                                             new List<Range>()
                                                                 {
                                                                     new Range() {Min = 0.0f, Max = float.MaxValue},
                                                                     new Range() {Min = 0.0f, Max = float.MaxValue},
                                                                     new Range() {Min = 0.0f, Max = float.MaxValue},
                                                                     new Range() {Min = 0.0f, Max = 1.0f},
                                                                 }));
                    }
                    else
                    {

                        PossibleVars.Add(new CurveVarContext(Group.PropertyViewer.Effect.Evolution, count, new List<Range>()
                                                                                                           {
                                                                                                               new Range() {Min = float.MinValue, Max = float.MaxValue},
                                                                                                               new Range() {Min = float.MinValue, Max = float.MaxValue},
                                                                                                               new Range() {Min = float.MinValue, Max = float.MaxValue}
                                                                                                           }));
                    }
                    Variable = PossibleVars[0];
                    break;
                default:
                    throw new ArgumentOutOfRangeException("type");
            }
        }

        public override Evolution Evolution
        {
            get { return Group.PropertyViewer.Effect.Evolution; }
        }

        internal PropertyViewerItemContext Copy(PropertyViewerGroupContext group)
        {
            PropertyViewerItemContext temp = new PropertyViewerItemContext(group, Name, VariableComponentType, VariableComponentCount);
            temp.Variable = Variable.Copy();
            return temp;
        }
    }
}
