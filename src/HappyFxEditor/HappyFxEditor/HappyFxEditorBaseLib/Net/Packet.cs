using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media.Media3D;
using System.Windows;

namespace HappyFxEditorBaseLib.Net
{
    public class Packet
    {
        private readonly List<byte> _buffer;
        private int _offset = 0;

        public Packet(FxOutHeader header) // open for write
        {
            _buffer = new List<byte>();
            Add((uint)header);
        }
        public Packet(IEnumerable<byte> buffer) // open for read
        {
            _buffer = new List<byte>(buffer);
        }

        public void Add(byte[] bytes)
        {
            _buffer.AddRange(bytes);
        }
        public void Add(Guid val)
        {
            Add(val.ToByteArray());
        }
        public void Add(float val)
        {
            Add(BitConverter.GetBytes(val));
        }
        public void Add(Int32 val)
        {
            Add(BitConverter.GetBytes(val));
        }
        public void Add(UInt32 val)
        {
            Add(BitConverter.GetBytes(val));
        }
        public void Add(Vector3D val)
        {
            Add((float)val.X);
            Add((float)val.Y);
            Add((float)val.Z);
        }
        public void Add(Vector val)
        {
            Add((float)val.X);
            Add((float)val.Y);
        }
        public void Add(string str)
        {
            Add((uint)str.Length);
            Add(Encoding.ASCII.GetBytes(str));
        }

        public FxInHeader ReadHeader()
        {
            int off = _offset;
            _offset += sizeof(UInt32);
            return (FxInHeader)BitConverter.ToUInt32(_buffer.ToArray(), off);
        }
        public Guid ReadGuid(bool peek = false)
        {
            int off = _offset;
            if (peek == false)
                _offset += 16;
            return new Guid(_buffer.GetRange(off, 16).ToArray());
        }
        public float ReadFloat(bool peek = false)
        {
            int off = _offset;
            if (peek == false)
                _offset += sizeof(float);
            return BitConverter.ToSingle(_buffer.ToArray(), off);
        }
        public int ReadInt(bool peek = false)
        {
            int off = _offset;
            if (peek == false)
                _offset += sizeof(Int32);
            return BitConverter.ToInt32(_buffer.ToArray(), off);
        }
        public uint ReadUInt(bool peek = false)
        {
            int off = _offset;
            if (peek == false)
                _offset += sizeof(UInt32);
            return BitConverter.ToUInt32(_buffer.ToArray(), off);
        }
        public Vector3D ReadVector3D()
        {
            Vector3D vec = new Vector3D();
            vec.X = ReadFloat();
            vec.Y = ReadFloat();
            vec.Z = ReadFloat();
            return vec;
        }
        public Vector ReadVector()
        {
            Vector vec = new Vector();
            vec.X = ReadFloat();
            vec.Y = ReadFloat();
            return vec;
        }
        public string ReadString()
        {
            uint len = ReadUInt();
            string str = Encoding.ASCII.GetString(_buffer.ToArray(), _offset, (int)len);
            _offset += (int)len;
            return str;
        }

        public byte[] GetBuffer()
        {
            return _buffer.ToArray();
        }
        public int GetSize()
        {
            return _buffer.Count;
        }
    }
}
