using System;
using System.IO;
using DaeMvvmFramework;
using System.Windows.Input;
using System.Collections.ObjectModel;
using HappyFxEditorBaseLib;

namespace HappyFxEditorContextLib.Effect.TimeLine
{
    public class TimeLineContext : UndoableChangeSource
    {
        public EffectContext Effect { get; private set; }

        #region Commands
        public ICommand AddTrackCommand { get; private set; }
        public ICommand DeleteTrackCommand { get; private set; }
        public ICommand DeleteComponentCommand { get; private set; }
        #endregion

        public UndoableCollection<TimeLineTrackContext> TimeLineTracks { get; private set; }

        #region Selected Track
        private TimeLineTrackContext _selectedTrack;
        public static readonly string SelectedTrackProperty = "SelectedTrack";
        public TimeLineTrackContext SelectedTrack 
        {
            get { return _selectedTrack; }
            set
            {
                SelectedComponent = null;
                Change(_selectedTrack, value, (newValue) =>
                {
                    SelectedComponent = null;
                    _selectedTrack = value;
                    foreach (var track in TimeLineTracks)
                    {
                        track.IsSelected = track == newValue;
                    }
                }, "SelectedTrack");
            }
        }
        #endregion

        #region Selected Component
        private ComponentContext _selectedComponent;
        public static readonly string SelectedComponentProperty = "SelectedComponent";
        public ComponentContext SelectedComponent 
        {
            get { return _selectedComponent; }
            set
            {
                Effect.ComponentEditor.SelectedBehaviour = null;
                Change(_selectedComponent, value, (newValue) =>
                {
                    _selectedComponent = value;
                    foreach (var t in TimeLineTracks)
                    {
                        foreach (var c in t.Components)
                        {
                            c.IsSelected = c == value;
                        }
                    }
                }, "SelectedComponent");
            }
        }
        #endregion

        #region Duration
        private double _duration;
        public const string DurationProperty = "Duration";
        public double Duration
        {
            get { return _duration; }
            set { Change(_duration, value, (newValue)=>Swap(ref _duration, newValue, DurationProperty, WidthProperty, ScrollWidthProperty)); }
        }
        #endregion

        #region TimeScale in pixel/s
        private double _timeScale;
        public const string TimeScaleProperty = "TimeScale";
        public double TimeScale
        {
            get { return _timeScale; }
            set { Change(ref _timeScale, value, TimeScaleProperty, WidthProperty); }
        }
        #endregion

        #region MinScale
        public double MinScale
        {
            get { return 16; }
        }
        #endregion

        #region MaxScale
        public double MaxScale
        {
            get { return 1600; }
        }
        #endregion

        #region Width
        public const string WidthProperty = "Width";
        public double Width
        {
            get { return Duration * TimeScale; }
        }
        #endregion        

        #region Scroll
        private double _scroll;
        public const string ScrollProperty = "Scroll";
        public double Scroll
        {
            get { return _scroll; }
            set { Change(ref _scroll, value, ScrollProperty); }
        }
        #endregion

        #region ScrollWidth
        public const string ScrollWidthProperty = "ScrollWidth";
        public double ScrollWidth
        {
            get { return Width - ScrollViewPort; }
        }
        #endregion

        #region ScrollViewPort
        private double _scrollViewPort;
        public const string ScrollViewPortProperty = "ScrollViewPort";
        public double ScrollViewPort
        {
            get { return _scrollViewPort; }
            set { Change(ref _scrollViewPort, value, ScrollViewPortProperty, ScrollWidthProperty); }
        }
        #endregion
        

        public ObservableCollection<ComponentContext> ComponentTools { get; private set; }

        #region Selected Component Tool
        ComponentContext _selectedComponentTool;
        public ComponentContext SelectedComponentTool 
        {
            get { return _selectedComponentTool; }
            set
            {
                Change(_selectedComponentTool, value, (newValue) =>
                    {
                        _selectedComponentTool = value;
                        foreach (var comp in ComponentTools)
                        {
                            comp.IsSelected = comp == newValue;
                        }
                    }, "SelectedComponentTool");
            }
        }
        #endregion

        private object _clipBoard;
        public static readonly string ClipBoardProperty = "ClipBoard";
        public object ClipBoard
        {
            get { return _clipBoard; }
            set { Change(ref _clipBoard, value, ClipBoardProperty); }
        }

        public TimeLineContext(EffectContext effect)
        {
            Effect = effect;
            TimeLineTracks = new UndoableCollection<TimeLineTrackContext>(this);
            AddTrackCommand = CommandFactory.Create(AddTrack);
            DeleteTrackCommand = CommandFactory.Create(DeleteTrack, () => SelectedTrack != null, this, SelectedTrackProperty);
            DeleteComponentCommand = CommandFactory.Create(() => SelectedTrack.Remove(SelectedComponent),
                                                           () => SelectedTrack != null && SelectedComponent != null, this, SelectedTrackProperty, SelectedComponentProperty);

            Duration = 2;
            TimeScale = 256;
            ComponentTools = new ObservableCollection<ComponentContext>()
            {
                new ComponentContext(null, TimeLineTrackComponentType.MAX_TYPES) { Width=128, Height=32 },  // Select tool
                new ComponentContext(null, TimeLineTrackComponentType.CameraFx) { Width=128, Height=32 },               
                new ComponentContext(null, TimeLineTrackComponentType.ParticleSystem) { Width=128, Height=32 },               
                new ComponentContext(null, TimeLineTrackComponentType.AudioFx) { Width=128, Height=32 }
            };
        }

        private void AddTrack()
        {
            using (BeginTransaction())
            {
                TimeLineTracks.Add(new TimeLineTrackContext(this));
            }
        }

        private void DeleteTrack()
        {
            using (BeginTransaction())
            {
                TimeLineTracks.Remove(SelectedTrack);
            }
            SelectedTrack = null;
            SelectedComponent = null;
        }

        public override Evolution Evolution
        {
            get { return Effect.Evolution; }
        }

        public void SetSelectTool()
        { 
            SelectedComponentTool = ComponentTools[0];
        }


        internal void Serialize(BinaryWriter stream)
        {
            stream.Write((float)Duration);
            stream.Write((UInt32)TimeLineTracks.Count);
            foreach (var track in TimeLineTracks)
            {
                track.Serialize(stream);
            }
        }

        internal static TimeLineContext DeSerialize(BinaryReader stream, EffectContext effectContext)
        {
            TimeLineContext timeLine = new TimeLineContext(effectContext);
            timeLine.Duration = stream.ReadSingle();
            uint tracks = stream.ReadUInt32();
            for (int i = 0; i < tracks; i++)
            {
                timeLine.TimeLineTracks.Add(TimeLineTrackContext.DeSerialize(stream, timeLine));
            }
            timeLine.SetSelectTool();
            return timeLine;
        }
    }
}
