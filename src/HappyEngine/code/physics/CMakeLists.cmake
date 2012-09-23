set (CODE_PHYSICS_NO_FILTER
        IPhysicsActor.cpp                          IPhysicsActor.h
        PhysicsDynamicActor.cpp                    PhysicsDynamicActor.h
        PhysicsEngine.cpp                          PhysicsEngine.h
        PhysicsMaterial.cpp                        PhysicsMaterial.h
        PhysicsRagdoll.cpp                         PhysicsRagdoll.h
        PhysicsStaticActor.cpp                     PhysicsStaticActor.h
        PhysicsTrigger.cpp                         PhysicsTrigger.h
        PhysicsUserData.h)
        
set (CODE_PHYSICS_JOINTS_FILTER
        IPhysicsJoint.h
        PhysicsFixedJoint.cpp                      PhysicsFixedJoint.h
        PhysicsRevoluteJoint.cpp                   PhysicsRevoluteJoint.h
        PhysicsSphericalJoint.cpp                  PhysicsSphericalJoint.h)

set (CODE_PHYSICS_SHAPES_FILTER
        IPhysicsShape.h
        PhysicsBoxShape.cpp                        PhysicsBoxShape.h
        PhysicsCapsuleShape.cpp                    PhysicsCapsuleShape.h
        PhysicsConcaveMesh.cpp                     PhysicsConcaveMesh.h
        PhysicsConcaveShape.cpp                    PhysicsConcaveShape.h
        PhysicsConvexMesh.cpp                      PhysicsConvexMesh.h
        PhysicsConvexShape.cpp                     PhysicsConvexShape.h
        PhysicsSphereShape.cpp                     PhysicsSphereShape.h)
                        
foreach(f ${CODE_PHYSICS_NO_FILTER}) 
    LIST(APPEND CODE_PHYSICS_NO_FILTER_SRCS code/physics/${f})
endforeach(f)

foreach(f ${CODE_PHYSICS_JOINTS_FILTER}) 
    LIST(APPEND CODE_PHYSICS_JOINTS_FILTER_SRCS code/physics/${f})
endforeach(f)

foreach(f ${CODE_PHYSICS_SHAPES_FILTER}) 
    LIST(APPEND CODE_PHYSICS_SHAPES_FILTER_SRCS code/physics/${f})
endforeach(f) 

source_group (src\\physics FILES ${CODE_PHYSICS_NO_FILTER_SRCS})
source_group (src\\physics\\joints FILES ${CODE_PHYSICS_JOINTS_FILTER_SRCS})
source_group (src\\physics\\shapes FILES ${CODE_PHYSICS_SHAPES_FILTER_SRCS})

LIST(APPEND HAPPYENGINE_CODE_PHYSICS_SRCS   ${CODE_PHYSICS_NO_FILTER_SRCS} 
                                            ${CODE_PHYSICS_JOINTS_FILTER_SRCS} 
                                            ${CODE_PHYSICS_SHAPES_FILTER_SRCS})
