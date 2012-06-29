cmake_minimum_required (VERSION 2.8) 

# include source files
include (${HappyEngine_SOURCE_DIR}/HappyTests/code/HappyPongServer/CMakeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyTests/code/HappyPongServer)
set (HAPPYPONGSERVER_SRCS ${HAPPYPONGSERVER_CODE_SRCS})

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# include + HappyEngine
include (${HappyEngine_SOURCE_DIR}/HappyEngine/code/CIncludeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/code)
link_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/bin)

# include + link 3thParth
include (${HappyEngine_SOURCE_DIR}/3thParty/CMakeLists.cmake)
IncludeThirdParty()

# add HappyPongServer executable
add_executable (HappyPongServer ${HAPPYPONGSERVER_SRCS})

# link thirdparty
LinkThirdParty(HappyPongServer)

# Copy thirdparty dll's
CopyThirdParty(${HappyEngine_SOURCE_DIR}/HappyTests/bin)

# link HappyEngine
target_link_libraries(HappyPongServer HappyEngine)


# setup PCH
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/PCHSupport_26.cmake)
ADD_NATIVE_PRECOMPILED_HEADER(HappyPongServer HappyPongServerPCH.h ../../code/HappyPongServer/HappyPongServerPCH.cpp)

# set output dir
SET_TARGET_PROPERTIES(HappyPongServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/HappyTests/bin)
