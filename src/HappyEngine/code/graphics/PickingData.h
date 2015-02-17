//HappyEngine Copyright (C) 2011 - 2015  Evil Interactive
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
//Created: 2015/02/17

#ifndef PickingData_h__
#define PickingData_h__

#include "MeshEnums.h"

namespace he {
namespace gfx {
class VertexLayout;
struct HAPPY_ENTRY PickingData
{
public:
    PickingData();
    ~PickingData();

    void destroyPickingData();
    void createPickingData(const void* const vertices, const size_t vertexCount, const VertexLayout& vertexLayout, const void* const indices, const size_t indexCount, const IndexStride indexStride);

    vec3* m_Vertices;
    void* m_Indices;
    IndexStride m_IndexStride;
    size_t m_TriangleCount;

private:
    PickingData(const PickingData&);
    PickingData& operator=(const PickingData&);
};


} }

#endif // PickingData_h__
