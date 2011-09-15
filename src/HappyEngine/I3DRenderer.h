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

#ifndef _HE_I3DRENDERER_H_
#define _HE_I3DRENDERER_H_
#pragma once

#include "Model.h"
#include "ModelMesh.h"

namespace happyengine {
namespace graphics {

class I3DRenderer
{
public:
    virtual ~I3DRenderer() {}

	virtual void draw(const Model::pointer& pModel) = 0;
	virtual void draw(const ModelMesh::pointer& pModel) = 0;
};

} } //end namespace

#endif
