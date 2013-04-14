set (HAPPYPONGSERVER_CODE_FILES_SRC_FILTER
        Ball.cpp                    Ball.h
        HPSStaticDataManager.cpp    HPSStaticDataManager.h
        main.cpp
        MainGame.cpp                MainGame.h
        Obstacle.cpp                Obstacle.h
        Palet.cpp                   Palet.h)

set (HAPPYPONGSERVER_CODE_FILES_PCH_FILTER    
        HappyPongServerPCH.cpp     HappyPongServerPCH.h)
                        
foreach(f ${HAPPYPONGSERVER_CODE_FILES_SRC_FILTER}) 
    LIST(APPEND HAPPYPONGSERVER_CODE_FILES_SRC_FILTER_SRCS ../../code/HappyPongServer/${f})
endforeach(f)           

foreach(f ${HAPPYPONGSERVER_CODE_FILES_PCH_FILTER}) 
    LIST(APPEND HAPPYPONGSERVER_CODE_FILES_PCH_FILTER_SRCS ../../code/HappyPongServer/${f})
endforeach(f) 

source_group(src FILES ${HAPPYPONGSERVER_CODE_FILES_SRC_FILTER_SRCS})
source_group(pch FILES ${HAPPYPONGSERVER_CODE_FILES_PCH_FILTER_SRCS})

set (HAPPYPONGSERVER_CODE_SRCS  ${HAPPYPONGSERVER_CODE_FILES_SRC_FILTER_SRCS} 
                          ${HAPPYPONGSERVER_CODE_FILES_PCH_FILTER_SRCS})
