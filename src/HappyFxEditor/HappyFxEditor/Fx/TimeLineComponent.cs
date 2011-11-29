using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.ComponentModel;

namespace HappyFxEditor
{
    enum TimeLineComponentType
    {
        CameraFx,
        ParticleSystem
    }
    class TimeLineComponent : INotifyPropertyChanged
    {
        private TimeLine _timeLine;

        double _startTime, _endTime;
        public double StartTime
        {
            get
            {
                return _startTime;
            }
            set
            {
                if (_startTime != value)
                {
                    _startTime = value;
                    OnPropertyChanged("X");
                    OnPropertyChanged("Width");
                    OnPropertyChanged("StartTime");
                }
            }
        }
        public double EndTime
        {
            get
            {
                return _endTime;
            }
            set
            {
                if (_endTime != value)
                {
                    _endTime = value;
                    OnPropertyChanged("Width");
                    OnPropertyChanged("EndTime");
                }
            }
        }

        public int X { get { return (int)(StartTime * _timeLine.TimeScale); } }
        public int Width { get { return (int)((EndTime - StartTime) * _timeLine.TimeScale); } }

        private SolidColorBrush _color;
        public SolidColorBrush Color 
        { 
            get
            {
                return _color;
            }
            private set
            {
                if (_color != value)
                {
                    _color = value;
                    OnPropertyChanged("Name");
                }
            }
        }
        private string _name;
        public string Name
        {
            get
            {
                return _name;
            }
            private set
            {
                if (_name != value)
                {
                    _name = value;
                    OnPropertyChanged("Name");
                }
            }
        }


        public TimeLineComponent(TimeLine parent, TimeLineComponentType type)
        {
            _timeLine = parent;

            _timeLine.PropertyChanged += new PropertyChangedEventHandler(_timeLine_PropertyChanged);

            switch (type)
            {
                case TimeLineComponentType.CameraFx:
                    Name = "Camera Fx";
                    Color = Brushes.Green;
                    break;
                case TimeLineComponentType.ParticleSystem:
                    Name = "Particle System";
                    Color = Brushes.Orange;
                    break;
                default:
                    break;
            }
        }

        void _timeLine_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "TimeScale")
            {
                OnPropertyChanged("X");
                OnPropertyChanged("Width");
            }
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
