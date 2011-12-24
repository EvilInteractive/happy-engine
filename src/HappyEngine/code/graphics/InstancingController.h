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
#include "boost/chrono.hpp"

#include "IDrawable.h"

namespace he {
namespace gfx {

class I3DObject;

class InstancingController : public IInstancedDrawable
{
public:
    InstancingController(bool dynamic, const ModelMesh::pointer& mesh, const Material& material);
    virtual ~InstancingController();

    uint addInstance(const I3DObject* pObj); //return id
    void removeInstance(uint id);
    
    virtual const Material& getMaterial() const;
    virtual void applyMaterial(const ICamera* pCamera) const;
    virtual void applyMaterial(const Material& customMaterial, const ICamera* pCamera) const;

    virtual const ModelMesh::pointer& getModelMesh() const;

    virtual bool getCastsShadow() const;
    virtual void setCastsShadow(bool castShadow);

    virtual bool isVisible() const;
    virtual void setVisible(bool visible);

    virtual float getDrawPriority(const he::gfx::ICamera *) const { return 0.0f; }

    virtual bool isInCamera(const ICamera* /*pCamera*/) const { return true; }; 

    virtual uint getCount() const;

    virtual void draw();
    virtual void drawShadow();

private:
    void init();
    void updateBuffer();

    bool m_Dynamic, m_NeedsUpdate;

    uint m_MatrixBuffer;
    uint m_MatrixBufferCapacity;

    uint m_Vao;
    uint m_ShadowVao;

    ModelMesh::pointer m_pModelMesh;
    Material m_Material;

    bool m_CastShadows;
    bool m_IsVisible;

    uint m_InstancesInBuffer;
    SlotPContainer<const I3DObject*> m_Instances;

    boost::chrono::high_resolution_clock::time_point m_PrevUpdateTime;

    //Disable default copy constructor and default assignment operator
    InstancingController(const InstancingController&);
    InstancingController& operator=(const InstancingController&);
};

} } //end namespace

#endif
