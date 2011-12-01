using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows.Input;
using System.Windows;
using System.Windows.Controls;

namespace HappyFxEditor
{
    class TimeLineTrack : INotifyPropertyChanged
    {
        public ObservableCollection<TimeLineComponent> Components { get; private set; }

        ContextMenu _trackContexMenu;
        public ContextMenu TrackContexMenu
        {
            get { return _trackContexMenu; }
            private set
            {
                if (_trackContexMenu != value)
                {
                    _trackContexMenu = value;
                    OnPropertyChanged("TrackContexMenu");
                }
            }
        }


        public TimeLine Parent { get; private set; }

        public TimeLineTrack(TimeLine parent)
        {
            Parent = parent;

            _trackContexMenu = new ContextMenu();
            MenuItem item = new MenuItem();
            item.Header = "Test";
            _trackContexMenu.Items.Add(item);

            Components = new ObservableCollection<TimeLineComponent>();
            Components.Add(new TimeLineComponent(this, TimeLineComponentType.CameraFx));
            Components[0].StartTime = 0;
            Components[0].EndTime = 1;
            Components[0].PropertyChanged += new PropertyChangedEventHandler(TimeLineTrackComponent_PropertyChanged);
            Components.Add(new TimeLineComponent(this, TimeLineComponentType.ParticleSystem));
            Components[1].StartTime = 1;
            Components[1].EndTime = 3;
            Components[1].PropertyChanged += new PropertyChangedEventHandler(TimeLineTrackComponent_PropertyChanged);
            Components.Add(new TimeLineComponent(this, TimeLineComponentType.ParticleSystem));
            Components[2].StartTime = 3;
            Components[2].EndTime = 4;
            Components[2].PropertyChanged += new PropertyChangedEventHandler(TimeLineTrackComponent_PropertyChanged);
            Components.Add(new TimeLineComponent(this, TimeLineComponentType.CameraFx));
            Components[3].StartTime = 5;
            Components[3].EndTime = 6;
            Components[3].PropertyChanged += new PropertyChangedEventHandler(TimeLineTrackComponent_PropertyChanged);
        }

        void TimeLineTrackComponent_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "IsSelected")
            {
                Parent.SelectionChanged(sender as TimeLineComponent);
            }
        }


        public void RightClick(object sender, MouseEventArgs e)
        {

        }

        private void OnPropertyChanged(string prop)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
                handler(this, new PropertyChangedEventArgs(prop));
        }
        public event PropertyChangedEventHandler PropertyChanged;
    }
}
