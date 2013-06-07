
addFilter(HAPPYSANDBOX_CODE_SRCS src/runtime/cameras code/runtime/
		FlyCamera.cpp			        FlyCamera.h
    )

addFilter(HAPPYSANDBOX_CODE_SRCS src/runtime/gamestates code/runtime/
		GameStateEdit.cpp			    GameStateEdit.h
        GameStateExit.cpp               GameStateExit.h
        GameStateInit.cpp               GameStateInit.h
        GameStateLoad.cpp               GameStateLoad.h
        GameStateUnload.cpp             GameStateUnload.h
        GameStateLoadPlugin.cpp         GameStateLoadPlugin.h
        GameStateUnloadPlugin.cpp       GameStateUnloadPlugin.h
        GameStatePlay.cpp               GameStatePlay.h
    )

addFilter(HAPPYSANDBOX_CODE_SRCS src/runtime/components code/runtime/
        SandboxEntityComponentFactory.cpp       SandboxEntityComponentFactory.h
        EditorComponent.cpp                     EditorComponent.h
)