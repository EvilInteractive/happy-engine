using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Collections.ObjectModel;
using System.Windows.Controls;
using System.Windows.Shapes;
using System.Windows.Media;
using System.Windows.Input;

namespace HappyFxEditor
{
    class TimeLine : INotifyPropertyChanged
    {
        private double _timeScale;
        public double TimeScale 
        {
            get { return _timeScale; }
            private set
            {
                if (_timeScale != value)
                {
                    _timeScale = value;
                    OnPropertyChanged("TimeScale");
                    UpdateVisual();
                }
            }
        }


        public ObservableCollection<TimeLineTrack> Tracks { get; private set; }

        
        private Canvas _visual;

        public TimeLine(Canvas visual)
        {
            Tracks = new ObservableCollection<TimeLineTrack>();
            TimeScale = 50;
            _visual = visual;
            Tracks.Add(new TimeLineTrack(this));
            Tracks.Add(new TimeLineTrack(this));
            Tracks.Add(new TimeLineTrack(this));
            Tracks.Add(new TimeLineTrack(this));
            Tracks.Add(new TimeLineTrack(this));

            visual.MouseDown += new MouseButtonEventHandler(visual_MouseDown);
            visual.MouseMove += new MouseEventHandler(visual_MouseMove);
            visual.MouseUp += new MouseButtonEventHandler(visual_MouseUp);
        }

#region Dragging
        private bool _isDragging = false;
        private double _dragStartY = 0.0;
        private double _dragStartScale = 0.0;
        void visual_MouseDown(object sender, MouseButtonEventArgs e)
        {
            _isDragging = true;
            _visual.CaptureMouse();
            _dragStartY = e.GetPosition(_visual).Y;
            _dragStartScale = TimeScale;
        }
        void visual_MouseMove(object sender, MouseEventArgs e)
        {
            if (_isDragging)
            {
                double scale = e.GetPosition(_visual).Y - _dragStartY;
                TimeScale = Math.Max(0.001, _dragStartScale + scale);

            }
        }
        void visual_MouseUp(object sender, MouseButtonEventArgs e)
        {
            _isDragging = false;
            _visual.ReleaseMouseCapture();
        }
#endregion

        public void UpdateVisual()
        {
            if (_visual == null)
                return;
            if (_visual.ActualWidth < 1)
                return;

            _visual.Children.Clear();
            double width = _visual.ActualWidth;
            double time = width / TimeScale; //in seconds

            double bigSub = 1;
            while (time < bigSub * 4)
                bigSub /= 4;
            while (time > bigSub * 20)
                bigSub *= 10;

            double smallSub = bigSub / 10.0;

            for (double i = 0; i < time; i += smallSub)
            {
                Rectangle line = new Rectangle();
                line.Fill = Brushes.White;
                line.Width = 2;
                line.Height = 12;
                _visual.Children.Add(line);
                Canvas.SetLeft(line, i * TimeScale - line.Width / 2.0);
                Canvas.SetTop(line, 0);
            }
            for (double i = 0; i < time; i += bigSub)
            {
                Rectangle line = new Rectangle();
                line.Fill = Brushes.White;
                line.Width = 4;
                line.Height = 16;
                _visual.Children.Add(line);
                Canvas.SetLeft(line, i * TimeScale - line.Width / 2.0);
                Canvas.SetTop(line, 0);

                TextBlock text = new TextBlock();
                text.Text = i.ToString("#.00");
                text.Foreground = Brushes.White;
                _visual.Children.Add(text);
                Canvas.SetLeft(text, i * TimeScale - line.Width / 2.0);
                Canvas.SetTop(text, 16);
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
