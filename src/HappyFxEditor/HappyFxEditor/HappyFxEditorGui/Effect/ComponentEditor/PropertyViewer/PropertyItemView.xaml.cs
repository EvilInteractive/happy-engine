using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
using HappyFxEditorApp.Effect.ComponentEditor.PropertyViewer.Variable;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer;
using HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables;

namespace HappyFxEditorApp.Effect.ComponentEditor.PropertyViewer
{
    /// <summary>
    /// Interaction logic for PropertyItemView.xaml
    /// </summary>
    public partial class PropertyItemView : UserControl
    {
        public ObservableCollection<UserControl> Items { get; private set; }

        public PropertyItemView()
        {
            Items = new ObservableCollection<UserControl>();
            DataContextChanged += (s, e) =>
                                      {
                                          if (e.OldValue != null)
                                          {
                                              PropertyViewerItemContext item = e.OldValue as PropertyViewerItemContext;
                                              if (item != null)
                                                item.PropertyChanged -= ItemPropertyChanged;
                                          }
                                          if (e.NewValue != null)
                                          {
                                              PropertyViewerItemContext item = e.NewValue as PropertyViewerItemContext;
                                              if (item != null)
                                                  item.PropertyChanged += ItemPropertyChanged;
                                          }
                                          UpdateVar();
                                      };
            InitializeComponent();
        }

        void ItemPropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == PropertyViewerItemContext.VariableProperty)
            {
                UpdateVar();
            }
        }
        
        public void UpdateVar()
        {
            Items.Clear();
            if (DataContext != null)
            {
                PropertyViewerItemContext item = DataContext as PropertyViewerItemContext;

                if (item != null)
                {
                    switch (item.Variable.GetVarType())
                    {
                        case VariableType.Curve:
                            Items.Add(new CurveVarView() { DataContext = item.Variable });
                            break;
                        case VariableType.Random:
                            Items.Add(new RandomVarView() { DataContext = item.Variable });
                            break;
                        case VariableType.Constant:
                            Items.Add(new ConstantVarView() { DataContext = item.Variable });
                            break;
                        case VariableType.Asset:
                            Items.Add(new AssetVarView() { DataContext = item.Variable });
                            break;
                        case VariableType.Choose:
                            break;
                        default:
                            throw new ArgumentOutOfRangeException();
                    }
                }
            }
        }

        private void ExpanderGotFocus(object sender, RoutedEventArgs e)
        {
            PropertyViewerItemContext item = (PropertyViewerItemContext) DataContext;
            item.Group.SelectedItem = item;
        }
    }
}
