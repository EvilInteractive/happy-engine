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

in vec2 inPosition;
in vec2 inTexCoord;
in mat4 matWVP;
//in vec2 tcOffset;
//in vec2 tcScale;

out vec2 passTexCoord;
out vec2 passTcOffset;
out vec2 passTcScale;

uniform float depth;

void main()
{
	gl_Position = matWVP * vec4(inPosition, 0.5, 1.0);
	passTexCoord = inTexCoord;
	//passTcOffset = tcOffset;
	//passTcScale = tcScale;
}