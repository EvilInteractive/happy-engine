include(${HappyEngine_SOURCE_DIR}/HappyTools/HappySandBox/code/runtime/CMakeLists.cmake)
include(${HappyEngine_SOURCE_DIR}/HappyTools/HappySandBox/code/system/CMakeLists.cmake)
include(${HappyEngine_SOURCE_DIR}/HappyTools/HappySandBox/code/ui/CMakeLists.cmake)
include(${HappyEngine_SOURCE_DIR}/HappyTools/HappySandBox/code/forms/CMakeLists.cmake)
include(${HappyEngine_SOURCE_DIR}/HappyTools/HappySandBox/code/plugins/CMakeLists.cmake)

addFilter(HAPPYSANDBOX_CODE_SRCS src/ code/
        main.cpp
        Sandbox.cpp                 Sandbox.h
        SandboxFixedStrings.cpp     SandboxFixedStrings.h
        SandboxFixedStringsTable.h
        StaticDataManager.cpp       StaticDataManager.h    
    )
       
addFilter(HAPPYSANDBOX_CODE_SRCS pch/ code/
        HappySandBoxPCH.cpp         HappySandBoxPCH.h
    )

