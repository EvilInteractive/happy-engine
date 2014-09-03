include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/LibFindPackage.cmake)

if (WIN32)
    add_definitions(-DHE_WINDOWS)
elseif (APPLE)
    add_definitions(-DHE_MAC)
else()
    add_definitions(-DHE_LINUX)
endif()

macro (IncludeThirdParty)

if(APPLE)

include_directories(/usr/local/include)
include_directories(/usr/local/include/freetype2)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/openAL/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/physx/Include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/raknet/Source)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/LibOVR/include)

#libs
link_directories(/usr/local/lib)

find_library(ASSIMP NAMES libassimp.dylib)
find_library(BOOST_CHRONO NAMES libboost_chrono-mt.a)
find_library(BOOST_DATE NAMES libboost_date_time-mt.a)
find_library(BOOST_FILESYSTEM NAMES libboost_filesystem-mt.a)
find_library(BOOST_SYSTEM NAMES libboost_system-mt.a)

find_library(IL NAMES libIL.dylib)
find_library(ILU NAMES libILU.dylib)

find_library(FREETYPE NAMES libfreetype.dylib)

find_library(GLEW NAMES libGLEWmx.a)
find_library(SNDFILE NAMES libsndfile.dylib)
find_library(CAIRO NAMES libcairo.dylib)

#Frameworks
find_library(SDL2 NAMES SDL2)
find_library(APPSERVICES NAMES ApplicationServices)
find_library(COREFOUNDATION NAMES CoreFoundation)
find_library(IOKIT NAMES IOKit)
find_library(OPENGL NAMES OpenGL)

add_definitions( -DGLEW_STATIC -DGLEW_MX -DLLVM )

find_library(OPENAL NAMES libopenal.dylib PATHS ${HappyEngine_SOURCE_DIR}/3thParty/lib/openAL/Lib/${CMAKE_BUILD_TYPE})
find_library(OVR NAMES libovr.a PATHS ${HappyEngine_SOURCE_DIR}/3thParty/lib/LibOVR/Lib/MacOS/${CMAKE_BUILD_TYPE})

SET(PHYSX_PATH ${HappyEngine_SOURCE_DIR}/3thParty/lib/physx/Lib/osx64)
if (${CMAKE_BUILD_TYPE} EQUAL "Debug")
	SET(PHYSX_CHECKED CHECKED)
else()
	SET(PHYSX_CHECKED)
