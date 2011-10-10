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

#ifndef _HE_SIMPLE_FORWARD_3D_RENDERER_H_
#define _HE_SIMPLE_FORWARD_3D_RENDERER_H_
#pragma once

#include "Model.h"

namespace he {
namespace gfx {

class SimpleForward3DRenderer
{
public:
	SimpleForward3DRenderer();
    virtual ~SimpleForward3DRenderer();

    virtual void draw(const Model::pointer& pModel);
    virtual void draw(const ModelMesh::pointer& pMesh);

private:

    //Disable default copy constructor and default assignment operator
    SimpleForward3DRenderer(const SimpleForward3DRenderer&);
    SimpleForward3DRenderer& operator=(const SimpleForward3DRenderer&);
};

} } //end namespace

#endif
