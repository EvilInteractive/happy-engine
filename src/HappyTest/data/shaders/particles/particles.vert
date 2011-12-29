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
//Author: Bastian Damman

#version 150 core

//Per vertex
in vec3 inPosition;

//Per Instance
in mat4 inWorld;
in vec4 inBlendColor;
in vec2 inUvTile;

// OUT
out vec2 passTexCoord;
out vec4 passBlendColor;
out vec2 passUvTile;

uniform mat4 matVP;

void main()
{  
	gl_Position = matVP * inWorld * vec4(inPosition, 1.0f);
	passTexCoord = inPosition.xy * 0.5 + 0.5;
    passBlendColor = inBlendColor;
}

