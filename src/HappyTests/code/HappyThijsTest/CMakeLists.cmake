set (HAPPYTHIJSTEST_CODE_FILES_SRC_FILTER    
        main.cpp
        MainGame.cpp            MainGame.h
		AStar.cpp				AStar.h
		Grid.cpp				Grid.h
		Level.cpp				Level.h
		Node.cpp				Node.h)

set (HAPPYTHIJSTEST_CODE_FILES_PCH_FILTER    
        HappyThijsTestPCH.cpp     HappyThijsTestPCH.h)
                        
foreach(f ${HAPPYTHIJSTEST_CODE_FILES_SRC_FILTER}) 
    LIST(APPEND HAPPYTHIJSTEST_CODE_FILES_SRC_FILTER_SRCS ../../code/HappyThijsTest/${f})
endforeach(f)           

foreach(f ${HAPPYTHIJSTEST_CODE_FILES_PCH_FILTER}) 
    LIST(APPEND HAPPYTHIJSTEST_CODE_FILES_PCH_FILTER_SRCS ../../code/HappyThijsTest/${f})
endforeach(f) 

source_group(src FILES ${HAPPYTHIJSTEST_CODE_FILES_SRC_FILTER_SRCS})
source_group(pch FILES ${HAPPYTHIJSTEST_CODE_FILES_PCH_FILTER_SRCS})

set (HAPPYTHIJSTEST_CODE_SRCS  ${HAPPYTHIJSTEST_CODE_FILES_SRC_FILTER_SRCS} 
                   ${HAPPYTHIJSTEST_CODE_FILES_PCH_FILTER_SRCS})
