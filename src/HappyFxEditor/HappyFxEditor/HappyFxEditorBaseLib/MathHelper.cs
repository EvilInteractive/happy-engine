using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HappyFxEditorBaseLib
{
    public static class MathHelper
    {
        public static float Clamp(float f, float min, float max)
        {
            return Math.Max(Math.Min(f, max), min);
        }
        public static int Clamp(int i, int min, int max)
        {
            return Math.Max(Math.Min(i, max), min);
        }

        public static decimal Clamp(decimal d, decimal min, decimal max)
        {
            return Math.Max(Math.Min(d, max), min);
        }
    }
}
