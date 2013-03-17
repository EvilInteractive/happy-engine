cmake_minimum_required (VERSION 2.8) 

SET(Include_HappyJsonTests off CACHE BOOL "Include HappyJsonTests in solution")

add_subdirectory (${HappyEngine_SOURCE_DIR}/HappyJson/build/HappyJson)
if(${Include_HappyJsonTests})
add_subdirectory (${HappyEngine_SOURCE_DIR}/HappyJson/build/HappyJsonTest)
add_subdirectory (${HappyEngine_SOURCE_DIR}/HappyJson/build/HappyJsonTestRunner)
endif()

