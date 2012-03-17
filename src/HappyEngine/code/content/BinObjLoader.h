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

#include "IModelLoader.h"

namespace he {
namespace ct {
namespace models {

class BinObjLoader : public IModelLoader
{
public:
    struct InternalVertex;
    struct InternalVertexNoBones;

    BinObjLoader();
    virtual ~BinObjLoader();

    virtual bool load(const std::string& path, const gfx::BufferLayout& vertLayout, bool allowByteIndices = true);

    virtual uint getNumMeshes() const;
    virtual const std::string& getMeshName(uint mesh) const;
    
    virtual const std::vector<gfx::Bone>& getBones(uint mesh) const;

    virtual const void* getVertices(uint mesh) const;
    virtual uint getNumVertices(uint mesh) const;

    virtual const void* getIndices(uint mesh) const;
    virtual gfx::IndexStride getIndexStride(uint mesh) const;
    virtual uint getNumIndices(uint mesh) const;


private:
    bool read(const std::string& path, bool allowByteIndices);
    void fill(const gfx::BufferLayout& vertLayout) const;

    std::vector<std::vector<InternalVertex>> m_VertexData;
    std::vector<std::vector<gfx::Bone>> m_BoneData;
    
    std::vector<std::string> m_MeshName;

    std::vector<void*> m_Vertices;
    std::vector<void*> m_Indices;

    std::vector<uint> m_NumIndices;
    std::vector<gfx::IndexStride> m_IndexStride;

    //Disable default copy constructor and default assignment operator
    BinObjLoader(const BinObjLoader&);
    BinObjLoader& operator=(const BinObjLoader&);
};

} } } //end namespace

#endif
