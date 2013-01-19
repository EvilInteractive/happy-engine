
addFilter(CODE_SRCS src/physics code/physics/
            IPhysicsActor.cpp                          IPhysicsActor.h
            PhysicsCharacterController.cpp             PhysicsCharacterController.h
            PhysicsDynamicActor.cpp                    PhysicsDynamicActor.h
            PhysicsEngine.cpp                          PhysicsEngine.h
            PhysicsMaterial.cpp                        PhysicsMaterial.h
            PhysicsRagdoll.cpp                         PhysicsRagdoll.h
            PhysicsStaticActor.cpp                     PhysicsStaticActor.h
            PhysicsTrigger.cpp                         PhysicsTrigger.h
            PhysicsUserData.h
        )
        
addFilter(CODE_SRCS src/physics/joints code/physics/
            IPhysicsJoint.h
            PhysicsFixedJoint.cpp                      PhysicsFixedJoint.h
            PhysicsRevoluteJoint.cpp                   PhysicsRevoluteJoint.h
            PhysicsSphericalJoint.cpp                  PhysicsSphericalJoint.h
        )

addFilter(CODE_SRCS src/physics/shapes code/physics/
            IPhysicsShape.h
            PhysicsBoxShape.cpp                        PhysicsBoxShape.h
            PhysicsCapsuleShape.cpp                    PhysicsCapsuleShape.h
            PhysicsConcaveMesh.cpp                     PhysicsConcaveMesh.h
            PhysicsConcaveShape.cpp                    PhysicsConcaveShape.h
            PhysicsConvexMesh.cpp                      PhysicsConvexMesh.h
            PhysicsConvexShape.cpp                     PhysicsConvexShape.h
            PhysicsSphereShape.cpp                     PhysicsSphereShape.h
        )
