
addFilter(CODE_SRCS src/io code/io/
            BinaryStream.cpp                    BinaryStream.h
            ControlsManager.cpp                 ControlsManager.h
            FileReader.cpp                      FileReader.h
            FileWriter.cpp                      FileWriter.h
            IniReader.cpp                       IniReader.h
            IniWriter.cpp                       IniWriter.h
        )
            
addFilter(CODE_SRCS src/io/keyboard code/io/
            IKeyboard.h
            Keyboard.cpp                        Keyboard.h
            Keys.cpp                            Keys.h
        )
        
addFilter(CODE_SRCS src/io/mouse code/io/
            IMouse.h
            Mouse.cpp                           Mouse.h
            MouseButtons.h
        )
