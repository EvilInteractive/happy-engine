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
//Created: 28/10/2011

#ifndef _HE_ICOMPONENT_H_
#define _HE_ICOMPONENT_H_
#pragma once

#include "Object3D.h"
#include "INetworkSerializable.h"

namespace he {
namespace io {
    class BinaryVisitor;
}
namespace ge {
class Entity;
class Property;
struct EntityComponentDesc;
class HAPPY_ENTRY EntityComponent : public IObject3D
{
DECLARE_OBJECT(EntityComponent)
friend class Entity;
public:
    virtual ~EntityComponent() {}

    virtual void visit(he::io::BinaryVisitor* const /*visitor*/) {}

    virtual void activate() {}
    virtual void deactivate() {}

    virtual const he::FixedString& getComponentID() const = 0;

    static void fillEntityComponentDesc(EntityComponentDesc& desc);
    virtual bool setProperty(const Property* const inProperty);
    virtual bool getProperty(Property* const inOutProperty);
    
    virtual Entity* getEntityParent();

    //////////////////////////////////////////////////////////////////////////
    // IObject3D
    //////////////////////////////////////////////////////////////////////////
    virtual void setLocalTranslate(const vec3& translate) = 0;
    virtual void setLocalRotate(const mat33& rotate) = 0;
    virtual void setLocalScale(const vec3& scale) = 0;

    virtual const vec3&  getLocalTranslate() const = 0;
    virtual const mat33& getLocalRotate() const = 0;
    virtual const vec3&  getLocalScale() const = 0;

    virtual const mat44& getLocalMatrix() const = 0;
    virtual const mat44& getWorldMatrix() const = 0;

    virtual void attach(IObject3D* child) = 0;
    virtual void detach(IObject3D* child) = 0;
    
protected:
    virtual IObject3D* getParent() const = 0;
    virtual void setParent(IObject3D* parent) = 0;

    virtual void setWorldMatrixDirty(uint8 cause) = 0;
    virtual void setLocalMatrixDirty(uint8 cause) = 0;

    virtual void calculateWorldMatrix() = 0;
    ////////////////////////////////////////////////////////////////////////// 


    virtual void init(Entity* parent) = 0;

};

} } //end namespace

#endif
