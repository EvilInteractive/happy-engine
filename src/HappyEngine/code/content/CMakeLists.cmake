
addFilter(CODE_SRCS src/content code/content/
            AssetContainer.h
            ContentManager.cpp                  ContentManager.h
        )
    
addFilter(CODE_SRCS src/content/model code/content/
            BinObjLoader.cpp                    BinObjLoader.h
            IModelLoader.h                      
            ModelLoader.cpp                     ModelLoader.h
            ModelLoaderFunctions.cpp            ModelLoaderFunctions.h
            ObjLoader.cpp                       ObjLoader.h
        )
        
addFilter(CODE_SRCS src/content/texture code/content/
            TextureLoader.cpp                   TextureLoader.h
        )
    
addFilter(CODE_SRCS src/content/material code/content/
            MaterialLoader.cpp                  MaterialLoader.h
            ShaderLoader.cpp                    ShaderLoader.h
            ShaderPreProcessor.cpp              ShaderPreProcessor.h
        )
    
addFilter(CODE_SRCS src/content/material/generator code/content/
            ShaderGenerator.cpp					ShaderGenerator.h
            ShaderGeneratorEnums.cpp            ShaderGeneratorEnums.h
            ShaderGeneratorVariableFactory.cpp  ShaderGeneratorVariableFactory.h
            ShaderGeneratorVariable.cpp			ShaderGeneratorVariable.h
        )
    
addFilter(CODE_SRCS src/content/physics code/content/
            PhysicsShapeLoader.cpp              PhysicsShapeLoader.h
        )
    
addFilter(CODE_SRCS src/content/font code/content/
            FontLoader.cpp                      FontLoader.h
        )
