
addFilter(CODE_SRCS src/game code/game/
            CameraManager.cpp                       CameraManager.h
            DefaultRenderPipeline.cpp				DefaultRenderPipeline.h
            Entity.cpp                              Entity.h
            Game.h
            ITickable.h
        )
                        
                        
addFilter(CODE_SRCS src/game/components code/game/
            CharacterPhysicsComponent.cpp           CharacterPhysicsComponent.h
            DynamicPhysicsComponent.cpp             DynamicPhysicsComponent.h
            InstancedModelComponent.cpp             InstancedModelComponent.h
            LightComponent.cpp                      LightComponent.h
            ModelComponent.cpp                      ModelComponent.h
            SkinnedModelComponent.cpp               SkinnedModelComponent.h
            StaticPhysicsComponent.cpp              StaticPhysicsComponent.h
            TriggerComponent.cpp                    TriggerComponent.h
        )
                        
addFilter(CODE_SRCS src/game/components/factory code/game/
            EntityComponentFactory.cpp              EntityComponentFactory.h
        )

addFilter(CODE_SRCS src/game/components/base code/game/
            EntityComponent.cpp                     EntityComponent.h
        )
