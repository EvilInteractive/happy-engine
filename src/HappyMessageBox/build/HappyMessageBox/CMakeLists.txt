include(${HappyEngine_SOURCE_DIR}/cmakeHelpers/AddFilterMacro.cmake)

addFilter(HAPPYMESSAGEBOX_SRCS src ../../../HappyMessageBox/src/HappyMessageBox/
            HappyMessageBox.cpp
            MessageBoxWindow.cpp                   MessageBoxWindow.h
            MessageBoxWindowListener.h
        )
addFilter(HAPPYMESSAGEBOX_SRCS src/stack ../../../HappyMessageBox/src/HappyMessageBox/stackwalk/
            StackWalker.cpp                        StackWalker.h
)
 
add_definitions(-DSFML_STATIC -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS )
include_directories (${HappyEngine_SOURCE_DIR}/HappyMessageBox/include)

include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/include)
if (MSVC11)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SFML2.0-mcvc11/include)
else()
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SFML2.0/include)
endif()

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# Create HappyMessageBox library
add_library (HappyMessageBox STATIC ${HAPPYMESSAGEBOX_SRCS})

#set output dir
SET_TARGET_PROPERTIES(HappyMessageBox PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/HappyMessageBox/bin) 
SET_TARGET_PROPERTIES(HappyMessageBox PROPERTIES FOLDER "HappyEngine/MessageBox")                      
  

