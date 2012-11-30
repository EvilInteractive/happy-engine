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
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author: Sebastiaan Sprengers

#version 150 core

in vec2 passTexCoord;

out vec4 outColor;

uniform sampler2D diffuseMap;
uniform vec4 center;
uniform vec2 size;
uniform vec2 originalSize;

vec2 remap(in vec2 value, in vec4 originalRange, in vec4 newRange)
{
    return (value - originalRange.xy) / (originalRange.zw - originalRange.xy) * (newRange.zw - newRange.xy) + newRange.xy;
}

void main()
{
	// for debug
    vec4 color = vec4(0.0,0.0,0.0,0.5); 
	
    
    vec2 texOffset = center.xy / size;
    vec4 border = vec4(texOffset.xy, 1.0 - texOffset);
    vec4 mask = step(border, passTexCoord.xyxy);
    
    vec2 border1 = center.xy;
    vec2 border2 = size - border1 - center.zw;
    
    vec4 origBorder = vec4(center.xy, originalSize - border1 - border2);
    vec4 newBorder = vec4(center.xy, center.zw);
    
    vec4 origPartBorder = vec4(
            origBorder.xy * mask.xy + origBorder.zw * mask.zw,
            origBorder.xy + origBorder.zw * mask.xy + origBorder.xy * mask.zw);
    vec4 newPartBorder = vec4(
            newBorder.xy * mask.xy + newBorder.zw * mask.zw,
            newBorder.xy + newBorder.zw * mask.xy + newBorder.xy * mask.zw);
            
	origPartBorder /= originalSize.xyxy;
	newPartBorder /= size.xyxy;
    
    vec2 texcoord = remap(passTexCoord, newPartBorder, origPartBorder);
    color = texture2D(diffuseMap, texcoord);
    
    outColor = color;
}