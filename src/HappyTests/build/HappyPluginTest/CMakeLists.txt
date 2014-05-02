cmake_minimum_required (VERSION 2.8) 

# include source files
include (${HappyEngine_SOURCE_DIR}/HappyTests/code/HappyPluginTest/CMakeLists.cmake)
set (HAPPYPLUGINTEST_SRCS ${HAPPYPLUGINTEST_CODE_SRCS})

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# include + HappyEngine
include (${HappyEngine_SOURCE_DIR}/HappyEngine/code/CIncludeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/code)
link_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/bin)

# include + link 3thParth
include (${HappyEngine_SOURCE_DIR}/3thParty/CMakeLists.cmake)
IncludeThirdParty()

# add HappyPluginTest executable
add_library (HappyPluginTest SHARED ${HAPPYPLUGINTEST_SRCS})

# link thirdparty
LinkThirdParty(HappyPluginTest)

# link HappyEngine
target_link_libraries(HappyPluginTest HappyEngine)

# setup PCH
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/PCHSupport_26.cmake)
ADD_NATIVE_PRECOMPILED_HEADER(HappyPluginTest ../../code/HappyPluginTest/ HappyPluginTestPCH.h HappyPluginTestPCH.cpp)

# set output dir
SET_TARGET_PROPERTIES(HappyPluginTest PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)
SET_TARGET_PROPERTIES(HappyPluginTest PROPERTIES FOLDER "HappyTests")
