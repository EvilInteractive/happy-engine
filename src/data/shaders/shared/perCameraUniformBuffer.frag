//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#ifndef _FRAG_CAMERAUNIFORMBUFFER
#define _FRAG_CAMERAUNIFORMBUFFER

struct PerCameraUniformBuffer
{
    mat4 viewMatrix;
    mat4 projMatrix;
    mat4 viewProjMatrix;
    
    vec3 cameraPosition;
    
    vec2 cameraNearFar;
    vec2 cameraViewSize;
    
    vec4 projParams;
};

layout(std140) uniform SharedPerCameraUniformBuffer
{
	PerCameraUniformBuffer perCameraUniformBuffer;
};

#endif // _FRAG_CAMERAUNIFORMBUFFER
