set (HAPPYBASTEST_CODE_FILES_SRC_FILTER    
        FlyCamera.cpp           FlyCamera.h
        main.cpp
        MainGame.cpp            MainGame.h)

set (HAPPYBASTEST_CODE_FILES_PCH_FILTER    
        HappyBasTestPCH.cpp     HappyBasTestPCH.h)
                        
foreach(f ${HAPPYBASTEST_CODE_FILES_SRC_FILTER}) 
    LIST(APPEND HAPPYBASTEST_CODE_FILES_SRC_FILTER_SRCS ../../code/HappyBasTest/${f})
endforeach(f)           

foreach(f ${HAPPYBASTEST_CODE_FILES_PCH_FILTER}) 
    LIST(APPEND HAPPYBASTEST_CODE_FILES_PCH_FILTER_SRCS ../../code/HappyBasTest/${f})
endforeach(f) 

source_group(src FILES ${HAPPYBASTEST_CODE_FILES_SRC_FILTER_SRCS})
source_group(pch FILES ${HAPPYBASTEST_CODE_FILES_PCH_FILTER_SRCS})

set (HAPPYBASTEST_CODE_SRCS  ${HAPPYBASTEST_CODE_FILES_SRC_FILTER_SRCS} 
                             ${HAPPYBASTEST_CODE_FILES_PCH_FILTER_SRCS})
