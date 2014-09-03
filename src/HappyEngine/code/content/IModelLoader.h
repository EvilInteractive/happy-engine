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

#ifndef _HE_IMODEL_LOADER_H_
#define _HE_IMODEL_LOADER_H_
#pragma once

namespace he {
namespace gfx {
    struct Bone;
    class VertexLayout;
    ENUM(IndexStride, uint8);
}
namespace ct {
namespace models {

class IModelLoader
{
public:
    virtual ~IModelLoader() {}
  
    virtual bool load(const he::String& path, bool allowByteIndices = true) = 0;

    virtual size_t getNumMeshes() const = 0;
    virtual const he::String& getMeshName(uint32 mesh) const = 0;

    virtual const void* getVertices(uint32 mesh) const = 0;
    virtual size_t getNumVertices(uint32 mesh) const = 0;

    virtual const he::ObjectList<gfx::Bone>& getBones(uint32 mesh) const = 0;

    virtual const void* getIndices(uint32 mesh) const = 0;
    virtual gfx::IndexStride getIndexStride(uint32 mesh) const = 0;
    virtual size_t getNumIndices(uint32 mesh) const = 0;

    virtual const gfx::VertexLayout& getVertexLayout() const = 0;
};

} } } //end namespace

#endif
