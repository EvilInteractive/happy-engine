using System;
using System.Collections.Generic;
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
using HappyFxEditorContextLib;

namespace HappyFxEditorApp
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
        private void HorzScrollbar_Scroll(object sender, System.Windows.Controls.Primitives.ScrollEventArgs e)
        {
            _trackContainer.HorizontalScroll = e.NewValue;
        }

        private void OnMouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            ((EffectContext)(((UserControl)sender).DataContext)).TimeLine.SetSelectTool();
        }
    }
}
