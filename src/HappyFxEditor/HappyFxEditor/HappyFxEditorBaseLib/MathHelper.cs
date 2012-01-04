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
    }
}
