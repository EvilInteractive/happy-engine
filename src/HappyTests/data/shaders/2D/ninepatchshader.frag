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

void main()
{
	// for debug
    vec4 color = vec4(0.0,0.0,0.0,0.5); 
	
	vec2 scale;
	scale.x = size.x / originalSize.x;
	scale.y = size.y / originalSize.y;
	
	vec2 pos;
	pos.x = passTexCoord.x * size.x;
	pos.y = passTexCoord.y * size.y;
	
	vec2 origPos;
	origPos.x = passTexCoord.x * scale.x;
	origPos.y = passTexCoord.y * scale.y;
	
	// bottom left
	if (pos.x < center.r && pos.y > center.g + center.a)
	{
		color = texture2D(diffuseMap, vec2(origPos.x, origPos.y - ((size.y - originalSize.y) / originalSize.y)));
	}
	// top left
	else if (pos.x < center.r && pos.y < center.g)
	{
		color = texture2D(diffuseMap, origPos);
	}
	// bottom right
	else if (pos.x > center.r + center.b && pos.y > center.g + center.a)
	{
		color = texture2D(diffuseMap, vec2(origPos.x - ((size.x - originalSize.x) / originalSize.x), origPos.y - ((size.y - originalSize.y) / originalSize.y)));
	}
	// top right
	else if (pos.x > center.r + center.b && pos.y < center.g)
	{
		color = texture2D(diffuseMap, vec2(origPos.x - ((size.x - originalSize.x) / originalSize.x), origPos.y));
	}
	// top
	else if (pos.x > center.r && pos.x < center.r + center.b && pos.y < center.g)
	{
		float sx = (center.b / size.x);
		float ox = (center.r / size.x);

		color = texture2D(diffuseMap, vec2((passTexCoord.x * sx) + ox, origPos.y));
	}
	
    outColor = color;
}