using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
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
using HappyFxEditorBaseLib;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types;

namespace HappyFxEditorApp.Effect.ComponentEditor.PropertyViewer.Variable
{
    /// <summary>
    /// Interaction logic for CurveView.xaml
    /// </summary>
    public partial class CurveView : UserControl
    {
        public IEnumerable<CurvePointContext> CurvePoints
        {
            get { return (IEnumerable<CurvePointContext>)GetValue(CurvePointsProperty); }
            set { SetValue(CurvePointsProperty, value); }
        }
        public static readonly DependencyProperty CurvePointsProperty =
            DependencyProperty.Register("CurvePoints", typeof(IEnumerable<CurvePointContext>), typeof(CurveView), new UIPropertyMetadata(null));

        public double XRange
        {
            get { return (double)GetValue(XRangeProperty); }
            set { SetValue(XRangeProperty, value); }
        }
        public static readonly DependencyProperty XRangeProperty =
            DependencyProperty.Register("XRange", typeof(double), typeof(CurveView), new UIPropertyMetadata(1.0));

        public double YRange
        {
            get { return (double)GetValue(YRangeProperty); }
            set { SetValue(YRangeProperty, value); }
        }
        public static readonly DependencyProperty YRangeProperty =
            DependencyProperty.Register("YRange", typeof(double), typeof(CurveView), new UIPropertyMetadata(1.0));

        public double XOffset
        {
            get { return (double)GetValue(XOffsetProperty); }
            set { SetValue(XOffsetProperty, value); }
        }
        public static readonly DependencyProperty XOffsetProperty =
            DependencyProperty.Register("XOffset", typeof(double), typeof(CurveView), new UIPropertyMetadata(0.0));

        public double YOffset
        {
            get { return (double)GetValue(YOffsetProperty); }
            set { SetValue(YOffsetProperty, value); }
        }
        public static readonly DependencyProperty YOffsetProperty =
            DependencyProperty.Register("YOffset", typeof(double), typeof(CurveView), new UIPropertyMetadata(0.0));



        public bool XVisible
        {
            get { return (bool)GetValue(XVisibleProperty); }
            set { SetValue(XVisibleProperty, value); }
        }
        public static readonly DependencyProperty XVisibleProperty =
            DependencyProperty.Register("XVisible", typeof(bool), typeof(CurveView), new UIPropertyMetadata(true));
        public bool YVisible
        {
            get { return (bool)GetValue(YVisibleProperty); }
            set { SetValue(YVisibleProperty, value); }
        }
        public static readonly DependencyProperty YVisibleProperty =
            DependencyProperty.Register("YVisible", typeof(bool), typeof(CurveView), new UIPropertyMetadata(true));
        public bool ZVisible
        {
            get { return (bool)GetValue(ZVisibleProperty); }
            set { SetValue(ZVisibleProperty, value); }
        }
        public static readonly DependencyProperty ZVisibleProperty =
            DependencyProperty.Register("ZVisible", typeof(bool), typeof(CurveView), new UIPropertyMetadata(true));
        public bool WVisible
        {
            get { return (bool)GetValue(WVisibleProperty); }
            set { SetValue(WVisibleProperty, value); }
        }
        public static readonly DependencyProperty WVisibleProperty =
            DependencyProperty.Register("WVisible", typeof(bool), typeof(CurveView), new UIPropertyMetadata(true));

        

        public double XScale { get { return ActualWidth / XRange; } }
        public double YScale { get { return ActualHeight / YRange; } }

        private readonly Pen _xPen = new Pen(Brushes.Red, 2);
        private readonly Pen _yPen = new Pen(Brushes.Green, 2);
        private readonly Pen _zPen = new Pen(Brushes.Blue, 2);
        private readonly Pen _wPen = new Pen(Brushes.Purple, 2);
        private readonly Pen _gridPen = new Pen(Brushes.Black, 1);

        private FloatType _selectedType;
        private CurvePointContext _selectedPoint;
        private IDisposable _pointDragTransaction;

        private bool _isPanning;
        private double _startOffx, _startOffy;
        private Point _panStartPoint;

        public CurveView()
        {
            DataContextChanged += (s, e) =>
            {
                InvalidateVisual();
                UpdatePointEventHandlers(e.OldValue as PropertyViewerItemContext,
                                         e.NewValue as PropertyViewerItemContext);
            };
            InitializeComponent();
        }

