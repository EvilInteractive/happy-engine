cmake_minimum_required (VERSION 2.8) 

SET(Include_HappyMessageBoxTests off CACHE BOOL "Include HappyMessageBoxTests in solution")

add_subdirectory (${HappyEngine_SOURCE_DIR}/HappyMessageBox/build/HappyMessageBox)

if (${Include_HappyMessageBoxTests})
add_subdirectory (${HappyEngine_SOURCE_DIR}/HappyMessageBox/build/HappyMessageBoxTest)
endif()

