using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using DaeMvvmFramework;
using HappyFxEditorBaseLib;
using HappyFxEditorBaseLib.Behaviour;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables;

namespace HappyFxEditorContextLib.Effect.ComponentEditor
{
    public class BehaviourToolboxContext : PropertyChangeSource
    {
        private ObservableCollection<BehaviourContext> _tools = new ObservableCollection<BehaviourContext>();

        public ComponentEditorContext ComponentEditor{ get; private set; }

        #region Tools
        public const string ToolsProperty = "Tools";
        public ObservableCollection<BehaviourContext> Tools
        {
            get { return _tools; }
            set { Change(ref _tools, value, ToolsProperty); }
        }
        #endregion

        #region SelectedTool
        private BehaviourContext _selectedTool;
        public const string SelectedToolProperty = "SelectedTool";
        public BehaviourContext SelectedTool
        {
            get { return _selectedTool; }
            set { Change(ref _selectedTool, value, SelectedToolProperty); }
        }
        #endregion

        public BehaviourToolboxContext(ComponentEditorContext componentEditor, TimeLineTrackComponentType type)
        {
            ComponentEditor = componentEditor;
            switch (type)
            {
                case TimeLineTrackComponentType.ParticleSystem:
                    GeneratePsItems();
                    break;
                case TimeLineTrackComponentType.CameraFx:
                    GenerateCamFxItems();
                    break;
                case TimeLineTrackComponentType.AudioFx:
                    GenerateAudFxItems();
                    break;
                default:
                    throw new ArgumentOutOfRangeException("type");
            }
        }

