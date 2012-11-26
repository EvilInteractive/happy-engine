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

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;
in vec3 inTangent;

out vec2 passTexCoord;
out vec3 passNormal;
out vec3 passTangent;
out float passDepth;

uniform mat4 matWVP;
uniform mat4 matWorldView;

void main()
{
    passTexCoord = inTexCoord;
    passNormal = (matWorldView * vec4(inNormal, 0.0f)).xyz;
    passTangent = (matWorldView * vec4(inTangent, 0.0f)).xyz;
    passDepth = (matWorldView * vec4(inPosition, 1.0f)).z;

    gl_Position = matWVP * vec4(inPosition, 1.0f);
}