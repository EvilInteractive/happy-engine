using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.ComponentModel;
using System.Windows;

namespace HappyFxEditor
{
    class HEConnectionManager : INotifyPropertyChanged
    {
        enum FxOutHeader
        {
            GetEffectPath = 0,
            GetTexturePath,
            GetModelPath,
            GetAudioPath,
        };
        enum FxInHeader
        {
            SetEffectPath = 0,
            SetTexturePath,
            SetModelPath,
            SetAudioPath,
        };

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
                        _stream.BeginRead(_buffer, 0, MAX_BUFFER_SIZE, handleReceive, _stream);
                    }
                }
                catch (Exception e)
                { }
            }
        }

        public void Dispose()
        {
            _stream.Close();
            _client.Close();
        }

        private void handleReceive(IAsyncResult ar)
        {       
            try
            {
                int numBytesRead = _stream.EndRead(ar);
    
                //Parse buffer
                int headerSize = 4;
                FxInHeader header = (FxInHeader)BitConverter.ToUInt32(_buffer, 0);
                switch (header)
                {
                    case FxInHeader.SetEffectPath:
                        _guiManager.EffectPath = BitConverter.ToString(_buffer, headerSize, numBytesRead - headerSize);
                        break;
                    case FxInHeader.SetTexturePath:
                        _guiManager.TexturePath = BitConverter.ToString(_buffer, headerSize, numBytesRead - headerSize);
                        break;
                    case FxInHeader.SetModelPath:
                        _guiManager.ModelPath = BitConverter.ToString(_buffer, headerSize, numBytesRead - headerSize);
                        break;
                    case FxInHeader.SetAudioPath:
                        _guiManager.AudioPath = BitConverter.ToString(_buffer, headerSize, numBytesRead - headerSize);
                        break;
                    default:
                        MessageBox.Show("Unknown message received: header: " + header.ToString());
                        break;
                }
    
                _stream.BeginRead(_buffer, 0, MAX_BUFFER_SIZE, handleReceive, _stream);
            }
            catch (System.Exception)
            {
                
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
