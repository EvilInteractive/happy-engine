# The name of our project is "HappyEngine". CMakeLists files in this project can 
# refer to the root source directory of the project as ${HappyEngine_SOURCE_DIR} and 
# to the root binary directory of the project as ${HappyEngine_BINARY_DIR}. 
cmake_minimum_required (VERSION 2.8) 
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

if (WIN32)
    set(PLATFORM "win")
elseif (APPLE)
    set(PLATFORM "apple")
    set(CMAKE_OSX_ARCHITECTURES=i386;x86_64)
else()
    set(PLATFORM "linux")
endif()

#if( NOT DEFINED CMAKE_INSTALL_NAME_TOOL)
#	find_program( CMAKE_INSTALL_NAME_TOOL install_name_tool)
#endif()

project (HappyEngine CXX) 

if (WIN32)
    set(PLATFORM "win")
elseif (APPLE)
    set(PLATFORM "osx")
else()
    set(PLATFORM "linux")
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(BITNESS "64")
else()
    SET(BITNESS "32")
endif()

message(Platform: ${PLATFORM}${BITNESS})

SET(Include_HappyEngine on CACHE BOOL "Include HappyEngine in solution")
SET(Include_HappyTests on CACHE BOOL "Include HappyTests in solution")

if (${Include_HappyEngine})
    add_subdirectory (HappyEngine)
endif()

if (${Include_HappyTests})
    add_subdirectory (HappyTests)
endif()

