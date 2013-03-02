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
//Author:  Sebastiaan Sprengers
//Created: 11/11/2011

#include "HappyPCH.h" 
#include "Pickable.h"

#include "Bound.h"
#include "Ray.h"
#include "Plane.h"
#include "PickResult.h"

#include "ModelMesh.h"

namespace he {
namespace ge {

bool triangleHitTest(const Ray& ray, const vec3& v1, const vec3& v2, const vec3& v3, float& inOutDist, vec3& normal);

bool Pickable::pick( const Ray& ray, PickResult& result )
{
    mat44 pickingWorld(getPickingWorld().inverse());

    const Bound& bound(getPickingBound());

    Ray testRay(ray.transform(pickingWorld)); // transform ray instead of triangles, only 1 transform needed!

    float dist(result.isHit()? std::min(result.getHitDistance(), testRay.getMaxDistance()) : testRay.getMaxDistance());
    bool hit(false);
    vec3 normal(0, 0, 0);
    if (bound.intersectTest(testRay))
    {
        const vec3* vertices(nullptr);
        const void* indicesTemp(nullptr);
        gfx::IndexStride indexStride(gfx::IndexStride_Byte);
        size_t triangleCount(0);
        getPickingData(vertices, indicesTemp, indexStride, triangleCount);
        const char* indices(static_cast<const char*>(indicesTemp));
        
        const uint32 mask(0xffffffff >> ((sizeof(uint32) - indexStride) * 8));
        for (size_t triangle(0); triangle < triangleCount; ++triangle)
        {
            uint32 i1((*reinterpret_cast<const uint32*>(indices + ((triangle * 3 + 0) * indexStride))) & mask);
            uint32 i2((*reinterpret_cast<const uint32*>(indices + ((triangle * 3 + 1) * indexStride))) & mask);
            uint32 i3((*reinterpret_cast<const uint32*>(indices + ((triangle * 3 + 2) * indexStride))) & mask);
            if (triangleHitTest(testRay, vertices[i1], vertices[i2], vertices[i3], dist, normal))
            {
                hit = true;
            }
        }
    }

    if (hit == true)
    {
        result.setHit(this, testRay.getOrigin() + testRay.getDirection() * dist, normal, dist);
    }

    return hit;
}

bool triangleHitTest(const Ray& ray, const vec3& v1, const vec3& v2, const vec3& v3, float& inOutDist, vec3& normal)
{
    bool result(true);

    Plane p(v1, v2, v3);
    float dist(0.0f);
    if (p.intersect(ray, dist))
    {
        if (dist >= inOutDist) // early out if the hit dist is further than the prev one
        {
            result = false;
        }
        else
        {
            const vec3& rayOrigin(ray.getOrigin());
            const vec3 pointOnPlane(rayOrigin + ray.getDirection() * dist);

            const vec3 v_v1(v1 - pointOnPlane);
            const vec3 v_v2(v2 - pointOnPlane);
            const vec3 n1(normalize(cross(v2, v1)));
            const float d1(dot(-rayOrigin, n1));

            if (dot(pointOnPlane, n1) - d1 < 0)
            {
                result = false;
            }
            else
            {
                const vec3 v_v3(v3 - pointOnPlane);
                const vec3 n2(normalize(cross(v3, v2)));
                const float d2(dot(-rayOrigin, n2));

                if (dot(pointOnPlane, n2) - d2 < 0)
                {
                    result = false;
                }
                else
                {
                    const vec3 n3(normalize(cross(v1, v3)));
                    const float d3(dot(-rayOrigin, n3));

                    if (dot(pointOnPlane, n3) - d3 < 0)
                    {
                        result = false;
                    }
                }
            }
        }
    }
    if (result == true)
    {
        inOutDist = dist;
        normal = p.getNormal();
    }
    return result;
}

} } //end namespace