cmake_minimum_required (VERSION 2.8) 

# include source files
include (${HappyEngine_SOURCE_DIR}/HappyTests/code/HappyPluginLoader/CMakeLists.cmake)
set (HAPPYPLUGINLOADER_SRCS ${HAPPYPLUGINLOADER_CODE_SRCS})

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# include + HappyEngine
include (${HappyEngine_SOURCE_DIR}/HappyEngine/code/CIncludeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/code)
include_directories (${HappyEngine_SOURCE_DIR}/HappyMessageBox/include)
link_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/bin)

# include + link 3thParth
include (${HappyEngine_SOURCE_DIR}/3thParty/CMakeLists.cmake)
IncludeThirdParty()

# add HappyPluginLoader executable
add_executable (HappyPluginLoader ${HAPPYPLUGINLOADER_SRCS})

include(${HappyEngine_SOURCE_DIR}/cmakeHelpers/CopyHappyRuntime.cmake)
copyHappyRuntime(HappyPluginLoader)

# link thirdparty
LinkThirdParty(HappyPluginLoader)

# link HappyEngine
target_link_libraries(HappyPluginLoader HappyEngine)
target_link_libraries(HappyPluginLoader HappyMessageBox)

# setup PCH
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/PCHSupport_26.cmake)
ADD_NATIVE_PRECOMPILED_HEADER(HappyPluginLoader ../../code/HappyPluginLoader/ HappyPluginLoaderPCH.h HappyPluginLoaderPCH.cpp)

# set output dir
SET_TARGET_PROPERTIES(HappyPluginLoader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)
SET_TARGET_PROPERTIES(HappyPluginLoader PROPERTIES FOLDER "HappyTests")
