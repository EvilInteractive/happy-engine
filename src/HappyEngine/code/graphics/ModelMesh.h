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
//Created: 15/09/2011 (copy of the previous Model class)

#ifndef _HE_MODELMESH_H_
#define _HE_MODELMESH_H_
#pragma once

#include "VertexLayout.h"
#include "MeshEnums.h"
#include "Bone.h"

#include "Resource.h"
#include "Bound.h"

namespace he {
namespace gfx {

struct PickingData
{
    PickingData() : m_Vertices(nullptr), m_Indices(nullptr), m_IndexStride(IndexStride_Byte), m_TriangleCount(0) {}
    vec3* m_Vertices;
    void* m_Indices;
    IndexStride m_IndexStride;
    size_t m_TriangleCount;
};

class ModelMesh : public Resource<ModelMesh>
{
public:    
    ModelMesh();
    virtual ~ModelMesh();

    void init(const VertexLayout& vertexLayout, MeshDrawMode mode);
    void setVertices(const void* const vertices, const uint32 num, const MeshUsage usage, const bool calcBound);
    void setIndices(const void* const indices, const uint32 num, const IndexStride type, const MeshUsage usage);
    void setBones(const he::ObjectList<Bone>& boneList);

    // Getters
    inline const he::ObjectList<Bone>& getBones() const { return m_BoneList; }

    inline uint32 getVBO() const { return m_VertexVboID; }
    inline uint32 getIBO() const { return m_IndexVboID; }
    inline const MeshDrawMode& getDrawMode() const { return m_DrawMode; }

    void createPickingData(const void* const vertices, const size_t vertexCount, const VertexLayout& vertexLayout, const void* const indices, const size_t indexCount, const IndexStride indexStride);
    void destroyPickingData();
    const PickingData& getPickingData() const { return m_PickingData; }
    bool hasPickingData() const { return m_PickingData.m_Vertices != nullptr; }

    inline uint32 getNumVertices() const { return m_NumVertices; }
    inline uint32 getNumIndices() const { return m_NumIndices; }

    inline uint32 getIndexType() const { return m_IndexType; }
    inline const VertexLayout& getVertexLayout() const { return m_VertexLayout; }

    inline const Bound& getBound() const { return m_Bound; }

    void draw() const;
    
private:
    struct ShadowSkinnedVertex
    {
        vec3 pos;
        vec4 boneId;
        vec4 boneWeight;
    };

    uint32 m_VertexVboID;
    uint32 m_IndexVboID;

    uint32 m_NumVertices;
    uint32 m_NumIndices;

    VertexLayout m_VertexLayout;
    uint32 m_IndexType;

    Bound m_Bound;
    PickingData m_PickingData;

    he::ObjectList<Bone> m_BoneList;

    MeshDrawMode m_DrawMode;
    
    //Disable default copy constructor and default assignment operator
    ModelMesh(const ModelMesh&);
    ModelMesh& operator=(const ModelMesh&);
};

typedef ResourceFactory<ModelMesh> ModelMeshFactory;

} } //end namespace

#endif
