cmake_minimum_required (VERSION 2.8) 

# include source files
include (${HappyEngine_SOURCE_DIR}/HappyTests/code/HappyUnitTests/CMakeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyTests/code/HappyUnitTests)
set (HAPPYUNITTESTS_SRCS ${HAPPYUNITTESTS_CODE_SRCS})

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# include + HappyEngine
include (${HappyEngine_SOURCE_DIR}/HappyEngine/code/CIncludeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/code)
link_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/bin)

# include + link 3thParth
include (${HappyEngine_SOURCE_DIR}/3thParty/CMakeLists.cmake)
IncludeThirdParty()

# add HappyUnitTests executable
add_executable (HappyUnitTests ${HAPPYUNITTESTS_SRCS})

# link thirdparty
LinkThirdParty(HappyUnitTests)

# Copy thirdparty dll's
CopyThirdParty(${HappyEngine_SOURCE_DIR}/HappyTests/bin)

# link HappyEngine
target_link_libraries(HappyUnitTests HappyEngine)


# setup PCH
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/PCHSupport_26.cmake)
ADD_NATIVE_PRECOMPILED_HEADER(HappyUnitTests HappyUnitTestsPCH.h ../../code/HappyUnitTests/HappyUnitTestsPCH.cpp)

# set output dir
SET_TARGET_PROPERTIES(HappyUnitTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/HappyTests/bin)
