using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.ComponentModel;
using System.Windows;

namespace HappyFxEditorBaseLib.Net
{
    public enum FxOutHeader
    {
        GetEffectPath = 0,
        GetTexturePath,
        GetModelPath,
        GetAudioPath,
        Play,
        Stop,
        Pauze,
        ResetAll,

        //TimeLine
        CreateTimeLine,
        DeleteTimeLine,
        SetTimeLineEndTime,
        SetTimeLineCurrentTime,

        //TimeLineTrack
        CreateTimeLineTrack,
        DeleteTimeLineTrack,

        //TimeLineTrackComponent
        CreateTimeLineTrackComponent,
        DeleteTimeLineTrackComponent
    };
    public enum FxInHeader
    {
        SetEffectPath = 0,
        SetTexturePath,
        SetModelPath,
        SetAudioPath,
        TimeLineCreated,
        TimeLineTrackCreated,
        TimeLineTrackComponentCreated,
    };

    public delegate void PacketReceivedHandler(FxInHeader header, Packet p);
    public class HeConnectionManager : IDisposable
    {
        private readonly TcpClient _client = new TcpClient();
        private NetworkStream _stream;

        private static HeConnectionManager s_Instance;
        
        private const int MAX_BUFFER_SIZE = 1024;
        private readonly byte[] _buffer = new byte[MAX_BUFFER_SIZE];

        public bool Connected 
        {
            get { return _client.Connected; }
        }
        public string ConnectionInfo
        {
            get 
            {
                return Connected ? "Connected!" : "Not Connected!";
            }
        }

        private HeConnectionManager()
        {
        }
        public void Dispose()
        {
            _stream.Close();
            _client.Close();
            MessageBox.Show("HeConnectionManager Disposed");
            // TODO: check if cleaned up
        }

        public static HeConnectionManager Instance
        {
            get { return s_Instance ?? (s_Instance = new HeConnectionManager()); }
        }

        public void Connect()
        {
            if (Connected) return;
            try
            {
                _client.Connect("localhost", 3250);
                if (Connected)
                {
                    _stream = _client.GetStream();
                    _stream.BeginRead(_buffer, 0, MAX_BUFFER_SIZE, HandleReceive, _stream);
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
            }
        }


        private void HandleReceive(IAsyncResult ar)
        {       
            try
            {
                int numBytesRead = _stream.EndRead(ar);
    
                //Parse buffer
                Packet p = new Packet(_buffer);
                FxInHeader header = p.ReadHeader();

                PacketReceived(header, p);
    
                _stream.BeginRead(_buffer, 0, MAX_BUFFER_SIZE, HandleReceive, _stream);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);              
            }
        }

        public void SendPacket(Packet packet)
        {
            if (Connected)
            {
                _stream.Write(packet.GetBuffer(), 0, packet.GetSize());
            }
        }

        public event PacketReceivedHandler PacketReceived;
    }
}
