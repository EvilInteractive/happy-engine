#version 150 core

out vec4 outColor;

uniform vec4 color;
uniform bool blending;

void main()
{
    if (blending)
    {
        if (color.a < 0.01f)
        {
            discard;
        }
    }
    else
    {
        if (color.a < 0.5f)
        {
            discard;
        }
    }

    outColor = color;
}