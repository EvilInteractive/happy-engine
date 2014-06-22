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
//Created: 14/12/2011

#ifndef _HE_PHYSICS_ACTOR_H_
#define _HE_PHYSICS_ACTOR_H_
#pragma once

namespace physx {
class PxRigidActor;
class PxShape;
}

namespace he {

struct vec3;
class mat44;

namespace px {
class PhysicsUserData;
class IPhysicsShape;
class PhysicsMaterial;

enum PhysicsUserDataContainerType
{
    PhysicsUserDataContainerType_Dynamic,
    PhysicsUserDataContainerType_Static,
    PhysicsUserDataContainerType_Trigger,
    PhysicsUserDataContainerType_Controller
};

class IPhysicsUserDataContainer
{
public:
    virtual const PhysicsUserData& getUserData() const = 0;
    virtual PhysicsUserDataContainerType getType() const = 0;
};

class PhysicsActor : public IPhysicsUserDataContainer
{
public:
    PhysicsActor();
    virtual ~PhysicsActor();

    virtual physx::PxRigidActor* getInternalActor() const = 0;
    virtual void getTranslation(vec3& translation) const = 0;
    virtual void getRotation(mat33& rotation) const = 0;
    virtual void getPose(mat44& pose) const = 0;

    virtual void teleport(const mat44& pose);

    // Not overrideable
    void attachToScene();
    void detachFromScene();

protected:
    virtual uint32 getCompatibleShapes() const = 0;
    // return true if successful
    virtual bool createShape(he::PrimitiveList<physx::PxShape*>& outShapeList, const IPhysicsShape* shape, 
                             const PhysicsMaterial& material, const mat44& localPose = mat44::Identity);

private:
    bool m_IsAttachedToScene;
};

} } //end namespace

#endif
