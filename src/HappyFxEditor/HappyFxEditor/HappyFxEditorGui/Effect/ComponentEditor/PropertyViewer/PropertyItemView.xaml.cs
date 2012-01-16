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
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer;

namespace HappyFxEditorApp.Effect.ComponentEditor.PropertyViewer
{
    /// <summary>
    /// Interaction logic for PropertyItemView.xaml
    /// </summary>
    public partial class PropertyItemView : UserControl
    {
        public PropertyItemView()
        {
            InitializeComponent();
        }
        
        private void ExpanderGotFocus(object sender, RoutedEventArgs e)
        {
            PropertyViewerItemContext item = (PropertyViewerItemContext) DataContext;
            item.Group.SelectedItem = item;
        }
    }
}