        void VarTypeChanged(object sender, EventArgs e)
        {
            UpdatePointEventHandlers(DataContext as PropertyViewerItemContext,
                                     DataContext as PropertyViewerItemContext);
        }
        void UpdatePointEventHandlers(PropertyViewerItemContext oldValue, PropertyViewerItemContext newValue)
        {
            if (oldValue != null)
            {
                oldValue.PropertyChanged -= VarTypeChanged;
                CurveVarContext curve = oldValue.Variable as CurveVarContext;
                if (curve != null)
                {
                    curve.Points.CollectionChanged -= PointCollectionChanged;
                    foreach (var point in curve.Points)
                    {
                        point.PropertyChanged -= PointPropertyChanged;

                        foreach (var value in point.Variable.Value)
                        {
                            value.PropertyChanged -= PointPropertyChanged;
                        }
                    }
                }
            }
            if (newValue != null)
            {
                newValue.PropertyChanged += VarTypeChanged;
                CurveVarContext curve = newValue.Variable as CurveVarContext;
                if (curve != null)
                {
                    curve.Points.CollectionChanged += PointCollectionChanged;
                    foreach (var point in curve.Points)
                    {
                        point.PropertyChanged += PointPropertyChanged;
                        foreach (var value in point.Variable.Value)
                        {
                            value.PropertyChanged += PointPropertyChanged;
                        }
                    }
                }
            }
            InvalidateVisual();
        }

        void PointPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            InvalidateVisual();
        }

