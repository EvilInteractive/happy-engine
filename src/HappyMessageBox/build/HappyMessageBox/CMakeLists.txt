include(${HappyEngine_SOURCE_DIR}/cmakeHelpers/AddFilterMacro.cmake)

addFilter(HAPPYMESSAGEBOX_SRCS src ../../../HappyMessageBox/src/HappyMessageBox/
            HappyMessageBox.cpp
            MessageBoxWindow.cpp                   MessageBoxWindow.h
            MessageBoxWindowListener.h
        )
IF (WIN32)
addFilter(HAPPYMESSAGEBOX_SRCS src/stack ../../../HappyMessageBox/src/HappyMessageBox/stackwalk/
            StackWalker.cpp                        StackWalker.h
)
ENDIF()
 
add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS )
include_directories (${HappyEngine_SOURCE_DIR}/HappyMessageBox/include)

if(WIN32)
	include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/awesomium/include)
    include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SDL2/include)
else()
	include_directories(/usr/local/include)
	find_library(AWESOMIUM NAMES Awesomium)
	find_library(SDL2 NAMES SDL2)
endif()


# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# Create HappyMessageBox library
add_library (HappyMessageBox STATIC ${HAPPYMESSAGEBOX_SRCS})

IF (APPLE)
target_link_libraries(HappyMessageBox ${AWESOMIUM})
target_link_libraries(HappyMessageBox ${SDL2})
ENDIF()

#set output dir
SET_TARGET_PROPERTIES(HappyMessageBox PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/HappyMessageBox/bin) 
SET_TARGET_PROPERTIES(HappyMessageBox PROPERTIES FOLDER "HappyEngine/MessageBox")                      
  

