cmake_minimum_required (VERSION 2.8) 

# include source files
include (${HappyEngine_SOURCE_DIR}/HappyTests/code/HappyThijsTest/CMakeLists.cmake)
set (HAPPYTHIJSTEST_SRCS ${HAPPYTHIJSTEST_CODE_SRCS})

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# include + HappyEngine
include (${HappyEngine_SOURCE_DIR}/HappyEngine/code/CIncludeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/code)
link_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/bin)

# include + link 3thParth
include (${HappyEngine_SOURCE_DIR}/3thParty/CMakeLists.cmake)
IncludeThirdParty()

# add HappyThijsTest executable
add_executable (HappyThijsTest ${HAPPYTHIJSTEST_SRCS})

include(${HappyEngine_SOURCE_DIR}/cmakeHelpers/CopyHappyRuntime.cmake)
copyHappyRuntime(HappyThijsTest)

# link thirdparty
LinkThirdParty(HappyThijsTest)

# link HappyEngine
target_link_libraries(HappyThijsTest HappyEngine)

# setup PCH
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/PCHSupport_26.cmake)
ADD_NATIVE_PRECOMPILED_HEADER(HappyThijsTest ../../code/HappyThijsTest/ HappyThijsTestPCH.h HappyThijsTestPCH.cpp)

# set output dir
SET_TARGET_PROPERTIES(HappyThijsTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)
SET_TARGET_PROPERTIES(HappyThijsTest PROPERTIES FOLDER "HappyTests")
