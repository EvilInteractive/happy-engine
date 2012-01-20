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
in vec4 inBoneId;
in vec4 inBoneWeight;

out vec2 passTexCoord;
out vec3 passNormal;
out vec3 passTangent;
out vec3 passPosition;

uniform mat4 matWVP;
uniform mat4 matWV;

uniform mat4[32] matBones;

void main()
{    
    ivec4 boneId = ivec4(inBoneId);
    
    vec4 position = matBones[boneId.x] * vec4(inPosition, 1.0f) * inBoneWeight.x;
    position += matBones[boneId.y] * vec4(inPosition, 1.0f) * inBoneWeight.y;
    position += matBones[boneId.z] * vec4(inPosition, 1.0f) * inBoneWeight.z;
    position += matBones[boneId.w] * vec4(inPosition, 1.0f) * inBoneWeight.w; 
    gl_Position = matWVP * vec4(position.xyz, 1.0f);
    
    passPosition = (matWV * vec4(position.xyz, 1.0f)).xyz;
    
    passTexCoord = inTexCoord;
    
    vec4 normal = matBones[boneId.x] * vec4(inNormal, 0.0f) * inBoneWeight.x;
    normal += matBones[boneId.y] * vec4(inNormal, 0.0f) * inBoneWeight.y;
    normal += matBones[boneId.z] * vec4(inNormal, 0.0f) * inBoneWeight.z;
    normal += matBones[boneId.w] * vec4(inNormal, 0.0f) * inBoneWeight.w; 
    passNormal = (matWV * normal).xyz;
    
    vec4 tangent = matBones[boneId.x] * vec4(inTangent, 0.0f) * inBoneWeight.x;
    tangent += matBones[boneId.y] * vec4(inTangent, 0.0f) * inBoneWeight.y;
    tangent += matBones[boneId.z] * vec4(inTangent, 0.0f) * inBoneWeight.z;
    tangent += matBones[boneId.w] * vec4(inTangent, 0.0f) * inBoneWeight.w; 
    passTangent = (matWV * tangent).xyz;
}



