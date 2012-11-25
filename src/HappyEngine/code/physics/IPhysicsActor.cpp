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
//Created: 20/08/2011
#include "HappyPCH.h" 

#include "PxRigidActor.h"

#include "PhysicsEngine.h"
#include "IPhysicsActor.h"
#include "IPhysicsShape.h"

#include "PhysicsBoxShape.h"
#include "PhysicsSphereShape.h"
#include "PhysicsCapsuleShape.h"
#include "PhysicsConcaveShape.h"
#include "PhysicsConvexShape.h"

#include "PhysicsConvexMesh.h"
#include "PhysicsConcaveMesh.h"

namespace he {
namespace px {

bool IPhysicsActor::createShape(he::PrimitiveList<physx::PxShape*>& outShapeList, const IPhysicsShape* shape, 
                                const PhysicsMaterial& material, const mat44& localPose /*= mat44::Identity*/ )
{
    physx::PxShape* pxShape(nullptr);
    physx::PxRigidActor* actor(getInternalActor());
    bool result(false);

    HE_IF_ASSERT((getCompatibleShapes() & shape->getType()) != 0, "Unsupported type for this kind of actor")
    {
        switch (shape->getType())
        {
        case PhysicsShapeType_Box:
            {
                const PhysicsBoxShape* boxShape(static_cast<const PhysicsBoxShape*>(shape));
                pxShape = actor->createShape(
                    physx::PxBoxGeometry(boxShape->getDimension().x / 2.0f, boxShape->getDimension().y / 2.0f, boxShape->getDimension().z / 2.0f), 
                    *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
                outShapeList.add(pxShape);
                result = true;
            } break;
        case PhysicsShapeType_Sphere:
            {
                const PhysicsSphereShape* sphereShape(static_cast<const PhysicsSphereShape*>(shape));
                pxShape = actor->createShape(
                    physx::PxSphereGeometry(sphereShape->getRadius()), 
                    *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
                outShapeList.add(pxShape);
                result = true;
            } break;
        case PhysicsShapeType_Capsule:
            {
                const PhysicsCapsuleShape* capsuleShape(static_cast<const PhysicsCapsuleShape*>(shape));
                pxShape = actor->createShape(
                    physx::PxCapsuleGeometry(capsuleShape->getRadius(), capsuleShape->getHeight() / 2.0f), 
                    *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
                outShapeList.add(pxShape);
                result = true;
            } break;
        case PhysicsShapeType_Convex:
            {
                const PhysicsConvexShape* convexShape(static_cast<const PhysicsConvexShape*>(shape));
                if (convexShape->getConvexMesh() != ObjectHandle::unassigned) // load failed
                {
                    const he::PrimitiveList<physx::PxConvexMesh*>& meshes(
                        ResourceFactory<PhysicsConvexMesh>::getInstance()->get(
                        convexShape->getConvexMesh())->getInternalMeshes());

                    meshes.forEach([&](physx::PxConvexMesh* mesh)
                    {
                        physx::PxVec3 scale;
                        convexShape->getScale().toPxVec3(&scale);

                        pxShape = actor->createShape(
                            physx::PxConvexMeshGeometry(mesh, 
                            physx::PxMeshScale(scale, physx::PxQuat::createIdentity())),
                            *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
                        outShapeList.add(pxShape);
                    });
                    result = true;
                }
            } break;
        case PhysicsShapeType_Concave:
            {
                const PhysicsConcaveShape* concaveShape(static_cast<const PhysicsConcaveShape*>(shape));
                if (concaveShape->getConcaveMesh() != ObjectHandle::unassigned) // load failed
                {
                    const he::PrimitiveList<physx::PxTriangleMesh*>& meshes(
                        ResourceFactory<PhysicsConcaveMesh>::getInstance()->get(
                        concaveShape->getConcaveMesh())->getInternalMeshes());

                    meshes.forEach([&](physx::PxTriangleMesh* mesh)
                    {
                        physx::PxVec3 scale;
                        concaveShape->getScale().toPxVec3(&scale);

                        pxShape = actor->createShape(
                            physx::PxTriangleMeshGeometry(mesh, 
                            physx::PxMeshScale(scale, physx::PxQuat::createIdentity())),
                            *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix()));
                        outShapeList.add(pxShape);
                    });
                    result = true;
                }
            } break;

        default: 
            {
                HE_ASSERT(false, "Unknown type %d", shape->getType());   
            } break;
        }
    }
    return result;
}

void IPhysicsActor::teleport( const mat44& pose )
{
    physx::PxRigidActor* actor(getInternalActor());
    actor->setGlobalPose(physx::PxTransform(pose.getPhyicsMatrix()));
}


} } //end namespace