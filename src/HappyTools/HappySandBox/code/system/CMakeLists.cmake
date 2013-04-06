 
addFilter(HAPPYSANDBOX_CODE_SRCS src/system/ code/system/
        SandboxTypes.h
        SandboxRenderPipeline.cpp   SandboxRenderPipeline.h
    )

addFilter(HAPPYSANDBOX_CODE_SRCS src/system/statemachine code/system/
        GameState.cpp               GameState.h
        GameStateMachine.cpp        GameStateMachine.h    
    )

addFilter(HAPPYSANDBOX_CODE_SRCS src/system/managers code/system/
		EntityManager.cpp			EntityManager.h
        SelectionManager.cpp        SelectionManager.h
    )
    
addFilter(HAPPYSANDBOX_CODE_SRCS src/system/plugin code/system/
        PluginManager.cpp           PluginManager.h
    )
	
addFilter(HAPPYSANDBOX_CODE_SRCS src/system/ui code/system/
        UI.cpp                      UI.h
        UIManager.cpp               UIManager.h
    )