endif()
find_library(PHYSX_LL NAMES libLowLevel${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_LLC NAMES libLowLevelCloth${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_CK NAMES libPhysX3CharacterKinematic${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_P3 NAMES libPhysX3${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_P3COMMON NAMES libPhysX3Common${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_P3COOK NAMES libPhysX3Cooking${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_P3EXT NAMES libPhysX3Extensions${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_P3VEHICLE NAMES libPhysX3Vehicle${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_PROFILE NAMES libPhysXProfileSDK${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_VDEB NAMES libPhysXVisualDebuggerSDK${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_PVD NAMES libPvdRuntime${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_TASK NAMES libPxTask${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_REP NAMES libRepX3${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_REPUPGRADER NAMES libRepXUpgrader3${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_SCENEQUERY NAMES libSceneQuery${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})
find_library(PHYSX_SIMCONTROLLER NAMES libSimulationController${PHYSX_CHECKED}.a PATHS ${PHYSX_PATH})

find_library(RAKNET NAMES libRakNetLibStatic.a PATHS ${HappyEngine_SOURCE_DIR}/3thParty/lib/raknet/Lib/${CMAKE_BUILD_TYPE})

else()

include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Assimp/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/include)
if (MSVC11)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Boost-msvc11/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/PhysX-msvc11/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/RakNet-msvc11/include)
else()
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Boost/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/PhysX/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/RakNet/include)
endif()
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Cairo/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/DevIL/include/${PLATFORM}${BITNESS})
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/FreeType/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/FreeType/include/freetype2)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/fontconfig/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/glew/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/libsndfile/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/LibOVR/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/OpenAl/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SDL2/include)

add_definitions( -DGLEW_STATIC -DGLEW_MX -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS )
if (${BITNESS} EQUAL 64)
	add_definitions(-DARCH_64)
else()
	add_definitions(-DARCH_32)
endif()

link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Assimp/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/lib/${PLATFORM}${BITNESS})
if (MSVC11)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Boost-msvc11/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/PhysX-msvc11/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/RakNet-msvc11/lib/${PLATFORM}${BITNESS})
else()
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Boost/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/PhysX/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/RakNet/lib/${PLATFORM}${BITNESS})
endif()
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Cairo/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/DevIL/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/FreeType/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/fontconfig/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/glew/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/libsndfile/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/LibOVR/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/OpenAl/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SDL2/lib/${PLATFORM}${BITNESS})

endif()

endmacro()

macro (LinkThirdParty target)

if (APPLE)

target_link_libraries(${target} ${SDL2})
target_link_libraries(${target} ${COREFOUNDATION}) 
target_link_libraries(${target} ${IOKIT})
target_link_libraries(${target} ${APPSERVICES})
target_link_libraries(${target} ${OPENGL})

target_link_libraries(${target} ${ASSIMP})

target_link_libraries(${target} ${BOOST_CHRONO})
target_link_libraries(${target} ${BOOST_DATE})
target_link_libraries(${target} ${BOOST_SYSTEM})
target_link_libraries(${target} ${BOOST_FILESYSTEM})

target_link_libraries(${target} ${IL})
target_link_libraries(${target} ${ILU})

target_link_libraries(${target} ${FREETYPE})

target_link_libraries(${target} ${GLEW})
target_link_libraries(${target} ${SNDFILE})
target_link_libraries(${target} ${OPENAL})
target_link_libraries(${target} ${CAIRO})

target_link_libraries(${target} ${PHYSX})
target_link_libraries(${target} ${RAKNET})
target_link_libraries(${target} ${OVR})

target_link_libraries(${target} ${PHYSX_LL})
target_link_libraries(${target} ${PHYSX_LLC})
target_link_libraries(${target} ${PHYSX_CK})
target_link_libraries(${target} ${PHYSX_P3})
target_link_libraries(${target} ${PHYSX_P3COMMON})
target_link_libraries(${target} ${PHYSX_P3COOK})
target_link_libraries(${target} ${PHYSX_P3EXT})
target_link_libraries(${target} ${PHYSX_P3VEHICLE})
target_link_libraries(${target} ${PHYSX_PROFILE})
target_link_libraries(${target} ${PHYSX_VDEB})
target_link_libraries(${target} ${PHYSX_PVD})
target_link_libraries(${target} ${PHYSX_TASK})
target_link_libraries(${target} ${PHYSX_REP})
target_link_libraries(${target} ${PHYSX_REPUPGRADER})
target_link_libraries(${target} ${PHYSX_SCENEQUERY})
target_link_libraries(${target} ${PHYSX_SIMCONTROLLER})

else()

target_link_libraries(${target} assimp)
target_link_libraries(${target} awesomium)

if (MSVC11)
target_link_libraries(${target} debug libboost_chrono-vc110-mt-gd-1_53)
target_link_libraries(${target} debug libboost_date_time-vc110-mt-gd-1_53)
target_link_libraries(${target} debug libboost_filesystem-vc110-mt-gd-1_53)

target_link_libraries(${target} optimized libboost_chrono-vc110-mt-1_53)
target_link_libraries(${target} optimized libboost_date_time-vc110-mt-1_53)
target_link_libraries(${target} optimized libboost_filesystem-vc110-mt-1_53)
else()
target_link_libraries(${target} debug libboost_chrono-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_date_time-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_filesystem-vc100-mt-gd-1_49)

target_link_libraries(${target} optimized libboost_chrono-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_date_time-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_filesystem-vc100-mt-1_49)
endif()

target_link_libraries(${target} DevIL)
target_link_libraries(${target} ILU)

target_link_libraries(${target} freetype)

target_link_libraries(${target} glew32mxs)
target_link_libraries(${target} libsndfile-1)
target_link_libraries(${target} OpenAL32)
target_link_libraries(${target} cairo)

target_link_libraries(${target} debug libovrd)
target_link_libraries(${target} optimized libovr)

if (${BITNESS} EQUAL 32)
    set (arch x86)
else()
    set (arch x64)
endif()

target_link_libraries(${target} debug PhysX3CharacterKinematicCHECKED_${arch})
target_link_libraries(${target} debug PhysX3CHECKED_${arch})
target_link_libraries(${target} debug PhysX3CommonCHECKED_${arch})
target_link_libraries(${target} debug PhysX3CookingCHECKED_${arch})
target_link_libraries(${target} debug PhysX3ExtensionsCHECKED)
target_link_libraries(${target} debug PhysX3VehicleCHECKED)
target_link_libraries(${target} debug PhysXProfileSDKCHECKED)
target_link_libraries(${target} debug PhysXVisualDebuggerSDKCHECKED)
target_link_libraries(${target} debug PxTaskCHECKED)
target_link_libraries(${target} debug RepX3CHECKED)
target_link_libraries(${target} debug RepXUpgrader3CHECKED) 

target_link_libraries(${target} optimized PhysX3CharacterKinematic_${arch})
target_link_libraries(${target} optimized PhysX3_${arch})
target_link_libraries(${target} optimized PhysX3Common_${arch})
target_link_libraries(${target} optimized PhysX3Cooking_${arch})
target_link_libraries(${target} optimized PhysX3Extensions)
target_link_libraries(${target} optimized PhysX3Vehicle)
target_link_libraries(${target} optimized PhysXProfileSDK)
target_link_libraries(${target} optimized PhysXVisualDebuggerSDK)
target_link_libraries(${target} optimized PxTask)
target_link_libraries(${target} optimized RepX3)
target_link_libraries(${target} optimized RepXUpgrader3)

target_link_libraries(${target} debug RakNetStatic-d)
target_link_libraries(${target} optimized RakNetStatic-r)
target_link_libraries(${target} ws2_32)

target_link_libraries(${target} SDL2)
target_link_libraries(${target} SDL2main)

target_link_libraries(${target} opengl32)

endif()

endmacro()

macro (CopyThirdParty target)

if (APPLE)

else()

file(INSTALL ${HappyEngine_SOURCE_DIR}/3thParty/dll/Debug/${PLATFORM}${BITNESS}/ DESTINATION ${target}/Debug FILES_MATCHING REGEX .*)
file(INSTALL ${HappyEngine_SOURCE_DIR}/3thParty/dll/Release/${PLATFORM}${BITNESS}/ DESTINATION ${target}/MinSizeRel FILES_MATCHING REGEX .*)
file(INSTALL ${HappyEngine_SOURCE_DIR}/3thParty/dll/Release/${PLATFORM}${BITNESS}/ DESTINATION ${target}/Release FILES_MATCHING REGEX .*)
file(INSTALL ${HappyEngine_SOURCE_DIR}/3thParty/dll/Release/${PLATFORM}${BITNESS}/ DESTINATION ${target}/RelWithDebInfo FILES_MATCHING REGEX .*)

endif()

endmacro()
