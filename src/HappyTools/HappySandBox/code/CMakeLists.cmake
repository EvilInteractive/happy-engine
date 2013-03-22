
addFilter(HAPPYSANDBOX_CODE_SRCS src/ code/
        main.cpp
        SandboxTypes.h
        MainGame.cpp                MainGame.h
		FlyCamera.cpp			    FlyCamera.h
    )
 
addFilter(HAPPYSANDBOX_CODE_SRCS src/managers code/
		EntityManager.cpp			EntityManager.h
        SelectionManager.cpp        SelectionManager.h
    )
    
addFilter(HAPPYSANDBOX_CODE_SRCS src/plugin code/
        PluginManager.cpp           PluginManager.h
    )
	
addFilter(HAPPYSANDBOX_CODE_SRCS src/UI code/
        UIController.cpp		    UIController.h
		UIBind.cpp				    UIBind.h
    )
       
addFilter(HAPPYSANDBOX_CODE_SRCS pch/ code/
        HappySandBoxPCH.cpp         HappySandBoxPCH.h
    )