        private void GeneratePsItems()
        {
            Tools.Clear();

            #region Color
            var property = new PropertyViewerContext(ComponentEditor.Effect);
            property.Groups.Add(new PropertyViewerGroupContext(property, "Color"));
            property.Groups[0].Items.Add(new PropertyViewerItemContext(property.Groups[0], "Color", VariableComponentType.Float, VariableComponentCount.Rgba));
            Tools.Add(new BehaviourContext(property));
            Tools[Tools.Count - 1].Hue = (360/(int) ParticleSystemBehaviour.MAX)*0;
            Tools[Tools.Count - 1].ToolTip = "Change Color and Alpha";
            Tools[Tools.Count - 1].SetType(ParticleSystemBehaviour.Color, "Color");
            #endregion

            #region Force
            property = new PropertyViewerContext(ComponentEditor.Effect);
            property.Groups.Add(new PropertyViewerGroupContext(property, "Force"));
            property.Groups[0].Items.Add(new PropertyViewerItemContext(property.Groups[0], "Force", VariableComponentType.Float, VariableComponentCount.Xyz));
            Tools.Add(new BehaviourContext(property));
            Tools[Tools.Count - 1].Hue = (360 / (int)ParticleSystemBehaviour.MAX) * 1;
            Tools[Tools.Count - 1].ToolTip = "Apply a force";
            Tools[Tools.Count - 1].SetType(ParticleSystemBehaviour.Force, "Force");
            #endregion

            #region Location
            property = new PropertyViewerContext(ComponentEditor.Effect);
            property.Groups.Add(new PropertyViewerGroupContext(property, "Location"));
            property.Groups[0].Items.Add(new PropertyViewerItemContext(property.Groups[0], "Location", VariableComponentType.Float, VariableComponentCount.Xyz));
            Tools.Add(new BehaviourContext(property));
            Tools[Tools.Count - 1].Hue = (360 / (int)ParticleSystemBehaviour.MAX) * 2;
            Tools[Tools.Count - 1].ToolTip = "Set the particle spawn location";
            Tools[Tools.Count - 1].SetType(ParticleSystemBehaviour.Location, "Location");
            #endregion

            #region Rotation
            property = new PropertyViewerContext(ComponentEditor.Effect);
            property.Groups.Add(new PropertyViewerGroupContext(property, "Rotation"));
            property.Groups[0].Items.Add(new PropertyViewerItemContext(property.Groups[0], "Rotation", VariableComponentType.Float, VariableComponentCount.X));
            Tools.Add(new BehaviourContext(property));
            Tools[Tools.Count - 1].Hue = (360 / (int)ParticleSystemBehaviour.MAX) * 3;
            Tools[Tools.Count - 1].ToolTip = "Set the rotation";
            Tools[Tools.Count - 1].SetType(ParticleSystemBehaviour.Rotation, "Rotation");
            #endregion

            #region RotationRate
            property = new PropertyViewerContext(ComponentEditor.Effect);
            property.Groups.Add(new PropertyViewerGroupContext(property, "RotationRate"));
            property.Groups[0].Items.Add(new PropertyViewerItemContext(property.Groups[0], "RotationRate", VariableComponentType.Float, VariableComponentCount.X));
            Tools.Add(new BehaviourContext(property));
            Tools[Tools.Count - 1].Hue = (360 / (int)ParticleSystemBehaviour.MAX) * 4;
            Tools[Tools.Count - 1].ToolTip = "Set the rotation rate / spin speed";
            Tools[Tools.Count - 1].SetType(ParticleSystemBehaviour.RotationRate, "RotationRate");
            #endregion

            #region Location
            property = new PropertyViewerContext(ComponentEditor.Effect);
            property.Groups.Add(new PropertyViewerGroupContext(property, "Scale"));
            property.Groups[0].Items.Add(new PropertyViewerItemContext(property.Groups[0], "Scale", VariableComponentType.Float, VariableComponentCount.Xyz));
            Tools.Add(new BehaviourContext(property));
            Tools[Tools.Count - 1].Hue = (360 / (int)ParticleSystemBehaviour.MAX) * 5;
            Tools[Tools.Count - 1].ToolTip = "Set the scale";
            Tools[Tools.Count - 1].SetType(ParticleSystemBehaviour.Scale, "Scale");
            #endregion

            #region Speed
            property = new PropertyViewerContext(ComponentEditor.Effect);
            property.Groups.Add(new PropertyViewerGroupContext(property, "Speed"));
            property.Groups[0].Items.Add(new PropertyViewerItemContext(property.Groups[0], "Speed", VariableComponentType.Float, VariableComponentCount.Xyz));
            Tools.Add(new BehaviourContext(property));
            Tools[Tools.Count - 1].Hue = (360 / (int)ParticleSystemBehaviour.MAX) * 6;
            Tools[Tools.Count - 1].ToolTip = "Set the speed/velocity";
            Tools[Tools.Count - 1].SetType(ParticleSystemBehaviour.Speed, "Speed");
            #endregion
        }
        private void GenerateCamFxItems()
        {
            Tools.Clear();

            #region Shake
            var property = new PropertyViewerContext(ComponentEditor.Effect);
            property.Groups.Add(new PropertyViewerGroupContext(property, "Shake"));
            property.Groups[0].Items.Add(new PropertyViewerItemContext(property.Groups[0], "Shake", VariableComponentType.Float, VariableComponentCount.Xyz));
            Tools.Add(new BehaviourContext(property));
            Tools[Tools.Count - 1].Hue = (360 / (int)CameraFxBehaviour.MAX) * 0;
            Tools[Tools.Count - 1].ToolTip = "Shake the camera";
            Tools[Tools.Count - 1].SetType(CameraFxBehaviour.Shake, "Shake");
            #endregion

            #region Flash
            property = new PropertyViewerContext(ComponentEditor.Effect);
            property.Groups.Add(new PropertyViewerGroupContext(property, "Flash"));
            property.Groups[0].Items.Add(new PropertyViewerItemContext(property.Groups[0], "Power", VariableComponentType.Float, VariableComponentCount.X));
            Tools.Add(new BehaviourContext(property));
            Tools[Tools.Count - 1].Hue = (360 / (int)CameraFxBehaviour.MAX) * 1;
            Tools[Tools.Count - 1].ToolTip = "White over-exposure effect";
            Tools[Tools.Count - 1].SetType(CameraFxBehaviour.Flash, "Flash");
            #endregion
        }
        private void GenerateAudFxItems()
        {
            Tools.Clear();

            #region Volume
            var property = new PropertyViewerContext(ComponentEditor.Effect);
            property.Groups.Add(new PropertyViewerGroupContext(property, "Volume"));
            property.Groups[0].Items.Add(new PropertyViewerItemContext(property.Groups[0], "Volume", VariableComponentType.Float, VariableComponentCount.X));
            Tools.Add(new BehaviourContext(property));
            Tools[Tools.Count - 1].Hue = (360 / (int)AudioFxBehaviour.MAX) * 0;
            Tools[Tools.Count - 1].ToolTip = "Set the volume";
            Tools[Tools.Count - 1].SetType(AudioFxBehaviour.Volume, "Volume");
            #endregion

            #region Pan
            property = new PropertyViewerContext(ComponentEditor.Effect);
            property.Groups.Add(new PropertyViewerGroupContext(property, "Pan"));
            property.Groups[0].Items.Add(new PropertyViewerItemContext(property.Groups[0], "Pan", VariableComponentType.Float, VariableComponentCount.X));
            Tools.Add(new BehaviourContext(property));
            Tools[Tools.Count - 1].Hue = (360 / (int)AudioFxBehaviour.MAX) * 1;
            Tools[Tools.Count - 1].ToolTip = "Set the audio panning";
            Tools[Tools.Count - 1].SetType(AudioFxBehaviour.Pan, "Pan");
            #endregion

            #region Pitch
            property = new PropertyViewerContext(ComponentEditor.Effect);
            property.Groups.Add(new PropertyViewerGroupContext(property, "Pitch"));
            property.Groups[0].Items.Add(new PropertyViewerItemContext(property.Groups[0], "Pitch", VariableComponentType.Float, VariableComponentCount.X));
            Tools.Add(new BehaviourContext(property));
            Tools[Tools.Count - 1].Hue = (360 / (int)AudioFxBehaviour.MAX) * 2;
            Tools[Tools.Count - 1].ToolTip = "Change the pitch of the audio";
            Tools[Tools.Count - 1].SetType(AudioFxBehaviour.Pitch, "Pitch");
            #endregion
        }

    }
}
