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
        private TimeLine _timeLine;

        public MainWindow()
        {
            InitializeComponent();
            _guiManager = new GuiManager();
            _heConnenctionManager = new HEConnectionManager(_guiManager);
            _statusConnectionState.DataContext = _heConnenctionManager;
            Closed += new EventHandler(OnClose);

            _timeLine = new TimeLine(_timeLineVisualizer);
            _timelineEditor.DataContext = _timeLine;

            Loaded += OnLoad;
        }

        void OnLoad(object sender, EventArgs e)
        {
            _timeLine.UpdateVisual();
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

        private void Canvas_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {

        }

        private void TrackRightClicked(object sender, MouseButtonEventArgs e)
        {
            FrameworkElement el = sender as FrameworkElement;
            TimeLineTrack track = el.DataContext as TimeLineTrack;
            if (track != null)
                track.RightClick(sender, e);
        }
        private void Component_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            FrameworkElement el = sender as FrameworkElement;
            TimeLineComponent comp = el.DataContext as TimeLineComponent;
            if (comp != null)
                comp.LeftPressed(sender, e);
        }
        private void Component_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            FrameworkElement el = sender as FrameworkElement;
            TimeLineComponent comp = el.DataContext as TimeLineComponent;
            if (comp != null)
                comp.LeftReleased(sender, e);
        }

        private void Component_MouseMove(object sender, MouseEventArgs e)
        {
            FrameworkElement el = sender as FrameworkElement;
            TimeLineComponent comp = el.DataContext as TimeLineComponent;
            if (comp != null)
                comp.MouseMove(sender, e);
        }
    }
}
