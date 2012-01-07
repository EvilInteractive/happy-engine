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

        public int Width { get { return 2048; } }

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

    }
}
