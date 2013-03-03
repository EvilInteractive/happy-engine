
addFilter(HAPPYSANDBOX_CODE_SRCS src/ code/
        main.cpp
        MainGame.cpp                MainGame.h
		FlyCamera.cpp			    FlyCamera.h
		UIController.cpp		    UIController.h
		UIBind.cpp				    UIBind.h
    )
 
addFilter(HAPPYSANDBOX_CODE_SRCS src/plugin code/
        PluginManager.cpp           PluginManager.h
    )
       
addFilter(HAPPYSANDBOX_CODE_SRCS pch/ code/
        HappySandBoxPCH.cpp         HappySandBoxPCH.h
    )

