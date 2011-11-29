using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace HappyFxEditor
{
    class TimeLineTrack : INotifyPropertyChanged
    {
        public ObservableCollection<TimeLineComponent> Components { get; private set; }

        private TimeLine _timeLine;

        public TimeLineTrack(TimeLine parent)
        {
            _timeLine = parent;
            Components = new ObservableCollection<TimeLineComponent>();
            Components.Add(new TimeLineComponent(parent, TimeLineComponentType.CameraFx));
            Components[0].StartTime = 0;
            Components[0].EndTime = 1;
            Components.Add(new TimeLineComponent(parent, TimeLineComponentType.ParticleSystem));
            Components[1].StartTime = 1;
            Components[1].EndTime = 3;
            Components.Add(new TimeLineComponent(parent, TimeLineComponentType.ParticleSystem));
            Components[2].StartTime = 3;
            Components[2].EndTime = 4;
            Components.Add(new TimeLineComponent(parent, TimeLineComponentType.CameraFx));
            Components[3].StartTime = 5;
            Components[3].EndTime = 6;
        }


        public event PropertyChangedEventHandler PropertyChanged;
    }
}