        private void PointCollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            InvalidateVisual();
            UpdatePointEventHandlers(DataContext as PropertyViewerItemContext,
                                     DataContext as PropertyViewerItemContext);
        }


        private Point ViewToContextCoords(Point p)
        {
            TransformGroup transform = new TransformGroup();
            transform.Children.Add(new TranslateTransform(0, -ActualHeight));
            transform.Children.Add(new ScaleTransform(1, -1));
            p = transform.Transform(p);
            p.X = p.X / XScale - XOffset;
            p.Y = p.Y / YScale - YOffset;
            return p;
        }

        private void CurveMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (_isPanning)
                return;
            if (_selectedPoint != null)
                return;

            Point mousePoint = e.GetPosition(this);
            if (e.MiddleButton == MouseButtonState.Pressed)
            {
                _panStartPoint = ViewToContextCoords(mousePoint);
                _startOffx = XOffset;
                _startOffy = YOffset;
                _isPanning = true;
                CaptureMouse();
                InvalidateVisual();
            }
            else if (e.LeftButton == MouseButtonState.Pressed)
            {
                mousePoint = ViewToContextCoords(mousePoint);

                double selectSizeX = 4/XScale;
                double selectSizeY = 4/YScale;

                foreach (var point in CurvePoints)
                {
                    if (point.Time - selectSizeX < mousePoint.X && point.Time + selectSizeX > mousePoint.X)
                    {
                        int count = 0;
                        foreach (FloatType comp in point.Variable.Value)
                        {
                            switch (count++)
                            {
                                case 0: if (XVisible == false) continue;
                                    break;
                                case 1: if (YVisible == false) continue;
                                    break;
                                case 2: if (ZVisible == false) continue;
                                    break;
                                case 3: if (WVisible == false) continue;
                                    break;
                            }
                            if (comp.Value - selectSizeY < mousePoint.Y && comp.Value + selectSizeY > mousePoint.Y)
                            {
                                _selectedType = comp;
                                _selectedPoint = point;

                                _selectedPoint.IsSelected = true;
                                _pointDragTransaction = _selectedPoint.Evolution.BeginTransaction();
                                CaptureMouse();
                                InvalidateVisual();
                                return;
                            }
                        }
                    }
                }
            }
        }
        private void CurveMouseUp(object sender, MouseButtonEventArgs e)
        {
            CurveMouseMove(sender, e);
            if (_isPanning && e.MiddleButton == MouseButtonState.Released ||
                _selectedPoint != null && e.LeftButton == MouseButtonState.Released)
            {
                _isPanning = false;
                _selectedPoint = null;
                _selectedType = null;
                _pointDragTransaction.Dispose();
                ReleaseMouseCapture();
                InvalidateVisual();
            }
        }
        private void CurveMouseMove(object sender, MouseEventArgs e)
        {
            Point mousePoint = ViewToContextCoords(e.GetPosition(this));

            if (_isPanning)
            {
                YOffset = mousePoint.Y - _panStartPoint.Y + _startOffy;
                InvalidateVisual();
            }
            else if (_selectedPoint != null)
            {
                if (_selectedPoint.IsTimeLocked == false)
                    _selectedPoint.Time = (float)mousePoint.X;
                _selectedType.Value = (float)mousePoint.Y;

                InvalidateVisual();
            }
        }

        private Pen GetPen(int comp)
        {
            switch (comp)
            {
                case 0:
                    return _xPen;
                case 1:
                    return _yPen;
                case 2:
                    return _zPen;
                case 3:
                    return _wPen;
                default:
                    return _xPen;
            }
        }

        private void DrawGrid(DrawingContext drawingContext)
        {
            double xSub = XRange / 10 * XScale,
                   ySub = YRange / 10 * YScale,
                   xOff = XOffset * XScale,
                   yOff = YOffset * YScale;

            for (double x = 0; x < ActualWidth; x += xSub)
            {
                for (double y = 0; y < ActualHeight; y += ySub)
                {
                    drawingContext.DrawLine(_gridPen, new Point(0, y + yOff), new Point(ActualWidth, y + yOff));
                    drawingContext.DrawLine(_gridPen, new Point(x + xOff, 0), new Point(x + xOff, ActualHeight));
                }
            }
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            if (CurvePoints != null && CurvePoints.Count() > 0)
            {
                List<CurvePointContext> points = CurvePoints.ToList();
                points.Sort((a, b) => a.Time < b.Time? -1 : a.Time > b.Time? 1 : 0);



                #region Set Background
                LinearGradientBrush background = new LinearGradientBrush();
                background.StartPoint = new Point(0, 0);
                background.EndPoint = new Point(1, 0);

                foreach (var point in points)
                {
                    Color c = new Color();
                    c.A = 255;
                    for (int i = 0; i < 4; ++i)
                    {
                        if (point.Variable.Value.Count <= i)
                            break;
                        switch (i)
                        {
                            case 0:
                                c.R = (byte)(MathHelper.Clamp(((FloatType)point.Variable.Value[i]).Value, 0.0f, 1.0f) * 255);
                                break;
                            case 1:
                                c.G = (byte)(MathHelper.Clamp(((FloatType)point.Variable.Value[i]).Value, 0.0f, 1.0f) * 255);
                                break;
                            case 2:
                                c.B = (byte)(MathHelper.Clamp(((FloatType)point.Variable.Value[i]).Value, 0.0f, 1.0f) * 255);
                                break;
                            case 3:
                                c.A = (byte)(MathHelper.Clamp(((FloatType)point.Variable.Value[i]).Value, 0.0f, 1.0f) * 255);
                                break;
                        }
                    }
                    background.GradientStops.Add(new GradientStop(c, point.Time));
                }
                drawingContext.DrawRectangle(background, null, new Rect(0, 0, ActualWidth, ActualHeight));
                #endregion
                
                DrawGrid(drawingContext);

                drawingContext.PushTransform(new ScaleTransform(1, -1));
                drawingContext.PushTransform(new TranslateTransform(0, -ActualHeight));
                for (int i = 3; i >= 0; --i)
                {
                    switch (i)
                    {
                        case 0: if (XVisible == false) continue;
                            break;
                        case 1: if (YVisible == false) continue;
                            break;
                        case 2: if (ZVisible == false) continue;
                            break;
                        case 3: if (WVisible == false) continue;
                            break;
                    }


                    Point? prevPoint = null;
                    Pen p = GetPen(i);
                    foreach (var point in points)
                    {
                        if (point.Variable.Value.Count <= i)
                            break;
                        Point next = new Point((XOffset + point.Time) * XScale, (YOffset + ((FloatType)point.Variable.Value[i]).Value) * YScale);
                        if (prevPoint != null)
                        {
                            drawingContext.DrawLine(p, prevPoint.Value, next);
                        }

                        drawingContext.DrawRoundedRectangle(point.IsSelected ? Brushes.White : Brushes.Gray, p,
                                                            new Rect(new Point(next.X - 4, next.Y - 4),
                                                                     new Point(next.X + 4, next.Y + 4)), 2, 2);

                        prevPoint = next;
                    }
                }
                drawingContext.Pop();
            }
            base.OnRender(drawingContext);
        }
    }
}
