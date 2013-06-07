
addFilter(CODE_SRCS src/game code/game/
            CameraManager.cpp                       CameraManager.h
            DefaultRenderPipeline.cpp				DefaultRenderPipeline.h
            Entity.cpp                              Entity.h
            Game.h
            ITickable.h
        )

addFilter(CODE_SRCS src/game/picking code/game/
            IPickingManager.h
            PickingManager.cpp                      PickingManager.h
            PickResult.h
            Pickable.cpp                            Pickable.h
        )
                        
addFilter(CODE_SRCS src/game/components/components code/game/
            CharacterPhysicsComponent.cpp           CharacterPhysicsComponent.h
            DynamicPhysicsComponent.cpp             DynamicPhysicsComponent.h
            InstancedModelComponent.cpp             InstancedModelComponent.h
            LightComponent.cpp                      LightComponent.h
            ModelComponent.cpp                      ModelComponent.h
            PickingComponent.cpp                    PickingComponent.h
            SkinnedModelComponent.cpp               SkinnedModelComponent.h
            StaticPhysicsComponent.cpp              StaticPhysicsComponent.h
            TriggerComponent.cpp                    TriggerComponent.h
        )
                        
addFilter(CODE_SRCS src/game/components code/game/
            EntityManager.cpp                       EntityManager.h
        )   
        
addFilter(CODE_SRCS src/game/components/factory code/game/
            IEntityComponentFactory.h
            EngineEntityComponentFactory.cpp        EngineEntityComponentFactory.h
        )

addFilter(CODE_SRCS src/game/components/base code/game/
            EntityComponent.cpp                     EntityComponent.h
            EntityComponentDesc.cpp                 EntityComponentDesc.h
        )

addFilter(CODE_SRCS src/game/components/editor code/game/
            Property.cpp                            Property.h
            PropertyFeel.cpp                        PropertyFeel.h
            PropertyConverter.cpp                   PropertyConverter.h
        )