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
//Author:  Bastian Damman
//Created: 11/08/2011

#version 150 core

in vec3 inPosition;

out vec4 passPos;
out vec4 passWVPos;

uniform mat4 mtxWVP;
uniform mat4 mtxWV;

void main()
{
	//vec4 pos = ;
    gl_Position = mtxWVP * vec4(inPosition, 1.0f);
	passPos = gl_Position;
    passWVPos = mtxWV * vec4(inPosition, 1.0f);
}