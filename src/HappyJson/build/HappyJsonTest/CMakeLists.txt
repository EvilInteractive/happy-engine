include(${HappyEngine_SOURCE_DIR}/cmakeHelpers/AddFilterMacro.cmake)

addFilter(HAPPYJSONTEST_SRCS src ../../../HappyJson/src/HappyJsonTest/
            jsontest.cpp                jsontest.h
            main.cpp
        )
 
include_directories (${HappyEngine_SOURCE_DIR}/HappyJson/include)

# set compiler options
include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/SetCompilerOptions.cmake)

# Create HappyJson library
add_executable (HappyJsonTest ${HAPPYJSONTEST_SRCS})

# link HappyJson
target_link_libraries(HappyJsonTest HappyJson)

#set output dir
SET_TARGET_PROPERTIES(HappyJsonTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${HappyEngine_SOURCE_DIR}/HappyJson/bin)   
SET_TARGET_PROPERTIES(HappyJsonTest PROPERTIES FOLDER "HappyEngine/Json")                
                            