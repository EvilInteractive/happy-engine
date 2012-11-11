//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/8licenses/>.
//
//Author:  Bastian Damman
//Created: 14/10/2011
//Name change: HDRtoLDR -> postEffects : 18/12/2011

#version 150 core
#pragma optionNV(fastmath on)
//#pragma optionNV(fastprecision on)
#pragma optionNV(ifcvt none)
#pragma optionNV(inline all)
#pragma optionNV(strict on)
#pragma optionNV(unroll all)

#include "packing/decode.frag"
#include "shared/tonemap.frag"

noperspective in vec2 texCoord;

out vec4 outColor;

uniform sampler2D colorMap;

#if BLOOM
uniform sampler2D blur0;
uniform sampler2D blur1;
uniform sampler2D blur2;
uniform sampler2D blur3;
#endif

///////// SSAO & GI //////////
#if AO
uniform sampler2D noiseMap;

uniform float radius;
uniform float intensity;
uniform float scale;
uniform float bias;

uniform vec2 viewPortSize;
uniform vec4 projParams;

#endif
/////////////////////////

uniform sampler2D lumMap;

uniform sampler2D normalDepthMap;

uniform vec3 fogColor;

float edgeMult1 = 5.0f;
float edgeMult2 = 10.0f;

float vignette(vec2 pos, float inner, float outer)
{
    float r = length(pos);
    r = 1.0 - smoothstep(inner, outer, r);
    return r;
}

float getEdge(in sampler2D map, in vec2 texCoord)
{
    vec3 sumX = vec3(0, 0, 0);
    vec3 sumY = vec3(0, 0, 0); 
    
    vec3 c00 = textureOffset(map, texCoord, ivec2(-1, -1)).rgb;
    vec3 c01 = textureOffset(map, texCoord, ivec2(0, -1)).rgb;
    vec3 c02 = textureOffset(map, texCoord, ivec2(1, -1)).rgb;

    vec3 c10 = textureOffset(map, texCoord, ivec2(-1, 0)).rgb;
    vec3 c12 = textureOffset(map, texCoord, ivec2(1, 0)).rgb;

    vec3 c20 = textureOffset(map, texCoord, ivec2(-1, 1)).rgb;
    vec3 c21 = textureOffset(map, texCoord, ivec2(0, 1)).rgb;
    vec3 c22 = textureOffset(map, texCoord, ivec2(1, 1)).rgb;

    sumX -= c00 * edgeMult1;
    sumX -= c01 * edgeMult2;
    sumX -= c02 * edgeMult1;
    sumX += c20 * edgeMult1;
    sumX += c21 * edgeMult2;
    sumX += c22 * edgeMult1;

    sumY -= c00 * edgeMult1;
    sumY += c02 * edgeMult1;
    sumY -= c10 * edgeMult2;
    sumY += c12 * edgeMult2;
    sumY -= c20 * edgeMult1;
    sumY += c22 * edgeMult1;

    vec3 endCol = sumX * sumX + sumY * sumY;
    return 1-clamp((endCol.r + endCol.g + endCol.b) / 3.0f, 0.0f, 1.0f); //black-ify
}

/////////////////////////////* SSAO *///////////////////////////////
#if AO

float readDepth(in vec2 coord)  
{  
    //if (coord.x < 0 || coord.y < 0) return 1.0f;
    //const float nearZ = 1.0f;
    //const float farZ = 250.0f;  
   /* float posZ =*/ return textureLod(normalDepthMap, coord, 0).z;
    //return (2.0f * nearZ) / (nearZ + farZ - posZ * (farZ - nearZ));  
}   

vec3 readColor(in vec2 coord)  
{  
    return texture2D(colorMap, coord).xyz;  
} 

