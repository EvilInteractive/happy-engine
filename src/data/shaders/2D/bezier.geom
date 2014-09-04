#version 150 core

layout(points) in;
layout(triangle_strip, max_vertices = 42) out; // (sub + 1) * 2

uniform mat4 matWVP;

uniform vec4 beginEndPos;
uniform vec4 tangents;
uniform float radius;

vec2 curve(in vec2 p0, in vec2 p1, in vec2 p2, in vec2 p3, in float t)
{
    float oneMinT = 1.0 - t;
    return oneMinT*oneMinT*oneMinT*p0 +
           3.0*oneMinT*oneMinT*t*p1 +
           3.0*oneMinT*t*t*p2 +
           t*t*t*p3;
}

vec2 curveTan(in vec2 p0, in vec2 p1, in vec2 p2, in vec2 p3, in float t)
{
    float oneMinT = 1.0 - t;
    return 3.0*oneMinT*oneMinT*(p1-p0) + 
           6.0*oneMinT*t*(p2-p1) + 
           3.0*t*t*(p3-p2);
}
void main() 
{
    vec4 base = gl_in[0].gl_Position;
    for (int i = 0; i <= 20; ++i) 
    {
        float t = i / 20.0;
        vec2 point = curve(beginEndPos.xy, tangents.xy, tangents.zw, beginEndPos.zw, t);
        vec2 tangent = curveTan(beginEndPos.xy, tangents.xy, tangents.zw, beginEndPos.zw, t);
        vec2 normal = normalize(vec2(-tangent.y, tangent.x));
        
        gl_Position = matWVP * (base + vec4(point + normal * radius, 0, 0));
        EmitVertex();
        gl_Position = matWVP * (base + vec4(point - normal * radius, 0, 0));
        EmitVertex();
    }
    
    EndPrimitive();
}