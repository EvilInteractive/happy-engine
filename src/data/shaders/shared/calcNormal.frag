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

#ifndef _FRAG_CALCNORMAL
#define _FRAG_CALCNORMAL

vec3 calcNormal(in vec3 normal, in vec3 tangent, in vec3 rgb)
{
    //NormalMap
    tangent = normalize(tangent);
    normal = normalize(normal);

    vec3 binormal = cross(tangent, normal);
    mat3 axis = mat3(binormal, tangent, normal);
    vec3 xyz = vec3(rgb.x * 2 - 1, (1-rgb.y) * 2 - 1, rgb.z * 2 - 1);

    return normalize(axis * xyz);
}

#endif // _FRAG_CALCNORMAL
