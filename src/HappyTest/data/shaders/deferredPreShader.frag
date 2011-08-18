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
#version 150 core

in vec2 passTexCoord;
in vec3 passNormal;
in vec3 passWorldPos;

out vec4 outColorIll;
out vec4 outNormalGloss;
out vec4 outPosSpec;

uniform sampler2D diffuseMap;

void main()
{
	outColorIll = vec4(texture2D(diffuseMap, passTexCoord).rgb, 0.0f);
	outPosSpec = vec4(passWorldPos, 1.0f);
	outNormalGloss = vec4(passNormal, 0.5f);
}