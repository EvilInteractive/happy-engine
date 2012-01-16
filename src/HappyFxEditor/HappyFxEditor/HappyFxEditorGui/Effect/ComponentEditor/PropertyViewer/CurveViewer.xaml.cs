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
using System.Windows.Threading;

namespace HappyFxEditorApp.Effect.ComponentEditor.PropertyViewer
{
    /// <summary>
    /// Interaction logic for CurveViewer.xaml
    /// </summary>
    public partial class CurveViewer : UserControl
    {
        public CurveViewer()
        {
            InitializeComponent();
        }

        private void CheckBoxCheckedChange(object sender, RoutedEventArgs e)
        {
            if (_curve != null)
                _curve.InvalidateVisual();
        }

        private void TextBoxLostFocus(object sender, RoutedEventArgs e)
        {
            if (_curve != null)
                _curve.InvalidateVisual();
        }
        
        private void TextBoxPreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                MoveFocus(new TraversalRequest(FocusNavigationDirection.Left));
                ((TextBox) sender).Focus();
            }
        }
    }
}
