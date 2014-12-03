cmake_minimum_required (VERSION 2.8) 

# include source files
include (${HappyEngine_SOURCE_DIR}/HappyTools/HappyCooker/HappyCookerDll/src/make/CMakeLists.cmake)
set (HAPPYCOOKERDLL_SRCS ${HAPPYCOOKERDLL_CODE_SRCS})

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# include + HappyEngine
include (${HappyEngine_SOURCE_DIR}/HappyEngine/code/CIncludeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/code)
link_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/bin)

# include + link 3thParth
include (${HappyEngine_SOURCE_DIR}/3thParty/CMakeLists.cmake)
IncludeThirdParty()

# add HappyCookerDll executable
add_library (HappyCookerDll SHARED ${HAPPYCOOKERDLL_SRCS})

# link thirdparty
LinkThirdParty(HappyCookerDll)

# link HappyEngine
target_link_libraries(HappyCookerDll HappyEngine)

# setup PCH
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/PCHSupport_26.cmake)
ADD_NATIVE_PRECOMPILED_HEADER(HappyCookerDll ../src/ HappyCookerPCH.h HappyCookerPCH.cpp)

# set output dir
SET_TARGET_PROPERTIES(HappyCookerDll PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)
SET_TARGET_PROPERTIES(HappyCookerDll PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)
SET_TARGET_PROPERTIES(HappyCookerDll PROPERTIES FOLDER "HappyCooker")
