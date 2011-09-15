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
//Author:  
//Created: //

#ifndef _HE_IMODEL_LOADER_H_
#define _HE_IMODEL_LOADER_H_
#pragma once

#include <string>
#include "VertexLayout.h"
#include "HappyTypes.h"
#include "Model.h"

namespace happyengine {
namespace content {
namespace models {

class IModelLoader
{
public:
    virtual ~IModelLoader() {}
  
    virtual void load(const std::string& path, const graphics::VertexLayout& vertLayout, bool allowByteIndices = true) = 0;

    virtual uint getNumMeshes() const = 0;
    virtual const std::string& getMeshName(uint mesh) const = 0;

    virtual const void* getVertices(uint mesh) const = 0;
    virtual uint getNumVertices(uint mesh) const = 0;

    virtual const void* getIndices(uint mesh) const = 0;
    virtual graphics::IndexStride getIndexStride(uint mesh) const = 0;
    virtual uint getNumIndices(uint mesh) const = 0;
};

} } } //end namespace

#endif
