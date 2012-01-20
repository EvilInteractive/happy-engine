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
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables;

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
            DataContextChanged += (s, e) =>
            {
                if (e.OldValue != null)
                {
                    PropertyViewerItemContext item = (PropertyViewerItemContext)e.OldValue;
                    item.PropertyChanged -= ItemPropertyChanged;
                }
                if (e.NewValue != null)
                {
                    PropertyViewerItemContext item = (PropertyViewerItemContext)e.NewValue;
                    item.PropertyChanged += ItemPropertyChanged;
                }
                UpdateCheckboxes();
            };
        }

        void ItemPropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == PropertyViewerItemContext.VariableProperty)
                UpdateCheckboxes();
        }

        private void UpdateCheckboxes()
        {
            if (DataContext != null)
            {
                PropertyViewerItemContext item = (PropertyViewerItemContext)DataContext;
                if (item.Variable as CurveVarContext != null)
                {
                    CurveVarContext v = item.Variable as CurveVarContext;
                    _chkX.Visibility = Visibility.Visible;
                    _chkX.Content = v.Points[0].Variable.Value[0].Name;

                    if (v.Points[0].Variable.Value.Count > 1)
                    {
                        _chkY.Visibility = Visibility.Visible;
                        _chkY.Content = v.Points[0].Variable.Value[1].Name;
                    }
                    else
                    {
                        _chkY.Visibility = Visibility.Hidden;
                    }

                    if (v.Points[0].Variable.Value.Count > 2)
                    {
                        _chkZ.Visibility = Visibility.Visible;
                        _chkZ.Content = v.Points[0].Variable.Value[2].Name;
                    }
                    else
                    {
                        _chkZ.Visibility = Visibility.Hidden;
                    }

                    if (v.Points[0].Variable.Value.Count > 3)
                    {
                        _chkW.Visibility = Visibility.Visible;
                        _chkW.Content = v.Points[0].Variable.Value[3].Name;
                    }
                    else
                    {
                        _chkW.Visibility = Visibility.Hidden;
                    }
                }
                else
                {
                    _chkX.Visibility = Visibility.Hidden;
                    _chkY.Visibility = Visibility.Hidden;
                    _chkZ.Visibility = Visibility.Hidden;
                    _chkW.Visibility = Visibility.Hidden;
                }
            }
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
