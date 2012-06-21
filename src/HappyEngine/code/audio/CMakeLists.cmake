set (CODE_AUDIO_FILES   ISound.h
                        Sound2D.h           Sound2D.cpp
                        Sound3D.h           Sound3D.cpp
                        SoundEngine.h       SoundEngine.cpp
                        SoundFile.h         SoundFile.cpp)
                        

foreach(f ${CODE_AUDIO_FILES}) 
    LIST(APPEND HAPPYENGINE_CODE_AUDIO_SRCS code/audio/${f})
endforeach(f) 

source_group (src\\audio FILES ${HAPPYENGINE_CODE_AUDIO_SRCS})
