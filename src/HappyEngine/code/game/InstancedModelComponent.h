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
//Created: 09/12/2011

#ifndef _HE_INSTANCED_MODEL_COMPONENT_H_
#define _HE_INSTANCED_MODEL_COMPONENT_H_
#pragma once

#include "EntityComponent.h"
#include "Object3D.h"
#include "IInstancible.h"

namespace he {
namespace gfx {
    class InstancingController;
}
namespace ge {
  
struct EntityComponentDesc;  
class HAPPY_ENTRY InstancedModelComponent : public EntityComponent, public gfx::IInstancible, public Object3D
{
    IMPLEMENT_IOBJECT3D_FROM(Object3D)
public:
    InstancedModelComponent();
    virtual ~InstancedModelComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                                 ///
    //////////////////////////////////////////////////////////////////////////
    virtual void init(Entity* pParent);

    virtual void visit(he::io::BinaryVisitor* const /*visitor*/) {}

    virtual const he::FixedString& getComponentID() const { return HEFS::strInstancedModelComponent; }

    //// Editor //////////////////////////////////////////////////////////////
    static void fillEntityComponentDesc(EntityComponentDesc& desc);
    virtual bool setProperty(const Property* const inProperty);
    virtual bool getProperty(Property* const inOutProperty);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    
    //////////////////////////////////////////////////////////////////////////
    ///                        IInstancible                                ///
    ////////////////////////////////////////////////////////////////////////// 
    virtual void fillInstancingBuffer(gfx::DynamicBuffer& buffer) const;
    //////////////////////////////////////////////////////////////////////////

    void setController(const he::String& key);
    const he::String& getControllerKey() const;

private:
    he::String m_ControllerKey;
    uint32 m_InstanceId;

    Entity* m_Parent; 

    //Disable default copy constructor and default assignment operator
    InstancedModelComponent(const InstancedModelComponent&);
    InstancedModelComponent& operator=(const InstancedModelComponent&);
};

} } //end namespace

#endif
