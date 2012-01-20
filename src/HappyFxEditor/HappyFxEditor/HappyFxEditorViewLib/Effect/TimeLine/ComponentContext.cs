using System;
using System.IO;
using System.Linq;
using System.Windows.Input;
using System.Windows.Media;
using HappyFxEditorBaseLib;
using DaeMvvmFramework;
using HappyFxEditorContextLib.Effect.ComponentEditor;

namespace HappyFxEditorContextLib.Effect.TimeLine
{
    public class ComponentContext : PropertyChangeSource
    {
        public ICommand CutCommand { get; private set; }
        public ICommand CopyCommand { get; private set; }
        public ICommand DeleteCommand { get; private set; }

        public TimeLineTrackContext Track { get; private set; }

        #region Behaviours
        private UndoableCollection<BehaviourContext> _behaviours;
        public const string BehavioursProperty = "Behaviours";
        public UndoableCollection<BehaviourContext> Behaviours
        {
            get { return _behaviours; }
            set { Change(ref _behaviours, value, BehavioursProperty); }
        }
        #endregion       

        public ComponentContext(TimeLineTrackContext track, TimeLineTrackComponentType type)
        {
            Track = track;

            if (Track != null)
            {
                _behaviours = new UndoableCollection<BehaviourContext>(Track.TimeLine.Effect);
                if (track.TimeLine.Effect.ComponentEditor.GetToolBox(type).Required != null)
                    _behaviours.Add(track.TimeLine.Effect.ComponentEditor.GetToolBox(type).Required.Copy());
            }

            CutCommand = CommandFactory.Create(() =>
                                                   {
                                                       track.TimeLine.ClipBoard = this;
                                                       track.Remove(this);
                                                   });
            CopyCommand = CommandFactory.Create(() => track.TimeLine.ClipBoard = Copy());
            DeleteCommand = CommandFactory.Create(() => track.Remove(this));


            Type = type;
            UpdateColors();
            Hue = (short)((360.0 / ((int)TimeLineTrackComponentType.MAX_TYPES+1)) * (int)type);
            switch (type)
            {
                case TimeLineTrackComponentType.ParticleSystem:
                    Text = "Particle System";
                    break;
                case TimeLineTrackComponentType.CameraFx:
                    Text = "Camera Fx";
                    break;
                case TimeLineTrackComponentType.AudioFx:
                    Text = "Audio Fx";
                    break;
                case TimeLineTrackComponentType.MAX_TYPES:
                    Text = "Select";
                    break;
                default:
                    Text = "";
                    break;
            }
        }

        #region X
        public const string XProperty = "X";
        public double X
        {
            get
            {
                if (Track != null)
                    return StartTime * Track.TimeLine.TimeScale;
                else
                    return StartTime;
            }
            set
            {
                if (Track != null)
                {
                    double w = Width;
                    StartTime = value / Track.TimeLine.TimeScale;
                    EndTime = StartTime + w / Track.TimeLine.TimeScale;
                }
                else
                {
                    double w = Width;
                    StartTime = value;
                    EndTime = value + w;
                }
            }
        }
        #endregion

        #region Width
        public const string WidthProperty = "Width";
        public double Width
        {
            get
            {
                if (Track != null)
                    return (EndTime - StartTime) * Track.TimeLine.TimeScale;
                else
                    return EndTime - StartTime;
            }
            set
            {
                if (Track != null)
                    EndTime = StartTime + value / Track.TimeLine.TimeScale;
                else
                    EndTime = StartTime + value;
            }
        }
        #endregion
        
        private int _height = 24;
        public int Height
        {
            get { return _height; }
            set { Change(ref _height, value, "Height", "ForegroundHeight", "FontSize"); }
        }

        private short _hue = 0;
        public short Hue
        {
            get { return _hue; }
            private set { Change(_hue, value, (newValue) => { _hue = newValue; UpdateColors(); }, "Hue", "BackgroundColor", "ForegroundColor"); }
        }

        #region StartTime
        private double _startTime;
        public const string StartTimeProperty = "StartTime";
        public double StartTime
        {
            get { return _startTime; }
            set { Change(ref _startTime, value, StartTimeProperty, XProperty, WidthProperty, ForegroundWidthProperty); }
        }
        #endregion

        #region EndTime
        private double _endTime;
        public const string EndTimeProperty = "EndTime";
        public double EndTime
        {
            get { return _endTime; }
            set { Change(ref _endTime, value, EndTimeProperty, WidthProperty, ForegroundWidthProperty); }
        }
        #endregion
 
        public TimeLineTrackComponentType Type { get; private set; }

        private string _text = "";
        public string Text
        {
            get { return _text; }
            set { Change(ref _text, value, "Text"); }
        }

        private bool _isSelected = false;
        public bool IsSelected
        {
            get { return _isSelected; }
            set { Change(_isSelected, value, (newValue) => { _isSelected = newValue; UpdateColors(); }, "IsSelected", "BackgroundColor", "ForegroundColor"); }
        }

        public const string ForegroundWidthProperty = "ForegroundWidth";
        public int ForegroundWidth
        {
            get
            {
                return (int)Width - 8;
            }
        }
        public int ForegroundHeight { get { return Height - 8; } }

        public double FontSize { get { return ForegroundHeight / 1.499998125; } } //Magic!

        public Brush BackgroundColor { get; private set; }
        public Brush ForegroundColor { get; private set; }

        private void UpdateColors()
        {
            BackgroundColor = new SolidColorBrush(ColorExtensions.FromHsb(Hue, 0.89, IsSelected ? 0.80 : 0.37));
            ForegroundColor = new SolidColorBrush(ColorExtensions.FromHsb((short)(Hue + 16), IsSelected ? 0.40 : 0.80, IsSelected ? 0.90 : 0.86));
        }

        internal ComponentContext Copy()
        {
            ComponentContext comp = new ComponentContext(Track, Type);
            comp.Height = Height;
            comp.X = X;
            comp.Width = Width;
            comp.Behaviours = new UndoableCollection<BehaviourContext>(Track.TimeLine.Effect);
            foreach (var behaviour in Behaviours)
            {
                comp.AddBehaviour(behaviour.Copy());
            }
            return comp;
        }

        internal void AddBehaviour(BehaviourContext behaviourContext)
        {
            Behaviours.Add(behaviourContext);
        }

        internal void DeleteBehaviour(BehaviourContext behaviour)
        {
            _behaviours.Remove(behaviour);
        }

        internal void Serialize(BinaryWriter stream)
        {
            stream.Write((UInt32)Type);
            stream.Write((float)StartTime);
            stream.Write((float)EndTime);

            stream.Write((UInt32)Behaviours.Count);
            foreach (var behaviour in Behaviours)
            {
                behaviour.Serialize(stream);
            }
        }

        public static ComponentContext DeSerialize(BinaryReader stream, TimeLineTrackContext timeLineTrackContext)
        {
            ComponentContext comp = new ComponentContext(timeLineTrackContext, (TimeLineTrackComponentType)stream.ReadUInt32());
            comp.StartTime = stream.ReadSingle();
            comp.EndTime = stream.ReadSingle();
            comp.Behaviours = new UndoableCollection<BehaviourContext>(timeLineTrackContext.TimeLine.Effect);

            uint behaviours = stream.ReadUInt32();
            for (int i = 0; i < behaviours; i++)
            {
                comp.AddBehaviour(BehaviourContext.DeSerialize(stream, comp));
            }

            return comp;
        }
    }
}
