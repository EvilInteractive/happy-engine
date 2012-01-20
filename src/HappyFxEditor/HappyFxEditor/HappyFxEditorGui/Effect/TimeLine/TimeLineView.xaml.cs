using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using HappyFxEditorContextLib.Effect;

namespace HappyFxEditorApp.Effect.TimeLine
{
    /// <summary>
    /// Interaction logic for TimeLineView.xaml
    /// </summary>
    public partial class TimeLineView : UserControl
    {
        public TimeLineView()
        {
            InitializeComponent();
        }
        private void HorzScrollbarScroll(object sender, System.Windows.Controls.Primitives.ScrollEventArgs e)
        {
            _trackContainer.HorizontalScroll = e.NewValue;
        }

        private void OnMouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            ((EffectContext)(((UserControl)sender).DataContext)).TimeLine.SetSelectTool();
        }

        private void TrackContainerOnSizeChanged(object sender, SizeChangedEventArgs e)
        {
            ((EffectContext) (((UserControl) sender).DataContext)).TimeLine.ScrollViewPort = ActualWidth;
        }
    }
}
