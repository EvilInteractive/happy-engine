include(${HappyEngine_SOURCE_DIR}/cmakeHelpers/AddFilterMacro.cmake)

addFilter(HAPPYJSONTESTRUNNER_SRCS src ../../../HappyJson/src/HappyJsonTestRunner/
            main.cpp
        )
 
include_directories (${HappyEngine_SOURCE_DIR}/HappyJson/include)

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# Create HappyJson library
add_executable (HappyJsonTestRunner ${HAPPYJSONTESTRUNNER_SRCS})

# link HappyJson
target_link_libraries(HappyJsonTestRunner HappyJson)

#set output dir
SET_TARGET_PROPERTIES(HappyJsonTestRunner PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/HappyJson/bin)                     
