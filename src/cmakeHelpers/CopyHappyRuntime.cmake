MACRO(copyHappyRuntime target)  

add_custom_command(TARGET ${target} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${HappyEngine_SOURCE_DIR}/HappyEngine/bin/$<CONFIGURATION>/HappyEngine.dll"
        $<TARGET_FILE_DIR:${target}>)
        
add_custom_command(TARGET ${target} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${HappyEngine_SOURCE_DIR}/HappyEngine/bin/$<CONFIGURATION>/HappyEngine.pdb"
        $<TARGET_FILE_DIR:${target}>)

ENDMACRO(copyHappyRuntime target)