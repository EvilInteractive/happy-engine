using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;

namespace HappyFxEditor
{
    class HEConnectionManager
    {
        TcpClient _client;

        public bool Connected { get; private set; }
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

        public HEConnectionManager()
        {
            Connected = false;
            _client.BeginConnect("localhost", 3250, ConnectedCallback, null);
        }

        void ConnectedCallback(object connectionInfo)
        {
            Connected = true;
        }
    }
}
