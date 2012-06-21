set (CODE_CONTENT_FILES AssetContainer.h
                        BinObjLineLoader.cpp                BinObjLineLoader.h
                        BinObjLoader.cpp                    BinObjLoader.h
                        ContentManager.cpp                  ContentManager.h
                        FontLoader.cpp                      FontLoader.h
                        IModelLoader.h                      LineLoader.cpp
                        LineLoader.h
                        MaterialLoader.cpp                  MaterialLoader.h
                        ModelLoader.cpp                     ModelLoader.h
                        ModelLoaderFunctions.cpp            ModelLoaderFunctions.h
                        ObjLineLoader.cpp                   ObjLineLoader.h
                        ObjLoader.cpp                       ObjLoader.h
                        PhysicsShapeLoader.cpp              PhysicsShapeLoader.h
                        ShaderLoader.cpp                    ShaderLoader.h
                        ShaderPreProcessor.cpp              ShaderPreProcessor.h
                        TextureLoader.cpp                   TextureLoader.h)
                        
foreach(f ${CODE_CONTENT_FILES}) 
    LIST(APPEND HAPPYENGINE_CODE_CONTENT_SRCS code/content/${f})
endforeach(f) 

source_group (src\\content FILES ${HAPPYENGINE_CODE_CONTENT_SRCS})
