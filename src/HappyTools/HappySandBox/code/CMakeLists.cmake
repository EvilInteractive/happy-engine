include(${HappyEngine_SOURCE_DIR}/HappyTools/HappySandBox/code/runtime/CMakeLists.cmake)
include(${HappyEngine_SOURCE_DIR}/HappyTools/HappySandBox/code/system/CMakeLists.cmake)
include(${HappyEngine_SOURCE_DIR}/HappyTools/HappySandBox/code/ui/CMakeLists.cmake)

addFilter(HAPPYSANDBOX_CODE_SRCS src/ code/
        main.cpp
        Sandbox.cpp                 Sandbox.h
        StaticDataManager.cpp       StaticDataManager.h    
    )
       
addFilter(HAPPYSANDBOX_CODE_SRCS pch/ code/
        HappySandBoxPCH.cpp         HappySandBoxPCH.h
    )

