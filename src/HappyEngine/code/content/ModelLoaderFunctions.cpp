//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Created: 31/08/2011
#include "HappyPCH.h" 

#include "ModelLoaderFunctions.h"
#include "HappyNew.h"
#include "vec2.h"
#include "MathFunctions.h"
#include "ModelMesh.h"

namespace he {
namespace ct {
namespace models {

void calculateTangents( const void* pVertices, size_t numVertices,
                        uint32 posOff, uint32 texOff, uint32 normOff, uint32 vertStride,
                        const void* pIndices, size_t numIndices, gfx::IndexStride indexStride,
                        he::PrimitiveList<vec3>& outTangents)
{
    he::PrimitiveList<vec3> tan1(numVertices);
    tan1.resize(numVertices);

    const char* pCharVertices(static_cast<const char*>(pVertices));

    const uint8* indicesByte(nullptr); 
    const uint16* indicesUShort(nullptr); 
    const uint32* indicesUInt(nullptr); 
    
    if (indexStride == gfx::IndexStride_Byte)
        indicesByte = static_cast<const uint8*>(pIndices);
    else if (indexStride == gfx::IndexStride_UShort)
        indicesUShort = static_cast<const uint16*>(pIndices);
    else if (indexStride == gfx::IndexStride_UInt)
        indicesUInt = static_cast<const uint32*>(pIndices);
    else
        HE_ASSERT(false, "unkown index stride: %d", indexStride);
    
    for (uint32 i = 0; i < numIndices; i += 3) //per triangle
    {
        uint32 i1(0), i2(0), i3(0);
        if (indexStride == gfx::IndexStride_Byte)
        {
            i1 = indicesByte[i];
            i2 = indicesByte[i + 1];
            i3 = indicesByte[i + 2];
        }
        else if (indexStride == gfx::IndexStride_UShort)
        {
            i1 = indicesUShort[i];
            i2 = indicesUShort[i + 1];
            i3 = indicesUShort[i + 2];
        }
        else if (indexStride == gfx::IndexStride_UInt)
        {
            i1 = indicesUInt[i];
            i2 = indicesUInt[i + 1];
            i3 = indicesUInt[i + 2];
        }

        const vec3& v1 = *reinterpret_cast<const vec3*>(pCharVertices + (i1 * vertStride + posOff));
        const vec3& v2 = *reinterpret_cast<const vec3*>(pCharVertices + (i2 * vertStride + posOff));
        const vec3& v3 = *reinterpret_cast<const vec3*>(pCharVertices + (i3 * vertStride + posOff));

        const vec2& tx1 = *reinterpret_cast<const vec2*>(pCharVertices + (i1 * vertStride + texOff));
        const vec2& tx2 = *reinterpret_cast<const vec2*>(pCharVertices + (i2 * vertStride + texOff));
        const vec2& tx3 = *reinterpret_cast<const vec2*>(pCharVertices + (i3 * vertStride + texOff));

        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;

        float s1 = tx2.x - tx1.x;
        float s2 = tx3.x - tx1.x;
        float t1 = tx2.y - tx1.y;
        float t2 = tx3.y - tx1.y;

        float r = 1.0f / (s1 * t2 - s2 * t1);

        vec3 sdir(
            (t2 * x1 - t1 * x2) * r, 
            (t2 * y1 - t1 * y2) * r, 
            (t2 * z1 - t1 * z2) * r );
        vec3 tdir(
            (s1 * x2 - s2 * x1) * r, 
            (s1 * y2 - s2 * y1) * r, 
            (s1 * z2 - s2 * z1) * r );

        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;
    }

    for (uint32 i = 0; i < numVertices; ++i)
    {
        const vec3& n = *reinterpret_cast<const vec3*>(pCharVertices + (i * vertStride + normOff));
        const vec3& t = tan1[i];

        outTangents.add(normalize(t - n * dot(n, t)));
    }
}

} } } //end namespace