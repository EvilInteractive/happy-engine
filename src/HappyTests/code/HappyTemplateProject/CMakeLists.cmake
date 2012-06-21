set (XX_CODE_FILES_SRC_FILTER    
        main.cpp
        MainGame.cpp            MainGame.h)

set (XX_CODE_FILES_PCH_FILTER    
        XxPCH.cpp     XxPCH.h)
                        
foreach(f ${XX_CODE_FILES_SRC_FILTER}) 
    LIST(APPEND XX_CODE_FILES_SRC_FILTER_SRCS ../../code/Xx/${f})
endforeach(f)           

foreach(f ${XX_CODE_FILES_PCH_FILTER}) 
    LIST(APPEND XX_CODE_FILES_PCH_FILTER_SRCS ../../code/Xx/${f})
endforeach(f) 

source_group(src FILES ${XX_CODE_FILES_SRC_FILTER_SRCS})
source_group(pch FILES ${XX_CODE_FILES_PCH_FILTER_SRCS})

set (XX_CODE_SRCS  ${XX_CODE_FILES_SRC_FILTER_SRCS} 
                   ${XX_CODE_FILES_PCH_FILTER_SRCS})
