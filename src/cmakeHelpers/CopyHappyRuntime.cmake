MACRO(copyHappyRuntime target)  

#add_custom_command(TARGET ${target} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${HappyEngine_SOURCE_DIR}/HappyEngine/bin/$<CONFIGURATION>/HappyEngine.dll"
#        $<TARGET_FILE_DIR:${target}>)
#        
#add_custom_command(TARGET ${target} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${HappyEngine_SOURCE_DIR}/HappyEngine/bin/$<CONFIGURATION>/HappyEngine.pdb"
#        $<TARGET_FILE_DIR:${target}>
#        )

ENDMACRO(copyHappyRuntime target)