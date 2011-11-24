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

namespace HappyFxEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private HEConnectionManager _heConnenctionManager;
        private GuiManager _guiManager;

        public MainWindow()
        {
            InitializeComponent();
            _guiManager = new GuiManager();
            _heConnenctionManager = new HEConnectionManager(_guiManager);
            _statusConnectionState.DataContext = _heConnenctionManager;
            Closed += new EventHandler(OnClose);
        }

        void OnClose(object sender, EventArgs e)
        {
            _heConnenctionManager.Dispose();
        }

        private void OnExit(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            _heConnenctionManager.Connect();
        }
    }
}
