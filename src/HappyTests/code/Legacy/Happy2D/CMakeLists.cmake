set (HAPPY2D_CODE_FILES_SRC_FILTER    
        main.cpp
        MainGame.cpp            MainGame.h)

set (HAPPY2D_CODE_FILES_PCH_FILTER    
        Happy2DPCH.cpp     Happy2DPCH.h)
                        
foreach(f ${HAPPY2D_CODE_FILES_SRC_FILTER}) 
    LIST(APPEND HAPPY2D_CODE_FILES_SRC_FILTER_SRCS ../../code/Happy2D/${f})
endforeach(f)           

foreach(f ${HAPPY2D_CODE_FILES_PCH_FILTER}) 
    LIST(APPEND HAPPY2D_CODE_FILES_PCH_FILTER_SRCS ../../code/Happy2D/${f})
endforeach(f) 

source_group(src FILES ${HAPPY2D_CODE_FILES_SRC_FILTER_SRCS})
source_group(pch FILES ${HAPPY2D_CODE_FILES_PCH_FILTER_SRCS})

set (HAPPY2D_CODE_SRCS  ${HAPPY2D_CODE_FILES_SRC_FILTER_SRCS} 
                        ${HAPPY2D_CODE_FILES_PCH_FILTER_SRCS})