float compareDepths(in float depth1, in float depth2)  
{  
    float gauss = 0.0f; 
    float diff = (depth1 - depth2) * 100.0f; //depth difference (0-100)
    float gdisplace = 0.2f; //gauss bell center
    float garea = 3.0f; //gauss bell width

    //reduce left bell width to avoid self-shadowing
    if (diff < gdisplace) garea = 0.2f; 

    gauss = pow(2.7182f, -2.0f * (diff - gdisplace) * (diff - gdisplace) / (garea * garea));

    return max(0.2f, gauss);  
}  

vec3 calcAO(in vec2 texCoord, in float depth, in float dw, in float dh, inout float ao)  
{  
    float temp = 0;
    vec3 bleed = vec3(0.0f, 0.0f, 0.0f);
    float coordw = texCoord.x + dw / depth;
    float coordh = texCoord.y + dh / depth;

    //if (coordw < 1.0f && coordw > 0.0f && coordh < 1.0f && coordh > 0.0f)
    //{
        vec2 coord = vec2(coordw, coordh);
        temp = compareDepths(depth, readDepth(coord)); 
        bleed = readColor(coord); 
    //}
    ao += temp;
    return temp * bleed;  
}   
     
void getAoAndGi(in vec2 texCoord, out float out_ao, out vec3 out_gi)  
{  
    float pw = 1.0f / viewPortSize.x * 0.5f;
    float ph = 1.0f / viewPortSize.y * 0.5f;

    //randomization texture:
    vec2 noiseTextureSize = vec2(64, 64);
    vec3 random = textureLod(noiseMap, texCoord * viewPortSize.xy / noiseTextureSize, 0).xyz * 2.0f - 1.0f;

    //initialize stuff:
    float depth = readDepth(texCoord);
    vec3 gi = vec3(0.0f, 0.0f, 0.0f);  
    float ao = 0.0f;

    for(int i = 0; i < 8; ++i) 
    {  
        //calculate color bleeding and ao:
        gi += calcAO(texCoord, depth,  pw,  ph, ao);  
        gi += calcAO(texCoord, depth,  pw, -ph, ao);  
        gi += calcAO(texCoord, depth, -pw,  ph, ao);  
        gi += calcAO(texCoord, depth, -pw, -ph, ao); 
     
        //sample jittering:
        pw += random.x * 0.0005f;
        ph += random.y * 0.0005f;

        //increase sampling area:
        pw *= 1.4;  
        ph *= 1.4;    
    }         

    //final values, some adjusting:
    out_ao = 1.0f - (ao / 32.0f);
    out_gi = (gi / 32.0f) * 0.6f;    
}

#endif
///////////////////////////////////////////////////////////////////

void main()
{
    vec4 sampleColor = textureLod(colorMap, texCoord, 0.0f);
        
    vec3 color = sampleColor.rgb;
    
#if BLOOM
    color += texture(blur0, texCoord).rgb * 0.25f;  
    color += texture(blur1, texCoord).rgb * 0.25f;  
    color += texture(blur2, texCoord).rgb * 0.25f;  
    color += texture(blur3, texCoord).rgb * 0.25f;  
#endif
	bool post = sampleColor.a > 0.01f;

#if AO
    float ao = 1.0f;
	if (post)
	{
		vec3 gi; 
		getAoAndGi(texCoord, ao, gi);
		color = color + gi * 5;
	}
#endif

#if HDR  
	float lum = getWhite(lumMap, 1.0f, 10.0f);
    color = tonemap(color, vec3(1.0f, 1.0f, 1.0f) * lum);
#endif
 
#if VIGNETTE   
    color *= vignette(texCoord * 2.0f - 1.0f, 0.9f, 2.0f);
#endif

	if (post)
	{
#if FOG
    const float beginFog = 0.5f;
	vec3 normalDepth = texture(normalDepthMap, texCoord).xyz;
    float fog = max(0, normalDepth.z - beginFog) * (1.0f / (1.0f - beginFog));

    color = color * (1 - fog) + fogColor * (fog);
#endif

#if AO
    color *= ao;
#endif
    }
    //color = color - gi;
    //color = vec3(ao);
			
    outColor = vec4(color, 1.0f);
}
