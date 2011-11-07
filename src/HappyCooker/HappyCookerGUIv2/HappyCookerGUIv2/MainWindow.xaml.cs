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
using System.Collections.ObjectModel;
using System.Collections;
using System.IO;

namespace HappyCookerGUIv2
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Model _model = new Model();

        public MainWindow()
        {
            InitializeComponent();

            _modelImportListBox.DataContext = _model;
            _modelUpdateListBox.DataContext = _model;
            _modelExportedListBox.DataContext = _model;

            _modelImportPathTB.DataContext = _model;
            _modelExportPathTB.DataContext = _model;

            _modelOutput.DataContext = _model;

            _modelProgressBar.DataContext = _model;
            _modelInfoLabel.DataContext = _model;

            _physicsExportPathTB.DataContext = _model;
            
            StreamReader stream = null;
            try
            {
                stream = new StreamReader("config.cfg");
                _model.Deserialize(stream);
            }
            catch (Exception)
            {}
            if (stream != null)
                stream.Close();
        }

        private void moveModelItemFromLBtoL(IList items, ObservableCollection<ModelItem> from, ObservableCollection<ModelItem> to)
        {
            List<ModelItem> tempList = new List<ModelItem>(items.Count);

            foreach (ModelItem item in items)
            {
                to.Add(item);
                tempList.Add(item);
            }
            foreach (ModelItem item in tempList)
            {
                from.Remove(item);
            }
        }
    
        private void _modelShiftBtn_IM_UP_Click(object sender, RoutedEventArgs e)
        {
            moveModelItemFromLBtoL(_modelImportListBox.SelectedItems, _model.ImportItems, _model.UpdateItems);
        }

        private void _modelShiftBtn_UP_IM_Click(object sender, RoutedEventArgs e)
        {
            moveModelItemFromLBtoL(_modelUpdateListBox.SelectedItems, _model.UpdateItems, _model.ImportItems);
        }

        private void _modelImportPathBtn_Click(object sender, RoutedEventArgs e)
        {
            _model.GetImportPath();
        }

        private void _modelExportPathBtn_Click(object sender, RoutedEventArgs e)
        {
            _model.GetModelExportPath();
        }

        private void _physicsExportPathBtn_Click(object sender, RoutedEventArgs e)
        {
            _model.GetPhysicsExportPath();
        }

        private void _modelRefreshBtn_Click(object sender, RoutedEventArgs e)
        {
            _model.Refresh();
        }

        private void _modelCookBtn_Click(object sender, RoutedEventArgs e)
        {
            _model.Cook();
        }

        private void _modelRemoveBtn_Click(object sender, RoutedEventArgs e)
        {
            foreach (ModelItem item in _modelExportedListBox.SelectedItems)
            {
                foreach (string ext in _model.ModelExportExtensions)
                {
                    try { File.Delete(_model.ModelExportPath + item.ShortPath + ext); }
                    catch (Exception) { /* something happened but we don't care */ }
                }
                foreach (string ext in _model.PhysicsExportExtensions)
                {
                    try { File.Delete(_model.PhysicsExportPath + item.ShortPath + ext); }
                    catch (Exception) { /* something happened but we don't care */ }
                }
            }

            _model.Refresh();
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            StreamWriter stream = new StreamWriter("config.cfg");
            _model.Serialize(stream);
            stream.Close();
        }


    }
}
