using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.ComponentModel;
using System.Windows;
using System.Windows.Input;
using System.Windows.Controls;

namespace HappyFxEditor
{
    enum TimeLineComponentType
    {
        CameraFx,
        ParticleSystem
    }
    class TimeLineComponent : INotifyPropertyChanged
    {
        public TimeLineTrack Parent { get; private set; }

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

        public int X { get { return (int)Math.Floor(StartTime * Parent.Parent.TimeScale); } }
        public int Width { get { return (int)Math.Ceiling((EndTime - StartTime) * Parent.Parent.TimeScale); } }

        private Brush _color;
        private Brush _selectedColor;

        bool _isSelected;
        public bool IsSelected
        {
            get
            {
                return _isSelected;
            }
            set
            {
                if (_isSelected != value)
                {
                    _isSelected = value;
                    OnPropertyChanged("IsSelected");
                    OnPropertyChanged("Color");
                }
            }
        }

        public Brush Color 
        { 
            get
            {
                return IsSelected? _selectedColor : _color;
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


        public TimeLineComponent(TimeLineTrack parent, TimeLineComponentType type)
        {
            Parent = parent;
            IsSelected = false;

            Parent.Parent.PropertyChanged += new PropertyChangedEventHandler(_timeLine_PropertyChanged);

            short hueShift = 0;
            switch (type)
            {
                case TimeLineComponentType.CameraFx:
                    Name = "Camera Fx";
                    hueShift = 123;
                    break;
                case TimeLineComponentType.ParticleSystem:
                    Name = "Particle System";
                    hueShift = 281;
                    break;
                default:
                    break;
            }

            LinearGradientBrush brush = new LinearGradientBrush();
            brush.StartPoint = new Point(0.5, 0.0);
            brush.EndPoint = new Point(0.5, 1.0);
            brush.GradientStops.Add(new GradientStop(ColorExtensions.FromHsb((short)(9 + hueShift), 0.45, 0.63), 0.0));
            brush.GradientStops.Add(new GradientStop(ColorExtensions.FromHsb((short)(6 + hueShift), 0.50, 0.70), 0.25));
            brush.GradientStops.Add(new GradientStop(ColorExtensions.FromHsb((short)(5 + hueShift), 0.53, 0.74), 0.46));
            brush.GradientStops.Add(new GradientStop(ColorExtensions.FromHsb((short)(12 + hueShift), 0.35, 0.80), 0.67));
            brush.GradientStops.Add(new GradientStop(ColorExtensions.FromHsb((short)(27 + hueShift), 0.20, 0.93), 1.0));

            LinearGradientBrush selectedBrush = new LinearGradientBrush();
            selectedBrush.StartPoint = new Point(0.5, 0.0);
            selectedBrush.EndPoint = new Point(0.5, 1.0);
            selectedBrush.GradientStops.Add(new GradientStop(ColorExtensions.FromHsb((short)(9 + hueShift), 0.45, 1.0), 0.0));
            selectedBrush.GradientStops.Add(new GradientStop(ColorExtensions.FromHsb((short)(6 + hueShift), 0.50, 0.90), 0.25));
            selectedBrush.GradientStops.Add(new GradientStop(ColorExtensions.FromHsb((short)(5 + hueShift), 0.53, 0.80), 0.46));
            selectedBrush.GradientStops.Add(new GradientStop(ColorExtensions.FromHsb((short)(12 + hueShift), 0.35, 0.90), 0.67));
            selectedBrush.GradientStops.Add(new GradientStop(ColorExtensions.FromHsb((short)(27 + hueShift), 0.20, 0.40), 1.0));

            _color = brush;
            _selectedColor = selectedBrush;
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

        private bool _canSelect = false;
        private bool _isDragging = false;
        private Vector _clickStart;
        internal void LeftPressed(object sender, MouseButtonEventArgs e)
        {
            _canSelect = true;
            _clickStart = (Vector)e.GetPosition(sender as Canvas);
            _isDragging = true;
            Canvas canvas = sender as Canvas;
            canvas.CaptureMouse();
        }
        internal void LeftReleased(object sender, MouseButtonEventArgs e)
        {
            if (_isDragging)
            {
                if (_canSelect)
                    IsSelected = !IsSelected;
                _canSelect = false;
                _isDragging = false;
                Canvas canvas = sender as Canvas;
                canvas.ReleaseMouseCapture();
            }
        }
        internal void MouseMove(object sender, MouseEventArgs e)
        {
            if (_isDragging)
            {
                Canvas canvas = sender as Canvas;
                Vector move = (Vector)e.GetPosition(canvas) - _clickStart;
                if (Math.Abs(move.X) > 5 || _canSelect == false)
                {
                    IsSelected = true;
                    Parent.Parent.TimeMoveSelected(move.X / Parent.Parent.TimeScale);
                    _canSelect = false;
                }
            }
        }
    }
}
