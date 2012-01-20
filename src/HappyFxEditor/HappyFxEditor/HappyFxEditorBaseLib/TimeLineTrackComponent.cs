using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using HappyFxEditorBaseLib.Net;

namespace HappyFxEditorBaseLib
{
    public enum TimeLineTrackComponentType
    {
        ParticleSystem = 0,
        CameraFx,
        AudioFx,
        MAX_TYPES
    }
    public class TimeLineTrackComponent
    {
        private readonly TimeLineTrack _timeLineTrack;
        private readonly TimeLineTrackComponentType _type;
        private readonly Guid _guid = Guid.NewGuid();

        public TimeLineTrackComponent(TimeLineTrack timeLineTrack, TimeLineTrackComponentType type)
        {
            _timeLineTrack = timeLineTrack;
            _type = type;
            Id = uint.MaxValue;

            if (_timeLineTrack.IsCreated)
            {
                _timeLineTrack.Created += (s, e) =>
                {
                    HeConnectionManager.Instance.PacketReceived += HandleTimeLineTrackComponentCreated;

                    Packet p = new Packet(FxOutHeader.CreateTimeLineTrackComponent);
                    p.Add(_guid);
                    p.Add((uint)_type);
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
        private void HandleTimeLineTrackComponentCreated(FxInHeader header, Packet packet)
        {
            if (header != FxInHeader.TimeLineTrackComponentCreated) return;
            if (packet.ReadGuid(true) != _guid) return;

            packet.ReadGuid();
            Id = packet.ReadUInt();
            HeConnectionManager.Instance.PacketReceived -= HandleTimeLineTrackComponentCreated;
        }


    }
}
