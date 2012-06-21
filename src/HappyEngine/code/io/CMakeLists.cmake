set (CODE_IO_NO_FILTER 
        BinaryStream.cpp                    BinaryStream.h
        ControlsManager.cpp                 ControlsManager.h
        FileReader.cpp                      FileReader.h
        IniReader.cpp                       IniReader.h
        IniWriter.cpp                       IniWriter.h)
        
set (CODE_IO_KEYBOARD_FILTER
        IKeyboard.h
        Keyboard.cpp                        Keyboard.h
        Keys.cpp                            Keys.h)
        
set (CODE_IO_MOUSE_FILTER
        IMouse.h
        Mouse.cpp                           Mouse.h)
        
                        
foreach(f ${CODE_IO_NO_FILTER}) 
    LIST(APPEND CODE_IO_NO_FILTER_SRCS code/io/${f})
endforeach(f) 
                        
foreach(f ${CODE_IO_KEYBOARD_FILTER}) 
    LIST(APPEND CODE_IO_KEYBOARD_FILTER_SRCS code/io/${f})
endforeach(f) 
                        
foreach(f ${CODE_IO_MOUSE_FILTER}) 
    LIST(APPEND CODE_IO_MOUSE_FILTER_SRCS code/io/${f})
endforeach(f) 

source_group (src\\io FILES ${CODE_IO_NO_FILTER_SRCS})
source_group (src\\io\\keyboard FILES ${CODE_IO_KEYBOARD_FILTER_SRCS})
source_group (src\\io\\mouse FILES ${CODE_IO_MOUSE_FILTER_SRCS})

LIST(APPEND HAPPYENGINE_CODE_IO_SRCS ${CODE_IO_NO_FILTER_SRCS} 
                                     ${CODE_IO_KEYBOARD_FILTER_SRCS} 
                                     ${CODE_IO_MOUSE_FILTER_SRCS})
