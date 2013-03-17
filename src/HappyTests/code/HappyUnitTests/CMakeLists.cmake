set (HAPPYUNITTESTS_CODE_FILES_SRC_FILTER    
        main.cpp
        MainGame.cpp                MainGame.h
        UnitTestFixedStrings.cpp    UnitTestFixedStrings.h
        UnitTestFixedStringsTable.h
    )

set (HAPPYUNITTESTS_CODE_FILES_PCH_FILTER    
        HappyUnitTestsPCH.cpp     HappyUnitTestsPCH.h)
                        
foreach(f ${HAPPYUNITTESTS_CODE_FILES_SRC_FILTER}) 
    LIST(APPEND HAPPYUNITTESTS_CODE_FILES_SRC_FILTER_SRCS ../../code/HappyUnitTests/${f})
endforeach(f)           

foreach(f ${HAPPYUNITTESTS_CODE_FILES_PCH_FILTER}) 
    LIST(APPEND HAPPYUNITTESTS_CODE_FILES_PCH_FILTER_SRCS ../../code/HappyUnitTests/${f})
endforeach(f) 

source_group(src FILES ${HAPPYUNITTESTS_CODE_FILES_SRC_FILTER_SRCS})
source_group(pch FILES ${HAPPYUNITTESTS_CODE_FILES_PCH_FILTER_SRCS})

set (HAPPYUNITTESTS_CODE_SRCS  ${HAPPYUNITTESTS_CODE_FILES_SRC_FILTER_SRCS} 
							   ${HAPPYUNITTESTS_CODE_FILES_PCH_FILTER_SRCS})
