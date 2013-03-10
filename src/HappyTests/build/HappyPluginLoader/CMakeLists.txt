cmake_minimum_required (VERSION 2.8) 

# include source files
include (${HappyEngine_SOURCE_DIR}/HappyTests/code/HappyPluginLoader/CMakeLists.cmake)
set (HAPPYPLUGINLOADER_SRCS ${HAPPYPLUGINLOADER_CODE_SRCS})

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# include + HappyEngine
include (${HappyEngine_SOURCE_DIR}/HappyEngine/code/CIncludeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/code)
link_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/bin)

# include + link 3thParth
include (${HappyEngine_SOURCE_DIR}/3thParty/CMakeLists.cmake)
IncludeThirdParty()

# add HappyPluginLoader executable
add_executable (HappyPluginLoader ${HAPPYPLUGINLOADER_SRCS})

# link thirdparty
LinkThirdParty(HappyPluginLoader)

# Copy thirdparty dll's
CopyThirdParty(${HappyEngine_SOURCE_DIR}/HappyTests/bin)

# link HappyEngine
target_link_libraries(HappyPluginLoader HappyEngine)


# setup PCH
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/PCHSupport_26.cmake)
ADD_NATIVE_PRECOMPILED_HEADER(HappyPluginLoader HappyPluginLoaderPCH.h ../../code/HappyPluginLoader/HappyPluginLoaderPCH.cpp)

# set output dir
SET_TARGET_PROPERTIES(HappyPluginLoader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/HappyTests/bin)
