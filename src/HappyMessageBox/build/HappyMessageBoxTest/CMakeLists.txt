include(${HappyEngine_SOURCE_DIR}/cmakeHelpers/AddFilterMacro.cmake)

addFilter(HAPPYMESSAGEBOXTEST_SRCS src ../../../HappyMessageBox/src/HappyMessageBoxTest/
            main.cpp
        )
 
include_directories (${HappyEngine_SOURCE_DIR}/HappyMessageBox/include)
include_directories (${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/include)

link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/lib/${PLATFORM}${BITNESS})
if (MSVC11)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SFML2.0-msvc11/lib/${PLATFORM}${BITNESS})
else()
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SFML2.0/lib/${PLATFORM}${BITNESS})
endif()

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

add_definitions(-DSFML_STATIC -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS )

# Create HappyJson library
add_executable (HappyMessageBoxTest ${HAPPYMESSAGEBOXTEST_SRCS})

# link HappyJson
target_link_libraries(HappyMessageBoxTest HappyMessageBox)
target_link_libraries(HappyMessageBoxTest awesomium)

target_link_libraries(HappyMessageBoxTest debug sfml-main-d)
target_link_libraries(HappyMessageBoxTest debug sfml-system-s-d)
target_link_libraries(HappyMessageBoxTest debug sfml-window-s-d)
target_link_libraries(HappyMessageBoxTest optimized sfml-main)
target_link_libraries(HappyMessageBoxTest optimized sfml-system-s)
target_link_libraries(HappyMessageBoxTest optimized sfml-window-s)

#set output dir
SET_TARGET_PROPERTIES(HappyMessageBoxTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/bin)    
SET_TARGET_PROPERTIES(HappyMessageBoxTest PROPERTIES FOLDER "HappyEngine/MessageBox")                    
