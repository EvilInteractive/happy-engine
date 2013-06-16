#version 150 core

noperspective in vec2 texCoord;
out vec4 outColor;

uniform sampler2D preDistortMap;

uniform vec4 hmdWarpParam;
uniform vec2 lensCenter;
uniform vec2 screenCenter;
uniform vec2 scale;
uniform vec2 scaleIn;
uniform vec4 tcTransform;


const vec2 screenHalfLength = vec2(0.25f, 0.5f);

vec2 hmdWarp(in vec2 in01)
{
    vec2 theta = (in01 - lensCenter) * scaleIn; // Scales to [-1, 1]
    float rSq = dot(theta, theta);
    vec2 rvector = theta * (hmdWarpParam.x + hmdWarpParam.y * rSq +
                            hmdWarpParam.z * rSq * rSq +
                            hmdWarpParam.w * rSq * rSq * rSq);
    return lensCenter + scale * rvector;
}

void main()
{
    vec2 tc = hmdWarp(tcTransform.xy + texCoord * tcTransform.zw);
    if (any(bvec2(clamp(tc, screenCenter - screenHalfLength, screenCenter + screenHalfLength) - tc)))
        outColor = vec4(0);
    else
        outColor = texture(preDistortMap, tc);
};
