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
//Author: Bastian Damman
//Created: 28/08/2011

#ifndef _HE_BINOBJ_LOADER_H_
#define _HE_BINOBJ_LOADER_H_
#pragma once

#include "Bone.h"
#include "IModelLoader.h"
#include "BufferLayout.h"

namespace he {
namespace ct {
namespace models {

class BinObjLoader : public IModelLoader
{
public:
    struct InternalVertex
    {
        vec3 pos;
        vec2 tex;
        vec3 norm;
        vec3 tan;
        uint8 boneID[gfx::Bone::MAX_BONEWEIGHTS];
        float boneWeight[gfx::Bone::MAX_BONEWEIGHTS];

        InternalVertex(): pos(), tex(), norm(), tan()
        {
            for (int i = 0; i < gfx::Bone::MAX_BONEWEIGHTS; ++i)
            {
                boneID[i] = 0;
                boneWeight[i] = 0.0f;
            }
        } 
    };
    struct InternalVertexNoBones
    {
        vec3 pos;
        vec2 tex;
        vec3 norm;
        vec3 tan;

        InternalVertexNoBones(): pos(), tex(), norm(), tan()
        {
        } 
    };

    BinObjLoader();
    ~BinObjLoader();

    bool load(const he::String& path, bool allowByteIndices = true);

    size_t getNumMeshes() const;
    const he::String& getMeshName(uint32 mesh) const;
    
    const he::ObjectList<gfx::Bone>& getBones(uint32 mesh) const;

    const void* getVertices(uint32 mesh) const;
    size_t getNumVertices(uint32 mesh) const;

    const void* getIndices(uint32 mesh) const;
    gfx::IndexStride getIndexStride(uint32 mesh) const;
    size_t getNumIndices(uint32 mesh) const;

    const gfx::BufferLayout& getVertexLayout() const { return m_VertexLayout; }


private:
    bool read(const he::String& path, bool allowByteIndices);
    void fill();
    
    he::PrimitiveList<he::ObjectList<InternalVertex>*> m_VertexData;
    he::PrimitiveList<he::ObjectList<gfx::Bone>*> m_BoneData;
    
    he::ObjectList<he::String> m_MeshName;

    he::PrimitiveList<void*> m_Vertices;
    he::PrimitiveList<void*> m_Indices;

    he::PrimitiveList<uint32> m_NumIndices;
    he::PrimitiveList<gfx::IndexStride> m_IndexStride;

    gfx::BufferLayout m_VertexLayout;

    //Disable default copy constructor and default assignment operator
    BinObjLoader(const BinObjLoader&);
    BinObjLoader& operator=(const BinObjLoader&);
};

} } } //end namespace

#endif
