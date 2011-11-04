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
in byte[4] inBoneId;
in float[4] inBoneWeight;

out vec2 passTexCoord;
out vec3 passNormal;
out vec3 passTangent;

uniform mat4 matWVP;
uniform mat4 matWorld;

uniform mat4[32] matBones;

void main()
{
    mat4 world = matWorld;
    
    vec4 position = matBones[inBoneId[0]] * vec4(inPosition, 1.0f) * inBoneWeight[0];
    position += matBones[inBoneId[1]] * vec4(inPosition, 1.0f) * inBoneWeight[1];
    position += matBones[inBoneId[2]] * vec4(inPosition, 1.0f) * inBoneWeight[2];
    position += matBones[inBoneId[3]] * vec4(inPosition, 1.0f) * inBoneWeight[3]; 
	gl_Position = matWVP * vec4(position, 1.0f);
	
    passTexCoord = inTexCoord;
    
    vec4 normal = matBones[inBoneId[0]] * vec4(inNormal, 0.0f) * inBoneWeight[0];
    normal += matBones[inBoneId[1]] * vec4(inNormal, 0.0f) * inBoneWeight[1];
    normal += matBones[inBoneId[2]] * vec4(inNormal, 0.0f) * inBoneWeight[2];
    normal += matBones[inBoneId[3]] * vec4(inNormal, 0.0f) * inBoneWeight[3]; 
	passNormal = (matWorld * normal).xyz;
    
    vec4 tangent = matBones[inBoneId[0]] * vec4(inTangent, 0.0f) * inBoneWeight[0];
    tangent += matBones[inBoneId[1]] * vec4(inTangent, 0.0f) * inBoneWeight[1];
    tangent += matBones[inBoneId[2]] * vec4(inTangent, 0.0f) * inBoneWeight[2];
    tangent += matBones[inBoneId[3]] * vec4(inTangent, 0.0f) * inBoneWeight[3]; 
	passTangent = (matWorld * tangent).xyz;
}



