include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/LibFindPackage.cmake)

macro (IncludeThirdPartyOSX)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.49.0 COMPONENTS date_time system thread regex chrono )

if (Boost_FOUND)
include_directories(${Boost_INCLUDE_DIRS})
message(Boost found!)
else()
message(Could not find boost!)
endif()



# Dependencies
libfind_package(glew glew)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(glew_PKGCONF glew)

# Include dir
find_path(glew_INCLUDE_DIR
  PATHS ${glew_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(glew_LIBRARY
  PATHS ${glew_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(glew_PROCESS_INCLUDES glew_INCLUDE_DIR glew_INCLUDE_DIRS)
set(glew_PROCESS_LIBS glew_LIBRARY glew_LIBRARIES)
libfind_process(glew)
endmacro()


macro (IncludeThirdParty)


include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Assimp/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Boost/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Cairo/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/DevIL/include/${PLATFORM}${BITNESS})
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/FreeType/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/glew/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/libsndfile/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/OpenAl/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/PhysX/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/RakNet/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SFML2.0/include)

add_definitions( -DGLEW_STATIC -DSFML_STATIC -DGLEW_MX )

link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Assimp/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Boost/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Cairo/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/DevIL/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/FreeType/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/glew/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/libsndfile/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/OpenAl/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/PhysX/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/RakNet/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SFML2.0/lib/${PLATFORM}${BITNESS})
endmacro()



macro (LinkThirdPartyOSX target)
target_link_libraries($(target) ${Boost_LIBRARIES})
endmacro()

macro (LinkThirdParty target)
target_link_libraries(${target} assimp)
target_link_libraries(${target} awesomium)

target_link_libraries(${target} debug libboost_chrono-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_date_time-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_regex-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_system-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_thread-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_filesystem-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_signals-vc100-mt-gd-1_49)

target_link_libraries(${target} optimized libboost_chrono-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_date_time-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_regex-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_system-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_thread-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_filesystem-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_signals-vc100-mt-1_49)

target_link_libraries(${target} DevIL)
target_link_libraries(${target} ILU)

target_link_libraries(${target} debug freetype249_D)
target_link_libraries(${target} optimized freetype249)

target_link_libraries(${target} debug glew32mxsd)
target_link_libraries(${target} optimized glew32mxs)
target_link_libraries(${target} libsndfile-1)
target_link_libraries(${target} OpenAL32)
target_link_libraries(${target} cairo)

if (${BITNESS} EQUAL 32)
    set (arch x86)
else()
    set (arch x64)
endif()

target_link_libraries(${target} debug PhysX3CharacterKinematicDEBUG_${arch})
target_link_libraries(${target} debug PhysX3DEBUG_${arch})
target_link_libraries(${target} debug PhysX3CommonDEBUG_${arch})
target_link_libraries(${target} debug PhysX3CookingDEBUG_${arch})
target_link_libraries(${target} debug PhysX3ExtensionsDEBUG)
target_link_libraries(${target} debug PhysX3VehicleDEBUG)
target_link_libraries(${target} debug PhysXProfileSDKDEBUG)
target_link_libraries(${target} debug PhysXVisualDebuggerSDKDEBUG)
target_link_libraries(${target} debug PxTaskDEBUG)
target_link_libraries(${target} debug RepX3DEBUG)
target_link_libraries(${target} debug RepXUpgrader3DEBUG)

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

target_link_libraries(${target} debug sfml-graphics-s-d)
target_link_libraries(${target} debug sfml-main-d)
target_link_libraries(${target} debug sfml-system-s-d)
target_link_libraries(${target} debug sfml-window-s-d)
target_link_libraries(${target} optimized sfml-graphics-s)
target_link_libraries(${target} optimized sfml-main)
target_link_libraries(${target} optimized sfml-system-s)
target_link_libraries(${target} optimized sfml-window-s)
endmacro()

macro (CopyThirdParty target)
file(INSTALL ${HappyEngine_SOURCE_DIR}/3thParty/dll/Debug/${PLATFORM}${BITNESS}/ DESTINATION ${target}/Debug FILES_MATCHING REGEX .*)
file(INSTALL ${HappyEngine_SOURCE_DIR}/3thParty/dll/Release/${PLATFORM}${BITNESS}/ DESTINATION ${target}/MinSizeRel FILES_MATCHING REGEX .*)
file(INSTALL ${HappyEngine_SOURCE_DIR}/3thParty/dll/Release/${PLATFORM}${BITNESS}/ DESTINATION ${target}/Release FILES_MATCHING REGEX .*)
file(INSTALL ${HappyEngine_SOURCE_DIR}/3thParty/dll/Release/${PLATFORM}${BITNESS}/ DESTINATION ${target}/RelWithDebInfo FILES_MATCHING REGEX .*)
endmacro()
