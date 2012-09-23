set (CODE_GAME_NO_FILTER    
                        CameraManager.cpp                       CameraManager.h
                        Entity.cpp                              Entity.h
                        Game.h
                        ITickable.h
                        SerializerStream.cpp                    SerializerStream.h)
                        
                        
set (CODE_GAME_COMPONENT_FILTER 
                        DynamicPhysicsComponent.cpp             DynamicPhysicsComponent.h
                        InstancedModelComponent.cpp             InstancedModelComponent.h
                        LightComponent.cpp                      LightComponent.h
                        ModelComponent.cpp                      ModelComponent.h
                        SkinnedModelComponent.cpp                SkinnedModelComponent.h
                        StaticPhysicsComponent.cpp              StaticPhysicsComponent.h
                        TriggerComponent.cpp                    TriggerComponent.h
                        )
                        
set (CODE_GAME_COMPONENT_FACTORY_FILTER 
                        EntityComponentFactory.cpp              EntityComponentFactory.h
                        )

set (CODE_GAME_COMPONENT_BASE_FILTER 
                        EntityComponent.cpp              EntityComponent.h
                        )
                        
foreach(f ${CODE_GAME_NO_FILTER}) 
    LIST(APPEND GAME_COMPONENT_NO_FILTER_SRCS code/game/${f})
endforeach(f)

foreach(f ${CODE_GAME_COMPONENT_FILTER}) 
    LIST(APPEND GAME_COMPONENT_FILTER_SRCS code/game/${f})
endforeach(f) 

foreach(f ${CODE_GAME_COMPONENT_FACTORY_FILTER}) 
    LIST(APPEND GAME_COMPONENT_FACTORY_FILTER_SRCS code/game/${f})
endforeach(f) 

foreach(f ${CODE_GAME_COMPONENT_BASE_FILTER}) 
    LIST(APPEND GAME_COMPONENT_BASE_FILTER_SRCS code/game/${f})
endforeach(f) 

source_group (src\\game FILES ${GAME_COMPONENT_NO_FILTER_SRCS})
source_group (src\\game\\components FILES ${GAME_COMPONENT_FILTER_SRCS})
source_group (src\\game\\components\\factory FILES ${GAME_COMPONENT_FACTORY_FILTER_SRCS})
source_group (src\\game\\components\\base FILES ${GAME_COMPONENT_BASE_FILTER_SRCS})


LIST(APPEND HAPPYENGINE_CODE_GAME_SRCS 
    ${GAME_COMPONENT_NO_FILTER_SRCS} 
    ${GAME_COMPONENT_FILTER_SRCS} 
    ${GAME_COMPONENT_FACTORY_FILTER_SRCS}
    ${GAME_COMPONENT_BASE_FILTER_SRCS})


