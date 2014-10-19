cmake_minimum_required (VERSION 2.8) 

# include source files
include (${HappyEngine_SOURCE_DIR}/HappyTests/code/HappyPlugin2DTest/CMakeLists.cmake)
set (HappyPlugin2DTest_SRCS ${HAPPYPLUGIN2DTEST_CODE_SRCS})

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

# add HappyPlugin2DTest executable
add_library (HappyPlugin2DTest SHARED ${HappyPlugin2DTest_SRCS})

# link thirdparty
LinkThirdParty(HappyPlugin2DTest)

# link HappyEngine
target_link_libraries(HappyPlugin2DTest HappyEngine)
target_link_libraries(HappyPlugin2DTest HappyMessageBox)

# setup PCH
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/PCHSupport_26.cmake)
ADD_NATIVE_PRECOMPILED_HEADER(HappyPlugin2DTest ../../code/HappyPlugin2DTest/ HappyPlugin2DTestPCH.h HappyPlugin2DTestPCH.cpp)

# set output dir
SET_TARGET_PROPERTIES(HappyPlugin2DTest PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)
SET_TARGET_PROPERTIES(HappyPlugin2DTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY  ${HappyEngine_SOURCE_DIR}/bin)
SET_TARGET_PROPERTIES(HappyPlugin2DTest PROPERTIES FOLDER "HappyTests")
