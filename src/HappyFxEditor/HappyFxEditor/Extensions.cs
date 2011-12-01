using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Diagnostics;

namespace HappyFxEditor
{
    public static class ColorExtensions
    {
        ///http://130.113.54.154/~monger/hsl-rgb.html
        public static Color FromHsb(short hueDegrees, double saturation, double brightness)
        {
            Debug.Assert(hueDegrees >= 0 && hueDegrees <= 360);
            Debug.Assert(saturation >= 0.0 && saturation <= 1.0);
            Debug.Assert(brightness >= 0.0);


            double[] rgb = new double[] { 0, 0, 0 };

            if (brightness == 0 || saturation == 0)
            {
                rgb[0] = brightness;
                rgb[1] = brightness;
                rgb[2] = brightness;
            }
            else
            {
                double temp1 = 0.0, temp2 = 0.0;
                if (brightness < 0.5)
                    temp2 = brightness * (1.0 + saturation);
                else
                    temp2 = brightness + saturation - (brightness * saturation);
                temp1 = 2.0 * brightness - temp2;

                double hue = hueDegrees / 360.0;
                double[] temp3 = new double[] { hue + 1.0 / 3.0, hue, hue - 1.0 / 3.0 };
                for (int i = 0; i < 3; i++) //R - G - B
                {
                    if (temp3[i] < 0)
                        temp3[i] += 1.0;
                    if (temp3[i] > 1)
                        temp3[i] -= 1.0;

                    if (6.0 * temp3[i] < 1.0)
                        rgb[i] = temp1 + (temp2 - temp1) * temp3[i] * 6.0;
                    else if (2.0 * temp3[i] < 1.0)
                        rgb[i] = temp2;
                    else if (3.0 * temp3[i] < 2.0)
                        rgb[i] = (temp1 + (temp2 - temp1) * ((2.0 / 3.0) - temp3[i]) * 6.0);
                    else
                        rgb[i] = temp1;
                }
            }

            return Color.FromRgb((byte)(255 * rgb[0]), (byte)(255 * rgb[1]), (byte)(255 * rgb[2]));
        }
    }
}
