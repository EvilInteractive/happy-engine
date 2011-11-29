using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.ComponentModel;
using System.Windows;

namespace HappyFxEditor
{
    public enum FxOutHeader
    {
        GetEffectPath = 0,
        GetTexturePath,
        GetModelPath,
        GetAudioPath,
        Play,
        Stop,
        Pauze
    };
    public enum FxInHeader
    {
        SetEffectPath = 0,
        SetTexturePath,
        SetModelPath,
        SetAudioPath,
    };
    public class HEConnectionManager : INotifyPropertyChanged
    {

        private TcpClient _client;
        private NetworkStream _stream;

        private GuiManager _guiManager;

        private const int MAX_BUFFER_SIZE = 1024;
        private byte[] _buffer = new byte[MAX_BUFFER_SIZE];

        public bool Connected 
        {
            get { return _client.Connected; }
        }
        public string ConnectionInfo
        {
            get 
            {
                if (Connected)
                    return "Connected!";
                else
                    return "Not Connected!";
            }
        }

        public HEConnectionManager(GuiManager guiManaer)
        {
            _client = new TcpClient();
            _guiManager = guiManaer;
        }

        public void Connect()
        {
            if (Connected == false)
            {
                try
                {
                    _client.Connect("localhost", 3250);
                    PropertyChanged(this, new PropertyChangedEventArgs("ConnectionInfo"));
                    if (Connected)
                    {
                        _stream = _client.GetStream();
                        _stream.BeginRead(_buffer, 0, MAX_BUFFER_SIZE, HandleReceive, _stream);
                    }
                }
                catch (Exception /*e*/)
                { }
            }
        }

        public void Dispose()
        {
            _stream.Close();
            _client.Close();
        }

        private void HandleReceive(IAsyncResult ar)
        {       
            try
            {
                int numBytesRead = _stream.EndRead(ar);
    
                //Parse buffer
                Packet p = new Packet(_buffer);
                FxInHeader header = p.ReadHeader();
                switch (header)
                {
                    case FxInHeader.SetEffectPath:
                        _guiManager.EffectPath = p.ReadString();
                        break;
                    case FxInHeader.SetTexturePath:
                        _guiManager.TexturePath = p.ReadString();
                        break;
                    case FxInHeader.SetModelPath:
                        _guiManager.ModelPath = p.ReadString();
                        break;
                    case FxInHeader.SetAudioPath:
                        _guiManager.AudioPath = p.ReadString();
                        break;
                    default:
                        MessageBox.Show("Unknown message received: header: " + header.ToString());
                        break;
                }
    
                _stream.BeginRead(_buffer, 0, MAX_BUFFER_SIZE, HandleReceive, _stream);
            }
            catch (System.Exception)
            {
                
            }
        }

        public void SendPacket(Packet packet)
        {
            if (Connected)
            {
                _stream.Write(packet.GetBuffer(), 0, packet.GetSize());
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
