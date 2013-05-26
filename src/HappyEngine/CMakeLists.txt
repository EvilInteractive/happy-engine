cmake_minimum_required (VERSION 2.8) 

# include Happy Source
include (code/CMakeLists.cmake)
include_directories (${HappyEngine_SOURCE_DIR}/HappyJson/include)
include_directories (${HappyEngine_SOURCE_DIR}/HappyMessageBox/include)
include_directories (${HappyEngine_SOURCE_DIR}/HappyEngine/code)
set (HAPPYENGINE_SRCS ${HAPPYENGINE_CODE_SRCS})

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# include + link thirdparty
include (${HappyEngine_SOURCE_DIR}/3thParty/CMakeLists.cmake)
if (APPLE)
IncludeThirdPartyOSX()
elseif(WIN32)
IncludeThirdParty()
endif()

# Create HappyEngine library
link_directories (${HappyEngine_SOURCE_DIR}/HappyJson/bin)
add_library (HappyEngine SHARED ${HAPPYENGINE_SRCS})
target_link_libraries(HappyEngine HappyJson)
target_link_libraries(HappyEngine HappyMessageBox)
LinkThirdParty(HappyEngine)

# setup PCH
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/PCHSupport_26.cmake)
if (WIN32)
ADD_NATIVE_PRECOMPILED_HEADER(HappyEngine HappyPCH.h code/HappyPCH.cpp)
else()
ADD_NATIVE_PRECOMPILED_HEADER(HappyEngine code/HappyPCH.h code/HappyPCH.cpp)
endif()

#set output dir
SET_TARGET_PROPERTIES(HappyEngine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/HappyEngine/bin
                                             RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)    
SET_TARGET_PROPERTIES(HappyEngine PROPERTIES FOLDER "HappyEngine")
