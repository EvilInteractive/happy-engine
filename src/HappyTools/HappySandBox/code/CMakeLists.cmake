set (HAPPYSANDBOX_CODE_FILES_SRC_FILTER
        main.cpp
        MainGame.cpp            MainGame.h
		FlyCamera.cpp			 FlyCamera.h
		UIController.cpp		 UIController.h
		UIBind.cpp				 UIBind.h)

set (HAPPYSANDBOX_CODE_FILES_PCH_FILTER    
        HappySandBoxPCH.cpp     HappySandBoxPCH.h)
                        
foreach(f ${HAPPYSANDBOX_CODE_FILES_SRC_FILTER}) 
    LIST(APPEND HAPPYSANDBOX_CODE_FILES_SRC_FILTER_SRCS ./code/${f})
endforeach(f)           

foreach(f ${HAPPYSANDBOX_CODE_FILES_PCH_FILTER}) 
    LIST(APPEND HAPPYSANDBOX_CODE_FILES_PCH_FILTER_SRCS ./code/${f})
endforeach(f) 

source_group(src FILES ${HAPPYSANDBOX_CODE_FILES_SRC_FILTER_SRCS})
source_group(pch FILES ${HAPPYSANDBOX_CODE_FILES_PCH_FILTER_SRCS})

set (HAPPYSANDBOX_CODE_SRCS  ${HAPPYSANDBOX_CODE_FILES_SRC_FILTER_SRCS} 
                             ${HAPPYSANDBOX_CODE_FILES_PCH_FILTER_SRCS})
