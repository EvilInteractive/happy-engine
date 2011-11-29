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
        
        private void PlayButton_Click(object sender, RoutedEventArgs e)
        {
            if (_heConnenctionManager.Connected == false)
                _heConnenctionManager.Connect();
            if (_heConnenctionManager.Connected)
            {
                Packet p = new Packet(4, FxOutHeader.Play);
                _heConnenctionManager.SendPacket(p);
            }
        }
        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            if (_heConnenctionManager.Connected == false)
                _heConnenctionManager.Connect();
            if (_heConnenctionManager.Connected)
            {
                Packet p = new Packet(4, FxOutHeader.Stop);
                _heConnenctionManager.SendPacket(p);
            }
        }

        private void PauzeButton_Click(object sender, RoutedEventArgs e)
        {
            if (_heConnenctionManager.Connected == false)
                _heConnenctionManager.Connect();
            if (_heConnenctionManager.Connected)
            {
                Packet p = new Packet(4, FxOutHeader.Pauze);
                _heConnenctionManager.SendPacket(p);
            }
        }
    }
}
