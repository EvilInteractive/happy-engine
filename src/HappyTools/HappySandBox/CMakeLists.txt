cmake_minimum_required (VERSION 2.8) 

# include source files
include (${HappyEngine_SOURCE_DIR}/HappyTools/HappySandBox/code/CMakeLists.cmake)
set (HAPPYSANDBOX_SRCS ${HAPPYSANDBOX_CODE_SRCS})

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# include + HappyEngine
include (${HappyEngine_SOURCE_DIR}/HappyEngine/code/CIncludeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/code)
link_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/bin)

# include + link 3rdParty
include (${HappyEngine_SOURCE_DIR}/3thParty/CMakeLists.cmake)
if (WIN32)
IncludeThirdParty()
endif()

# add HappySandbox executable
add_executable (HappySandBox ${HAPPYSANDBOX_SRCS})

include(${HappyEngine_SOURCE_DIR}/cmakeHelpers/CopyHappyRuntime.cmake)
copyHappyRuntime(HappySandBox)

# link thirdparty
if (WIN32)
LinkThirdParty(HappySandBox)
endif()

# link HappyEngine
target_link_libraries(HappySandBox HappyEngine)


# setup PCH
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/PCHSupport_26.cmake)
ADD_NATIVE_PRECOMPILED_HEADER(HappySandBox HappySandBoxPCH.h code/HappySandBoxPCH.cpp)

# set output dir
SET_TARGET_PROPERTIES(HappySandBox PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)   
SET_TARGET_PROPERTIES(HappySandBox PROPERTIES FOLDER "HappyTools")
