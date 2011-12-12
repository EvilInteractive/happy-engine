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
//Author:  Bastian Damman
//Created: 15/09/2011 (copy of the previous Model class)

#ifndef _HE_MODELMESH_H_
#define _HE_MODELMESH_H_
#pragma once

#include "HeAssert.h"
#undef assert
#define assert ASSERT

#include <vector>
#include <string>
#include "VertexLayout.h"
#include "boost/shared_ptr.hpp"
#include "HappyTypes.h"
#include "Sphere.h"
#include "Bone.h"

#include "boost/function.hpp"
#include "boost/thread/mutex.hpp"

#include "event.h"

namespace he {
namespace gfx {

enum IndexStride
{
    IndexStride_Byte = sizeof(byte),
    IndexStride_UShort = sizeof(ushort),
    IndexStride_UInt = sizeof(uint)
};

class ModelMesh
{
public:
    typedef boost::shared_ptr<ModelMesh> pointer;

    explicit ModelMesh(const std::string& name);
    virtual ~ModelMesh();

    void init();
    void setVertices(const void* pVertices, uint num, const VertexLayout& vertexLayout);
    void setIndices(const void* pIndices, uint num, IndexStride type);

    void setBones(const std::vector<Bone>& boneList);
    const std::vector<Bone>& getBones() const;

    uint getVertexArraysID() const;
    uint getVertexShadowArraysID() const;
    uint getVBOID() const;
    uint getVBOShadowID() const;
    uint getVBOIndexID() const;

    uint getNumVertices() const;
    uint getNumIndices() const;

    uint getIndexType() const;
    const VertexLayout& getVertexLayout() const;

    const std::string& getName() const;

    bool isLoaded() const;

    const shapes::Sphere& getBoundingSphere() const;

    void callbackIfLoaded(const boost::function<void()>& callback);


private:
    struct ShadowSkinnedVertex
    {
        vec3 pos;
        vec4 boneId;
        vec4 boneWeight;
    };


    event<void> Loaded;
    void setLoaded();
    boost::mutex m_LoadMutex;

    uint m_VaoID[1];
    uint m_VertexVboID[1];
    uint m_VaoShadowID[1];
    uint m_VertexVboShadowID[1];
    uint m_IndexVboID[1];

    uint m_NumVertices;
    uint m_NumIndices;


    VertexLayout m_VertexLayout;
    uint m_IndexType;

    std::string m_Name;

    bool m_isVisible;
    bool m_isLoaded;

    shapes::Sphere m_BoundingSphere;

    std::vector<Bone> m_BoneList;

    //Disable default copy constructor and default assignment operator
    ModelMesh(const ModelMesh&);
    ModelMesh& operator=(const ModelMesh&);
};

} } //end namespace

#endif
