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
    virtual ~BinObjLoader();

    virtual bool load(const std::string& path, const gfx::BufferLayout& vertLayout, bool allowByteIndices = true);

    virtual uint32 getNumMeshes() const;
    virtual const std::string& getMeshName(uint32 mesh) const;
    
    virtual const he::ObjectList<gfx::Bone>& getBones(uint32 mesh) const;

    virtual const void* getVertices(uint32 mesh) const;
    virtual uint32 getNumVertices(uint32 mesh) const;

    virtual const void* getIndices(uint32 mesh) const;
    virtual gfx::IndexStride getIndexStride(uint32 mesh) const;
    virtual uint32 getNumIndices(uint32 mesh) const;


private:
    bool read(const std::string& path, bool allowByteIndices);
    void fill(const gfx::BufferLayout& vertLayout) const;

    he::PrimitiveList<he::ObjectList<InternalVertex>*> m_VertexData;
    he::PrimitiveList<he::ObjectList<gfx::Bone>*> m_BoneData;
    
    he::ObjectList<std::string> m_MeshName;

    he::PrimitiveList<void*> m_Vertices;
    he::PrimitiveList<void*> m_Indices;

    he::PrimitiveList<uint32> m_NumIndices;
    he::PrimitiveList<gfx::IndexStride> m_IndexStride;

    //Disable default copy constructor and default assignment operator
    BinObjLoader(const BinObjLoader&);
    BinObjLoader& operator=(const BinObjLoader&);
};

} } } //end namespace

#endif
