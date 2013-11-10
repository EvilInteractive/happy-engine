cmake_minimum_required (VERSION 2.8) 

# include source files
include (${HappyEngine_SOURCE_DIR}/HappyTests/code/Xx/CMakeLists.cmake)
set (XX_SRCS ${XX_CODE_SRCS})

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# include + HappyEngine
include (${HappyEngine_SOURCE_DIR}/HappyEngine/code/CIncludeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/code)
link_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/bin)

# include + link 3thParth
include (${HappyEngine_SOURCE_DIR}/3thParty/CMakeLists.cmake)
IncludeThirdParty()

# add Xx executable
add_executable (Xx ${XX_SRCS})

# link thirdparty
LinkThirdParty(Xx)

# link HappyEngine
target_link_libraries(Xx HappyEngine)

# setup PCH
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/PCHSupport_26.cmake)
ADD_NATIVE_PRECOMPILED_HEADER(Xx ../../code/Xx/ XxPCH.h XxPCH.cpp)

# set output dir
SET_TARGET_PROPERTIES(Xx PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)
SET_TARGET_PROPERTIES(Xx PROPERTIES FOLDER "HappyTests")
