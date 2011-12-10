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
//Created: 07/12/2011

#ifndef _HE_INSTANCING_CONTROLLER_H_
#define _HE_INSTANCING_CONTROLLER_H_
#pragma once

#include "SlotPContainer.h"
#include "ModelMesh.h"
#include "Material.h"

namespace he {
namespace gfx {

class I3DObject;

class InstancingController
{
public:
    InstancingController(bool dynamic, const ModelMesh::pointer& mesh, const Material& material);
    virtual ~InstancingController();

    uint addInstance(const I3DObject* pObj); //return id
    void removeInstance(uint id);

    void draw();
    void drawShadow();

private:
    void init();


    bool m_Dynamic, m_NeedsUpdate;

    uint m_MatrixBuffer;
    uint m_MatrixBufferCapacity;

    uint m_Vao;
    uint m_ShadowVao;

    ModelMesh::pointer m_pModelMesh;
    Material m_Material;

    SlotPContainer<const I3DObject*> m_Instances;

    //Disable default copy constructor and default assignment operator
    InstancingController(const InstancingController&);
    InstancingController& operator=(const InstancingController&);
};

} } //end namespace

#endif
