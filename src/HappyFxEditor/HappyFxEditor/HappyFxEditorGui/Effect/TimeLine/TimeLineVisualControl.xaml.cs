using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using HappyFxEditorContextLib.Effect.TimeLine;

namespace HappyFxEditorApp.Effect.TimeLine
{
    /// <summary>
    /// Interaction logic for TimeLineVisualControl.xaml
    /// </summary>
    public partial class TimeLineVisualControl : UserControl
    {
        private TimeLineContext _timeLine;

        private readonly List<double> _subDLookUpTable = new List<double>(){0.1, 1, 10, 100, 1000};
        private readonly int _maxSubD = 15;

        private readonly Pen _subDPen = new Pen(Brushes.White, 2);

        public TimeLineVisualControl()
        {
            DataContextChanged += (s, e) =>
                                      {
                                          if (e.OldValue != null)
                                          {
                                              TimeLineContext t = e.OldValue as TimeLineContext;
                                              if (t != null)
                                                t.PropertyChanged -= DataContextPropertyChanged;
                                          }
                                          if (e.NewValue != null)
                                          {
                                              TimeLineContext t = e.NewValue as TimeLineContext;
                                              if (t != null)
                                                  t.PropertyChanged += DataContextPropertyChanged;
                                              _timeLine = t;
                                          }
                                          else
                                          {
                                              _timeLine = null;
                                          }
                                      };
            InitializeComponent();
        }

        void DataContextPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            InvalidateVisual();
        }


        protected override void OnRender(DrawingContext drawingContext)
        {
            if (_timeLine != null)
            {
                double minValue = _timeLine.ScrollWidth > 0.0
                                      ? _timeLine.Scroll/_timeLine.TimeScale
                                      : 0.0;
                double maxValue = _timeLine.ScrollWidth > 0.0
                                      ? (_timeLine.Scroll + _timeLine.ScrollViewPort)/_timeLine.TimeScale
                                      : _timeLine.ScrollViewPort/_timeLine.TimeScale;

                double subD = _subDLookUpTable[_subDLookUpTable.Count - 1];

                foreach (var s in _subDLookUpTable)
                {
                    if ((maxValue - minValue) / s < _maxSubD)
                    {
                        subD = s;
                        break;
                    }
                }

                double offset = subD - (minValue%subD);
                
                Typeface typeFace = new Typeface(FontFamily, FontStyle, FontWeight, FontStretch);
                for (; offset <= maxValue; offset+=subD)
                {
                    drawingContext.DrawLine(_subDPen, new Point(offset * _timeLine.TimeScale, 0), new Point(offset * _timeLine.TimeScale, ActualHeight));
                    drawingContext.DrawText(new FormattedText((minValue + offset).ToString("0.00"), CultureInfo.CurrentCulture, FlowDirection.LeftToRight, typeFace, 12, Brushes.White),
                                            new Point(offset * _timeLine.TimeScale + 4, ActualHeight - 16));
                }

                drawingContext.DrawText(new FormattedText(minValue.ToString("0.000"), CultureInfo.CurrentCulture, FlowDirection.LeftToRight, typeFace, 12, Brushes.White),
                                        new Point(0, 0));
                drawingContext.DrawText(new FormattedText(maxValue.ToString("0.000"), CultureInfo.CurrentCulture, FlowDirection.RightToLeft, typeFace, 12, Brushes.White),
                                        new Point(ActualWidth, 0));

            }
        }
    }
}
