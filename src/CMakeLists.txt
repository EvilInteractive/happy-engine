# The name of our project is "HappyEngine". CMakeLists files in this project can 
# refer to the root source directory of the project as ${HappyEngine_SOURCE_DIR} and 
# to the root binary directory of the project as ${HappyEngine_BINARY_DIR}. 
cmake_minimum_required (VERSION 2.8) 
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE OFF)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

if (WIN32)
    set(PLATFORM "win")
elseif (APPLE)
    set(PLATFORM "apple")
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


# Compiler flags
if (MSVC)
    message(msvc)
    string(REPLACE "/Zm1000" "/Zm200" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    #set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Deployment target for OSX" FORCE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Deployment target for OSX" FORCE)

    find_program(XCODE NAMES XCode)
    find_path(SYS_TEMP NAMES MacOSX10.9.sdk PATHS ${XCODE}/../../Developer/Platforms/MacOSX.platform/Developer/SDKs)
    set(CMAKE_OSX_SYSROOT ${SYS_TEMP}/MacOSX10.9.sdk CACHE STRING "" FORCE)
    set(SYS_TEMP)

    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -F/Library/Frameworks")
    message(clang)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
    message(gcc)
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(BITNESS "64")
else()
    SET(BITNESS "32")
endif()

message(Platform: ${PLATFORM}${BITNESS})

SET(Include_HappyEngine on CACHE BOOL "Include HappyEngine in solution")
SET(Include_HappyJson on CACHE BOOL "Include HappyJson in solution")
SET(Include_HappyMessageBox on CACHE BOOL "Include HappyMessageBox in solution")
SET(Include_HappyTests on CACHE BOOL "Include HappyTests in solution")
SET(Include_HappySandBox on CACHE BOOL "Include HappySandBox in solution")

if (${Include_HappyEngine})
    add_subdirectory (HappyEngine)
endif()

if (${Include_HappyJson})
    include (HappyJson/CMakeIncludeLists.cmake)
endif()

if (${Include_HappyMessageBox})
    include (HappyMessageBox/CMakeIncludeLists.cmake)
endif()

if (${Include_HappyTests})
    add_subdirectory (HappyTests)
endif()

if (${Include_HappySandBox})
    add_subdirectory (HappyTools/HappySandBox)
endif()

# Copy thirdparty dll's
if (WIN32)
CopyThirdParty(${HappyEngine_SOURCE_DIR}/bin)
endif()
