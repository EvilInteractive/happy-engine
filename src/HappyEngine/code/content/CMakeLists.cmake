set (CODE_CONTENT_NO_FILTER 
						AssetContainer.h
                        ContentManager.cpp                  ContentManager.h
	)
set (CODE_CONTENT_MODEL_FILTER
                        BinObjLineLoader.cpp                BinObjLineLoader.h
                        BinObjLoader.cpp                    BinObjLoader.h
                        IModelLoader.h                      
                        ModelLoader.cpp                     ModelLoader.h
                        ModelLoaderFunctions.cpp            ModelLoaderFunctions.h
                        ObjLoader.cpp                       ObjLoader.h
	)
set (CODE_CONTENT_TEXTURE_FILTER
                        TextureLoader.cpp                   TextureLoader.h
	)
set (CODE_CONTENT_MATERIAL_FILTER
                        MaterialLoader.cpp                  MaterialLoader.h
                        ShaderLoader.cpp                    ShaderLoader.h
                        ShaderPreProcessor.cpp              ShaderPreProcessor.h
	)
set (CODE_CONTENT_PHYSICS_FILTER
                        PhysicsShapeLoader.cpp              PhysicsShapeLoader.h
	)
set (CODE_CONTENT_FONT_FILTER
						FontLoader.cpp                      FontLoader.h
	)
                        
foreach(f ${CODE_CONTENT_NO_FILTER}) 
    LIST(APPEND CODE_CONTENT_NO_FILTER_SRCS code/content/${f})
endforeach(f) 
	foreach(f ${CODE_CONTENT_MODEL_FILTER}) 
    LIST(APPEND CODE_CONTENT_MODEL_FILTER_SRCS code/content/${f})
	endforeach(f) 
	foreach(f ${CODE_CONTENT_TEXTURE_FILTER}) 
    LIST(APPEND CODE_CONTENT_TEXTURE_FILTER_SRCS code/content/${f})
	endforeach(f) 
	foreach(f ${CODE_CONTENT_MATERIAL_FILTER}) 
    LIST(APPEND CODE_CONTENT_MATERIAL_FILTER_SRCS code/content/${f})
	endforeach(f) 
	foreach(f ${CODE_CONTENT_PHYSICS_FILTER}) 
    LIST(APPEND CODE_CONTENT_PHYSICS_FILTER_SRCS code/content/${f})
	endforeach(f) 
	foreach(f ${CODE_CONTENT_FONT_FILTER}) 
    LIST(APPEND CODE_CONTENT_FONT_FILTER_SRCS code/content/${f})
	endforeach(f) 



source_group (src\\content FILES ${CODE_CONTENT_NO_FILTER_SRCS})
	source_group (src\\content\\model FILES ${CODE_CONTENT_MODEL_FILTER_SRCS})
	source_group (src\\content\\texture FILES ${CODE_CONTENT_TEXTURE_FILTER_SRCS})
	source_group (src\\content\\material FILES ${CODE_CONTENT_MATERIAL_FILTER_SRCS})
	source_group (src\\content\\physics FILES ${CODE_CONTENT_PHYSICS_FILTER_SRCS})
	source_group (src\\content\\font FILES ${CODE_CONTENT_FONT_FILTER_SRCS})

LIST(APPEND HAPPYENGINE_CODE_GRAPHICS_SRCS 
        ${CODE_CONTENT_NO_FILTER_SRCS}
        ${CODE_CONTENT_MODEL_FILTER_SRCS}
        ${CODE_CONTENT_TEXTURE_FILTER_SRCS}
        ${CODE_CONTENT_MATERIAL_FILTER_SRCS}
        ${CODE_CONTENT_PHYSICS_FILTER_SRCS}
        ${CODE_CONTENT_FONT_FILTER_SRCS}
        )
