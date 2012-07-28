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
//Created: 30/09/2011
//Changed: 04/11/2011
//added networking: 26/06/2012

#ifndef _HE_ENTITY_H_
#define _HE_ENTITY_H_
#pragma once

#include "IComponent.h"
#include "I3DObject.h"
#include "INetworkSerializable.h"

namespace he {
namespace gfx {
    class Scene;
}
namespace ge {

class Entity : public gfx::I3DObject, public net::INetworkSerializable
{
public:
    Entity();
    virtual ~Entity();
    
    void init(gfx::Scene* scene);

    gfx::Scene* getScene() const { return m_Scene; }

    void addComponent(IComponent* pComponent); //will clean up pComponent
    void deleteComponent(IComponent* pComponent);

    virtual mat44 getWorldMatrix() const;
    void setWorldMatrix(const mat44& mtxWorld);

    bool isSleeping() const;
    void addSleepEvaluator(const boost::function0<bool>& evaluater);
    void removeSleepEvaluator(const boost::function0<bool>& evaluater);

    //////////////////////////////////////////////////////////////////////////
    /// INetworkSerializable
    //////////////////////////////////////////////////////////////////////////
    virtual void serializeCreate(NetworkStream* stream) const;
    virtual bool deserializeCreate(NetworkStream* stream);
    virtual void serializeRemove(NetworkStream* stream) const;
    virtual bool deserializeRemove(NetworkStream* stream);

    virtual bool isSerializeDataDirty() const;
    virtual void serialize(net::NetworkSerializer& serializer);
    virtual void deserialize(net::NetworkDeserializer& serializer);
    //////////////////////////////////////////////////////////////////////////

private:
    bool m_IsSerializeDataDirty;
    mat44 m_mtxWorld;
    std::vector<IComponent*> m_Components;
    gfx::Scene* m_Scene;

    std::vector<boost::function0<bool>> m_SleepEvaluaters;

    //Disable default copy constructor and default assignment operator
    Entity(const Entity&);
    Entity& operator=(const Entity&);
};

} } //end namespace

#endif
