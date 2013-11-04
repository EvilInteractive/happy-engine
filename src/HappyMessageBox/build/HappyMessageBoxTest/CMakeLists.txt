include(${HappyEngine_SOURCE_DIR}/cmakeHelpers/AddFilterMacro.cmake)

addFilter(HAPPYMESSAGEBOXTEST_SRCS src ../../../HappyMessageBox/src/HappyMessageBoxTest/
            main.cpp
        )
 
include_directories (${HappyEngine_SOURCE_DIR}/HappyMessageBox/include)
IF (APPLE)
find_library(AWESOMIUM NAMES Awesomium)
find_library(SFMLSYSTEM NAMES sfml-system)
find_library(SFMLWINDOW NAMES sfml-window)
ELSE()
include_directories (${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/include)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/lib/${PLATFORM}${BITNESS})
if (MSVC11)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SFML2.0-msvc11/lib/${PLATFORM}${BITNESS})
else()
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SFML2.0/lib/${PLATFORM}${BITNESS})
endif()
ENDIF()

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

add_definitions(-DSFML_STATIC -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS )

# Create HappyJson library
add_executable (HappyMessageBoxTest ${HAPPYMESSAGEBOXTEST_SRCS})

target_link_libraries(HappyMessageBoxTest HappyMessageBox)

# link HappyJson
IF (APPLE)
target_link_libraries(HappyMessageBoxTest ${AWESOMIUM})
target_link_libraries(HappyMessageBoxTest ${SFMLSYSTEM})
target_link_libraries(HappyMessageBoxTest ${SFMLWINDOW})
ELSE()
target_link_libraries(HappyMessageBoxTest awesomium)

target_link_libraries(HappyMessageBoxTest debug sfml-main-d)
target_link_libraries(HappyMessageBoxTest debug sfml-system-s-d)
target_link_libraries(HappyMessageBoxTest debug sfml-window-s-d)
target_link_libraries(HappyMessageBoxTest optimized sfml-main)
target_link_libraries(HappyMessageBoxTest optimized sfml-system-s)
target_link_libraries(HappyMessageBoxTest optimized sfml-window-s)
ENDIF()
#set output dir
SET_TARGET_PROPERTIES(HappyMessageBoxTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)    
SET_TARGET_PROPERTIES(HappyMessageBoxTest PROPERTIES FOLDER "HappyEngine/MessageBox")                    
