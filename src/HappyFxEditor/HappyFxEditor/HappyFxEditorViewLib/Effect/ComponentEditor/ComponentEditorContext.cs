using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Windows.Input;
using DaeMvvmFramework;
using HappyFxEditorBaseLib;
using HappyFxEditorContextLib.Effect.TimeLine;

namespace HappyFxEditorContextLib.Effect.ComponentEditor
{
    public class ComponentEditorContext : PropertyChangeSource
    {
        public EffectContext Effect { get; private set; }

        private ComponentContext _selectedComponent;
        public const string SelectedComponentProperty = "SelectedComponent";
        public ComponentContext SelectedComponent
        {
            get { return _selectedComponent; }
            set { Change(ref _selectedComponent, value, SelectedComponentProperty); }
        }

        #region SelectedBehaviour
        private BehaviourContext _selectedBehaviour;
        public const string SelectedBehaviourProperty = "SelectedBehaviour";
        public BehaviourContext SelectedBehaviour
        {
            get { return _selectedBehaviour; }
            set { Change(ref _selectedBehaviour, value, SelectedBehaviourProperty); }
        }
        #endregion


        public List<BehaviourToolboxContext> ToolBoxes { get; private set; }

        private BehaviourToolboxContext _toolbox = null;
        public const string ToolBoxProperty = "ToolBox";
        public BehaviourToolboxContext ToolBox
        {
            get { return _toolbox; }
            set { Change(ref _toolbox, value, ToolBoxProperty); }
        }

        #region AddBehaviourCommand

        public ICommand AddBehaviourCommand { get; set; }
        public ICommand DeleteBehaviourCommand { get; set; }

        #endregion
        

        public ComponentEditorContext(EffectContext effect)
        {
            ToolBoxes = new List<BehaviourToolboxContext>();
            Effect = effect;
            for (int i = 0; i < (int)TimeLineTrackComponentType.MAX_TYPES; ++i)
            {
                ToolBoxes.Add(new BehaviourToolboxContext(this, (TimeLineTrackComponentType)i));
            }

        }

        public void Init()
        {
            Effect.TimeLine.PropertyChanged += (s, e) =>
                                          {
                                              if (e.PropertyName == TimeLineContext.SelectedComponentProperty)
                                                  ChangeComponent(Effect.TimeLine.SelectedComponent);
                                          };
            
            AddBehaviourCommand = CommandFactory.Create(() => SelectedComponent.AddBehaviour(ToolBox.SelectedTool.Copy()));
            DeleteBehaviourCommand = CommandFactory.Create(() => SelectedComponent.DeleteBehaviour(SelectedBehaviour),
                () => SelectedBehaviour != null && SelectedComponent != null);
        }

        private void ChangeComponent(ComponentContext componentContext)
        {
            SelectedComponent = componentContext;
            ToolBox = (componentContext == null || componentContext.Type == TimeLineTrackComponentType.MAX_TYPES) ? null : ToolBoxes[(int)componentContext.Type];
        }

        internal BehaviourToolboxContext GetToolBox(TimeLineTrackComponentType timeLineTrackComponentType)
        {
            return ToolBoxes[(int) timeLineTrackComponentType];
        }
    }
}
