
addFilter(CODE_SRCS src/io code/io/
            BinaryVisitor.cpp                   BinaryVisitor.h
            BinaryStreamVisitor.cpp             BinaryStreamVisitor.h
            BinaryFileVisitor.cpp               BinaryFileVisitor.h
            ControlsManager.cpp                 ControlsManager.h
            FileReader.cpp                      FileReader.h
            FileWriter.cpp                      FileWriter.h
            IniReader.cpp                       IniReader.h
            IniWriter.cpp                       IniWriter.h
            JsonFileReader.cpp                  JsonFileReader.h
            JsonFileWriter.cpp                  JsonFileWriter.h
            StructuredVisitor.cpp               StructuredVisitor.h
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
