set (HAPPYPONG_CODE_FILES_SRC_FILTER
        Ball.cpp                Ball.h
        LightFlashComponent.cpp LightFlashComponent.h
        main.cpp
        MainGame.cpp            MainGame.h
        Obstacle.cpp            Obstacle.h
        Palet.cpp               Palet.h)

set (HAPPYPONG_CODE_FILES_PCH_FILTER    
        HappyPongPCH.cpp     HappyPongPCH.h)
                        
foreach(f ${HAPPYPONG_CODE_FILES_SRC_FILTER}) 
    LIST(APPEND HAPPYPONG_CODE_FILES_SRC_FILTER_SRCS ../../code/HappyPong/${f})
endforeach(f)           

foreach(f ${HAPPYPONG_CODE_FILES_PCH_FILTER}) 
    LIST(APPEND HAPPYPONG_CODE_FILES_PCH_FILTER_SRCS ../../code/HappyPong/${f})
endforeach(f) 

source_group(src FILES ${HAPPYPONG_CODE_FILES_SRC_FILTER_SRCS})
source_group(pch FILES ${HAPPYPONG_CODE_FILES_PCH_FILTER_SRCS})

set (HAPPYPONG_CODE_SRCS  ${HAPPYPONG_CODE_FILES_SRC_FILTER_SRCS} 
                          ${HAPPYPONG_CODE_FILES_PCH_FILTER_SRCS})
