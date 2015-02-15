 
addFilter(HAPPYSANDBOX_CODE_SRCS src/system/ code/system/
        SandboxTypes.h
        SandboxRenderPipeline.cpp   SandboxRenderPipeline.h
        GLContextQT.cpp             GLContextQT.h
    )

addFilter(HAPPYSANDBOX_CODE_SRCS src/system/statemachine code/system/
        GameState.cpp               GameState.h
        GameStateMachine.cpp        GameStateMachine.h    
    )

addFilter(HAPPYSANDBOX_CODE_SRCS src/system/managers code/system/
		EntityManager.cpp			EntityManager.h
        SelectionManager.cpp        SelectionManager.h
        EditorPickingManager.cpp    EditorPickingManager.h    
        InteractionManager.cpp      InteractionManager.h   
    )

addFilter(HAPPYSANDBOX_CODE_SRCS src/system/interaction code/system/
		IInteractionSet.h
		IInteractionMode.h
	)

addFilter(HAPPYSANDBOX_CODE_SRCS src/system/interaction/entity code/system/
		EntityInteractionSet.h 			EntityInteractionSet.cpp
		EntityMoveInteractionMode.h 	EntityMoveInteractionMode.cpp
		EntityRotateInteractionMode.h 	EntityRotateInteractionMode.cpp
		EntityScaleInteractionMode.h 	EntityScaleInteractionMode.cpp
	)
    
addFilter(HAPPYSANDBOX_CODE_SRCS src/system/plugin code/system/
        PluginManager.cpp           PluginManager.h
    )
    
addFilter(HAPPYSANDBOX_CODE_SRCS src/system/options code/system/
        Options.cpp           Options.h
    )
	
addFilter(HAPPYSANDBOX_CODE_SRCS src/system/ui code/system/
        UI.cpp                      UI.h
        UIManager.cpp               UIManager.h
    )

