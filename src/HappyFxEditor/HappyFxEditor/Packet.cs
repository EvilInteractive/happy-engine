using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media.Media3D;
using System.Windows;

namespace HappyFxEditor
{
    public class Packet
    {
        private byte[] _buffer;
        private int _offset = 0;

        public Packet(uint maxBufferSize, FxOutHeader header)
        {
            _buffer = new byte[maxBufferSize];
            Add((uint)header);
        }
        public Packet(byte[] buffer)
        {
            _buffer = buffer;
        }

        public void Add(byte[] bytes)
        {
            for (int i = 0; i < bytes.Length; ++i)
            {
                _buffer[_offset + i] = bytes[i];
            }
            _offset += bytes.Length;
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
            Add(System.Text.Encoding.ASCII.GetBytes(str));
        }

        public FxInHeader ReadHeader()
        {
            _offset += sizeof(uint);
            return (FxInHeader)BitConverter.ToUInt32(_buffer, _offset - sizeof(uint));
        }
        public float ReadFloat()
        {
            _offset += sizeof(float);
            return BitConverter.ToSingle(_buffer, _offset - sizeof(float));
        }
        public int ReadInt()
        {
            _offset += sizeof(int);
            return BitConverter.ToInt32(_buffer, _offset - sizeof(int));
        }
        public uint ReadUInt()
        {
            _offset += sizeof(uint);
            return BitConverter.ToUInt32(_buffer, _offset - sizeof(uint));
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
            string str = BitConverter.ToString(_buffer, _offset, (int)len);
            _offset += (int)len;
            return str;
        }

        public byte[] GetBuffer()
        {
            return _buffer;
        }
        public int GetSize()
        {
            return (int)_offset;
        }
    }
}
