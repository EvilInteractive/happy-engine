
include (code/CIncludeLists.cmake)

include (code/audio/CMakeLists.cmake)
include (code/containers/CMakeLists.cmake)
include (code/content/CMakeLists.cmake)
include (code/error/CMakeLists.cmake)
include (code/game/CMakeLists.cmake)
include (code/graphics/CMakeLists.cmake)
include (code/io/CMakeLists.cmake)
include (code/math/CMakeLists.cmake)
include (code/networking/CMakeLists.cmake)
include (code/physics/CMakeLists.cmake)
include (code/tools/CMakeLists.cmake)
                                    
set (CODE_FILES     event.h
                    HappyEngine.h           HappyEngine.cpp
                    HappyMemory.h
                    HappyNew.h
                    HappyPCH.h              HappyPCH.cpp
                    HappyTypes.h            HappyTypes.cpp
                    MessageBox.h            MessageBox.cpp
                    OpenGL.h                OpenGL.cpp
                    Singleton.h             Singleton.cpp
                    StaticDataManager.h     StaticDataManager.cpp)
                        
foreach(f ${CODE_FILES}) 
    LIST(APPEND CODE_SRCS code/${f})
endforeach(f) 

source_group(src FILES ${CODE_SRCS})

set (HAPPYENGINE_CODE_SRCS  ${CODE_SRCS}
                            ${HAPPYENGINE_CODE_AUDIO_SRCS}
                            ${HAPPYENGINE_CODE_CONTAINERS_SRCS}
                            ${HAPPYENGINE_CODE_CONTENT_SRCS}
                            ${HAPPYENGINE_CODE_ERROR_SRCS}
                            ${HAPPYENGINE_CODE_GAME_SRCS}
                            ${HAPPYENGINE_CODE_GRAPHICS_SRCS}
                            ${HAPPYENGINE_CODE_IO_SRCS}
                            ${HAPPYENGINE_CODE_MATH_SRCS}
                            ${HAPPYENGINE_CODE_NETWORKING_SRCS}
                            ${HAPPYENGINE_CODE_PHYSICS_SRCS}
                            ${HAPPYENGINE_CODE_TOOLS_SRCS}
                            )
                            