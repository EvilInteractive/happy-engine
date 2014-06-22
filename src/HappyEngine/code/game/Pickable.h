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
//Author:  Bastian Damman
//Created: 2013/03/02

#ifndef _HE_Pickable_H_
#define _HE_Pickable_H_
#pragma once


namespace he {
class Ray;
class Bound;

namespace gfx {
ENUM(IndexStride, uint8);
}
namespace ge {

class PickResult;
class HAPPY_ENTRY Pickable
{
public:
    virtual ~Pickable() {}

    bool pick(const Ray& ray, PickResult& result);

protected:
    virtual void getPickingData(const vec3*& outVertices, const void*& outIndices, gfx::IndexStride& outIndexStride, size_t& outTriangleCount) const = 0;   // Local space
    virtual const Bound& getPickingBound() const = 0; // Local space
    virtual const mat44& getPickingWorld() const = 0;
};

} } //end namespace

#endif
