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
//Author:  Bastian Damman
//Created: 07/12/2011

#ifndef _HE_INSTANCING_CONTROLLER_H_
#define _HE_INSTANCING_CONTROLLER_H_
#pragma once

#include "SlotPContainer.h"

#include "IDrawable.h"
#include "BufferLayout.h"
#include "InstancingBuffer.h"
#include "Bound.h"
#include "ITickable.h"

namespace he {
namespace gfx {

class IInstancible;
class IInstanceFiller;
class ModelMesh;
class Material;
class Scene;

class InstancingController : public InstancedDrawable, public ge::ITickable
{
public:
    InstancingController(const std::string& name, bool dynamic, const ObjectHandle& modelHandle, const ObjectHandle& material);
    virtual ~InstancingController();

    uint32 addInstance(const IInstancible* pObj); //return id
    uint32 addInstance(); //return id // only for manual mode
    void removeInstance(uint32 id);
    virtual uint32 getCount() const;

    void addManualFiller(IInstanceFiller* pFiller);
    void removeManualFiller(const IInstanceFiller* pFiller);
    
    //////////////////////////////////////////////////////////////////////////
    /// IDrawable
    //////////////////////////////////////////////////////////////////////////
    virtual const Material* getMaterial() const;
    virtual void applyMaterial(const ICamera* pCamera) const;
    virtual void applyMaterial(const Material* customMaterial, const ICamera* pCamera) const;

    virtual void detachFromScene();
    virtual void attachToScene(Scene* scene);
    virtual void setScene(Scene* scene);
    virtual Scene* getScene() const;
    virtual bool isAttachedToScene() const;

    virtual void calculateBound();
    virtual const Bound& getBound() const { return m_Bound; }

    virtual const ModelMesh* getModelMesh() const;

    virtual bool getCastsShadow() const;
    virtual void setCastsShadow(bool castShadow);

    virtual void draw();
    virtual void drawShadow();


    //////////////////////////////////////////////////////////////////////////
    // ITickable
    //////////////////////////////////////////////////////////////////////////
    virtual void tick(float dTime);

private:
    void init();
    void initVao(GLContext* context);
    void destroyVao(GLContext* context);
    void updateBuffer();

    bool m_Dynamic, m_NeedsUpdate, m_ManualMode;

    std::string m_Name;

    details::InstancingBuffer m_CpuBuffer;
    uint32  m_GpuBuffer;
    uint32  m_BufferCapacity;

    VaoID m_Vao[MAX_VERTEX_ARRAY_OBJECTS];
    VaoID m_ShadowVao[MAX_VERTEX_ARRAY_OBJECTS];

    BufferLayout m_InstancingLayout;
    const ModelMesh* m_pModelMesh;
    Material* m_Material;

    bool m_CastShadows;
    Bound m_Bound;

    SlotPContainer<const IInstancible*> m_Instances;

    boost::chrono::high_resolution_clock::time_point m_PrevUpdateTime;

    std::vector<IInstanceFiller*> m_ManualCpuBufferFillers;

    Scene* m_Scene;

    eventCallback1<void, GLContext*> m_ContextCreatedHandler;
    eventCallback1<void, GLContext*> m_ContextRemovedHandler;

    //Disable default copy constructor and default assignment operator
    InstancingController(const InstancingController&);
    InstancingController& operator=(const InstancingController&);
};

} } //end namespace

#endif
