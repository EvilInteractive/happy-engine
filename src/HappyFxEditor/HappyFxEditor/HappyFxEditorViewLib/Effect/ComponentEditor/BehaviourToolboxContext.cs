using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Windows;
using System.Xml;
using DaeMvvmFramework;
using HappyFxEditorBaseLib;
using HappyFxEditorBaseLib.Behaviour;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types;

namespace HappyFxEditorContextLib.Effect.ComponentEditor
{
    public class BehaviourToolboxContext : PropertyChangeSource
    {
        private readonly TimeLineTrackComponentType _type;

        public ComponentEditorContext ComponentEditor{ get; private set; }

        #region Tools
        public const string ToolsProperty = "Tools";
        private ObservableCollection<BehaviourContext> _tools = new ObservableCollection<BehaviourContext>();
        public ObservableCollection<BehaviourContext> Tools
        {
            get { return _tools; }
            set { Change(ref _tools, value, ToolsProperty); }
        }
        #endregion

        #region Required
        public const string RequiredProperty = "Required";
        private BehaviourContext _requiredBehaviour;
        public BehaviourContext Required
        {
            get { return _requiredBehaviour; }
            set { Change(ref _requiredBehaviour, value, RequiredProperty); }
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

        private PropertyViewerContext _currentParseProperty;

        public BehaviourToolboxContext(ComponentEditorContext componentEditor, TimeLineTrackComponentType type)
        {
            _type = type;
            ComponentEditor = componentEditor;
            XmlTextReader reader = new XmlTextReader("properties.hxml");

            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element)
                {
                    if (reader.Name == "Component" && reader.GetAttribute("id") == type.ToString())
                    {
                        ParseComponent(reader);
                        break;
                    }
                }
            }

            reader.Close();
        }

