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
using HappyFxEditorContextLib.Browsers;

namespace HappyFxEditorApp.Browsers
{
    /// <summary>
    /// Interaction logic for AssetManager.xaml
    /// </summary>
    public partial class AssetManager : UserControl
    {
        public AssetManager()
        {
            InitializeComponent();
        }

        private void TreeViewSelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            ((AssetManagerContext) DataContext).SelectedAsset = e.NewValue as Asset;
        }
    }
}
