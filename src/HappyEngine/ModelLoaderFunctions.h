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
//Created: 31/08/2011

#ifndef _HE_MODELLOADERFUNCTIONS_H_
#define _HE_MODELLOADERFUNCTIONS_H_
#pragma once

#include "IModelLoader.h"
#include "VertexLayout.h"
#include "Vector3.h"
#include <vector>

namespace happyengine {
namespace content {
namespace models {

std::vector<math::Vector3> calculateTangents(const void* pVertices, uint numVertices,
                                             uint posOff, uint texOff, uint normOff, uint vertStride,
                                             const void* pIndices, uint numIndices, graphics::IndexStride indexStride);

} } } //end namespace

#endif
