include(${HappyEngine_SOURCE_DIR}/cmakeHelpers/AddFilterMacro.cmake)

addFilter(HAPPYMESSAGEBOXTEST_SRCS src ../../../HappyMessageBox/src/HappyMessageBoxTest/
            main.cpp
        )
 
include_directories (${HappyEngine_SOURCE_DIR}/HappyMessageBox/include)
IF (APPLE)
    find_library(SDL2 NAMES SDL2)
ELSE()
	include_directories (${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/include)
	link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/lib/${PLATFORM}${BITNESS})
    link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SDL2/lib/${PLATFORM}${BITNESS})
ENDIF()

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS )

# Create HappyJson library
add_executable (HappyMessageBoxTest ${HAPPYMESSAGEBOXTEST_SRCS})

target_link_libraries(HappyMessageBoxTest HappyMessageBox)

# link HappyJson
IF (APPLE)
    target_link_libraries(HappyMessageBoxTest ${SDL2})
ELSE()
	target_link_libraries(HappyMessageBoxTest awesomium)

    target_link_libraries(HappyMessageBoxTest SDL2)
    target_link_libraries(HappyMessageBoxTest SDL2main)
ENDIF()
#set output dir
SET_TARGET_PROPERTIES(HappyMessageBoxTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)    
SET_TARGET_PROPERTIES(HappyMessageBoxTest PROPERTIES FOLDER "HappyEngine/MessageBox")                    
