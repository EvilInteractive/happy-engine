using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HappyFxEditorBaseLib.Net;

namespace HappyFxEditorBaseLib
{
    public class TimeLine
    {
        private float _endTime = 0.0f;
        private float _currentTime = 0.0f;

        private readonly Guid _guid = Guid.NewGuid();

        public TimeLine()
        {
            Id = uint.MaxValue;

            HeConnectionManager.Instance.PacketReceived += HandleTimeLineCreated;

            Packet p = new Packet(FxOutHeader.CreateTimeLine);
            p.Add(_guid);
            HeConnectionManager.Instance.SendPacket(p);
        }
        
        //////////////////////////////////////////////////////////////
        //                         Properties                       //
        //////////////////////////////////////////////////////////////
        public uint Id { get; private set; }

        public bool IsCreated { get { return Id != uint.MaxValue; } }


        //////////////////////////////////////////////////////////////
        //                           Methods                        //
        //////////////////////////////////////////////////////////////
        private void HandleTimeLineCreated(FxInHeader header, Packet packet)
        {
            if (header != FxInHeader.TimeLineCreated) return;
            if (packet.ReadGuid(true) != _guid) return;

            packet.ReadGuid();
            Id = packet.ReadUInt();
            HeConnectionManager.Instance.PacketReceived -= HandleTimeLineCreated;
            OnCreated();
        }

        public void SetEndTime(float endTime, bool sync = true)
        {
            if (Math.Abs(_endTime - endTime) <= float.Epsilon) return;

            _endTime = endTime;

            if (sync)
                SyncEndTime();       

            if (_currentTime > endTime)
                SetCurrentTime(endTime, sync);
        }
        public void SetCurrentTime(float time, bool sync = true)
        {
            if (Math.Abs(_currentTime - time) <= float.Epsilon) return;

            _currentTime = time;
            _currentTime = MathHelper.Clamp(_currentTime, 0.0f, _endTime);

            if (sync)
                SyncCurrentTime();
        }

        private void SyncEndTime()
        {
            if (Id == uint.MaxValue) return;

            Packet p = new Packet(FxOutHeader.SetTimeLineEndTime);
            p.Add(Id);
            p.Add(_endTime);
            HeConnectionManager.Instance.SendPacket(p);
        }
        private void SyncCurrentTime()
        {
            if (Id == uint.MaxValue) return;

            Packet p = new Packet(FxOutHeader.SetTimeLineCurrentTime);
            p.Add(Id);
            p.Add(_currentTime);
            HeConnectionManager.Instance.SendPacket(p);
        }

        public void FullSync()
        {
            SyncEndTime();
            SyncCurrentTime();
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
