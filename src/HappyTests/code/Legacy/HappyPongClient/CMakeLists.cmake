set (HAPPYPONGCLIENT_CODE_FILES_SRC_FILTER
        Ball.cpp                    Ball.h
        HPCStaticDataManager.cpp    HPCStaticDataManager.h
        LightFlashComponent.cpp     LightFlashComponent.h
        main.cpp
        MainGame.cpp                MainGame.h
        Obstacle.cpp                Obstacle.h
        Palet.cpp                   Palet.h)

set (HAPPYPONGCLIENT_CODE_FILES_PCH_FILTER    
        HappyPongClientPCH.cpp     HappyPongClientPCH.h)
                        
foreach(f ${HAPPYPONGCLIENT_CODE_FILES_SRC_FILTER}) 
    LIST(APPEND HAPPYPONGCLIENT_CODE_FILES_SRC_FILTER_SRCS ../../code/HappyPongClient/${f})
endforeach(f)           

foreach(f ${HAPPYPONGCLIENT_CODE_FILES_PCH_FILTER}) 
    LIST(APPEND HAPPYPONGCLIENT_CODE_FILES_PCH_FILTER_SRCS ../../code/HappyPongClient/${f})
endforeach(f) 

source_group(src FILES ${HAPPYPONGCLIENT_CODE_FILES_SRC_FILTER_SRCS})
source_group(pch FILES ${HAPPYPONGCLIENT_CODE_FILES_PCH_FILTER_SRCS})

set (HAPPYPONGCLIENT_CODE_SRCS  ${HAPPYPONGCLIENT_CODE_FILES_SRC_FILTER_SRCS} 
                          ${HAPPYPONGCLIENT_CODE_FILES_PCH_FILTER_SRCS})
