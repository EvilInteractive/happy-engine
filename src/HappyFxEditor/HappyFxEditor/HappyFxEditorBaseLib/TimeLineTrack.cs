using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HappyFxEditorBaseLib.Net;

namespace HappyFxEditorBaseLib
{
    public class TimeLineTrack
    {
        private readonly TimeLine _timeLine;
        private readonly Guid _guid = Guid.NewGuid();

        public TimeLineTrack(TimeLine timeLine)
        {
            _timeLine = timeLine;
            Id = uint.MaxValue;

            if (_timeLine.IsCreated)
            {
                _timeLine.Created += (s, e) =>
                {
                    HeConnectionManager.Instance.PacketReceived += HandleTimeLineTrackCreated;

                    Packet p = new Packet(FxOutHeader.CreateTimeLineTrack);
                    p.Add(_guid);
                    HeConnectionManager.Instance.SendPacket(p);
                };
            }
        }
        
        //////////////////////////////////////////////////////////////
        //                         Properties                       //
        //////////////////////////////////////////////////////////////
        public uint Id { get; private set; }
        public bool IsCreated { get { return Id != uint.MaxValue; } }


        //////////////////////////////////////////////////////////////
        //                           Methods                        //
        //////////////////////////////////////////////////////////////
        private void HandleTimeLineTrackCreated(FxInHeader header, Packet packet)
        {
            if (header != FxInHeader.TimeLineTrackCreated) return;
            if (packet.ReadGuid(true) != _guid) return;

            packet.ReadGuid();
            Id = packet.ReadUInt();
            HeConnectionManager.Instance.PacketReceived -= HandleTimeLineTrackCreated;
            OnCreated();
        }

        //////////////////////////////////////////////////////////////
        //                           Events                         //
        //////////////////////////////////////////////////////////////
        void OnCreated()
        {
            EventHandler createHandler = Created;
            if (createHandler != null)
                createHandler(this, EventArgs.Empty);
        }
        public event EventHandler Created;
    }
}