        private void ParseComponent(XmlTextReader reader)
        {
            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element)
                {
                    if (reader.Name == "Behaviour")
                    {
                        ParseBehaviour(reader);
                    }
                }
                else if (reader.NodeType == XmlNodeType.EndElement)
                {
                    if (reader.Name == "Component")
                    {
                        break;
                    }
                }
            }

            int count = Required == null ? Tools.Count : Tools.Count + 1;
            for (int i = 0; i < Tools.Count; i++)
            {
                Tools[i].Hue = (short)((360 / count) * i);
            }
            if (Required != null)
                Required.Hue = (short) ((360/count)*Tools.Count);
        }

        private void ParseBehaviour(XmlTextReader reader)
        {
            _currentParseProperty = new PropertyViewerContext(ComponentEditor.Effect);
            string tooltip = reader.GetAttribute("ToolTip");
            string type = reader.GetAttribute("type");
            string name = reader.GetAttribute("name");

            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element)
                {
                    if (reader.Name == "PropertyGroup")
                    {
                        ParsePropertyGroup(reader);
                    }
                }
                else if (reader.NodeType == XmlNodeType.EndElement)
                {
                    if (reader.Name == "Behaviour")
                    {
                        break;
                    }
                }
            }
            BehaviourContext behaviour = new BehaviourContext(_currentParseProperty);
            behaviour.ToolTip = tooltip;
            switch (_type)
            {
                case TimeLineTrackComponentType.ParticleSystem:
                    ParticleSystemBehaviour psb;
                    if (Enum.TryParse(type, true, out psb) == false)
                        MessageBox.Show("Could not parse ParticleSystemBehaviour got: " + _type);
                    if (psb == ParticleSystemBehaviour.Required)
                        Required = behaviour;
                    else
                        Tools.Add(behaviour);
                    behaviour.SetType(psb, name); 
                    break;
                case TimeLineTrackComponentType.CameraFx:
                    CameraFxBehaviour cfb;
                    if (Enum.TryParse(type, true, out cfb) == false)
                        MessageBox.Show("Could not parse CameraFxBehaviour got: " + _type);
                    if (cfb == CameraFxBehaviour.Required)
                        Required = behaviour;
                    else
                        Tools.Add(behaviour);
                    behaviour.SetType(cfb, name); 
                    break;
                case TimeLineTrackComponentType.AudioFx:
                    AudioFxBehaviour afb;
                    if (Enum.TryParse(type, true, out afb) == false)
                        MessageBox.Show("Could not parse AudioFxBehaviour got: " + _type);
                    if (afb == AudioFxBehaviour.Required)
                        Required = behaviour;
                    else
                        Tools.Add(behaviour);
                    behaviour.SetType(afb, name); 
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }
            _currentParseProperty = null;
        }

        private void ParsePropertyGroup(XmlTextReader reader)
        {
            _currentParseProperty.Groups.Add(new PropertyViewerGroupContext(_currentParseProperty,
                                                                            reader.GetAttribute("name")));
            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element)
                {
                    if (reader.Name == "Property")
                    {
                        ParseProperty(reader);
                    }
                }
                else if (reader.NodeType == XmlNodeType.EndElement)
                {
                    if (reader.Name == "PropertyGroup")
                    {
                        return;
                    }
                }
            }
        }

        private struct ComponentDesc
        {
            public string Value;
            public string Min;
            public string Max;
            public string Name;
        }
        private void ParseProperty(XmlTextReader reader)
        {
            string name = reader.GetAttribute("name");
            
            VariableComponentType type = VariableComponentType.Float;
            List<ComponentDesc> componentDescs = new List<ComponentDesc>();
            List<VariableType> varTypes = new List<VariableType>();

            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element)
                {
                    if (reader.Name == "Components")
                    {
                        if (Enum.TryParse(reader.GetAttribute("type"), true, out type) == false)
                            MessageBox.Show("Failed to parse hxml, unkown type" + reader.GetAttribute("type"));
                    }
                    if (reader.Name == "Component")
                    {
                        ComponentDesc desc;
                        desc.Min = reader.GetAttribute("min");
                        desc.Max = reader.GetAttribute("max");
                        desc.Value = reader.GetAttribute("value");
                        desc.Name = reader.GetAttribute("name");
                        componentDescs.Add(desc);
                    }
                    if (reader.Name == "Var")
                    {
                        VariableType varType = VariableType.Constant;
                        if (Enum.TryParse(reader.GetAttribute("type"), true, out varType) == false)
                            MessageBox.Show("Failed to parse hxml, unkown varType" + reader.GetAttribute("type"));
                        else
                            varTypes.Add(varType);
                    }
                }
                else if (reader.NodeType == XmlNodeType.EndElement)
                {
                    if (reader.Name == "Property")
                    {
                        List<IVariableContext> vars = new List<IVariableContext>();

                        foreach (var varType in varTypes)
                        {
                            switch (varType)
                            {
                                case VariableType.Curve:
                                    vars.Add(new CurveVarContext(ComponentEditor.Effect, CreateConstantVar(componentDescs, type)));
                                    break;
                                case VariableType.Random:
                                    vars.Add(new RandomVarContext(ComponentEditor.Effect, CreateConstantVar(componentDescs, type)));
                                    break;
                                case VariableType.Constant:
                                    vars.Add(CreateConstantVar(componentDescs, type));
                                    break;
                                case VariableType.Asset:
                                    vars.Add(new AssetVarContext(ComponentEditor.Effect, type));
                                    break;
                                case VariableType.Choose:
                                    break;
                                default:
                                    throw new ArgumentOutOfRangeException();
                            }
                        }

                        _currentParseProperty.Groups.Last().Items.Add(new PropertyViewerItemContext(
                            _currentParseProperty.Groups.Last(), name, vars));
                        return;
                    }
                }
            }
        }
        private int ParseInt(string str)
        {
            if (str == "min")
                return int.MinValue;
            else if (str == "max")
                return int.MaxValue;
            else
            {
                int temp = 0;
                if (int.TryParse(str, out temp) == false)
                    MessageBox.Show("Int Parse fail, got: " + str);
                return temp;
            }
        }
        private float ParseFloat(string str)
        {
            if (str == "min")
                return float.MinValue;
            else if (str == "max")
                return float.MaxValue;
            else
            {
                float temp = 0;
                if (float.TryParse(str, out temp) == false)
                    MessageBox.Show("Float Parse fail, got: " + str);
                return temp;
            }
        }
        private ConstantVarContext CreateConstantVar(List<ComponentDesc> componentDescs, VariableComponentType type)
        {
            List<IType> var = new List<IType>(componentDescs.Count);
            foreach (var desc in componentDescs)
            {
                switch (type)
                {
                    case VariableComponentType.Int:
                        var.Add(new IntType(ComponentEditor.Effect.Evolution,
                            ParseInt(desc.Min), ParseInt(desc.Max), ParseInt(desc.Value)) { Name = desc.Name });
                        break;
                    case VariableComponentType.Float:
                        var.Add(new FloatType(ComponentEditor.Effect.Evolution,
                            ParseFloat(desc.Min), ParseFloat(desc.Max), ParseFloat(desc.Value)) { Name = desc.Name });
                        break;
                    case VariableComponentType.Enum:
                        var.Add(new EnumType(new EnumTypeDesc(desc.Name, uint.Parse(desc.Value))));
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
            return new ConstantVarContext(ComponentEditor.Effect, var);
        }

        public BehaviourContext GetNewBehaviour(int bType)
        {
            foreach (var behaviourContext in Tools)
            {
                if (behaviourContext.GetType<int>() == bType)
                {
                    return behaviourContext.Copy();
                }
            }
            if (Required != null)
                if (Required.GetType<int>() == bType)
                    return Required.Copy();
            throw new ArgumentOutOfRangeException("bType");
        }
    }
}
