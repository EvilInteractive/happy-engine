set (CODE_GRAPHICS_NO_FILTER    
            BufferLayout.cpp                    BufferLayout.h
            Color.cpp                           Color.h
            DefaultSingleDrawable.cpp           DefaultSingleDrawable.h
            DefaultSkinnedDrawable.cpp          DefaultSkinnedDrawable.h
            Hitregion.cpp                       Hitregion.h
            GLContext.h
            IDrawable2D.h
            IShapeDrawable.h
            Object3D.cpp                        Object3D.h
            IDrawable.h    
            IPickable.h 
            SkyBox.cpp                          SkyBox.h
			Texture.cpp							Texture.h
            Texture2D.cpp                       Texture2D.h
            TextureCube.cpp                     TextureCube.h
            Vertex.cpp                          Vertex.h)
                            
set (CODE_GRAPHICS_GUI_FILTER
            Button.cpp                          Button.h
            Font.cpp                            Font.h
            Scrollbar.cpp                       Scrollbar.h
            Text.cpp                            Text.h
            TextBox.cpp                         TextBox.h)
            
set (CODE_GRAPHICS_LIGHT_FILTER
            AmbientLight.cpp                    AmbientLight.h
            DirectionalLight.cpp                DirectionalLight.h                   
            Light.cpp                           Light.h
            LightFactory.cpp                    LightFactory.h
            LightManager.cpp                    LightManager.h
            PointLight.cpp                      PointLight.h
            SpotLight.cpp                       SpotLight.h)
            
set (CODE_GRAPHICS_MATERIAL_FILTER
            Material.cpp                        Material.h
            Shader.cpp                          Shader.h
            ShaderLayout.cpp                    ShaderLayout.h
            ShaderVar.h                         )
            
set (CODE_GRAPHICS_MODEL_FILTER
            Bone.cpp                            Bone.h
            Line.cpp                            Line.h
            Mesh2D.cpp                          Mesh2D.h
            Model.cpp                           Model.h
            ModelMesh.cpp                       ModelMesh.h)
            
set (CODE_GRAPHICS_PICKING_FILTER
            PickEffect.cpp                      PickEffect.h
            Picker.cpp                          Picker.h)
            
set (CODE_GRAPHICS_RENDERING_FILTER
            CameraOrtho.cpp                     CameraOrtho.h
            CameraPerspective.cpp               CameraPerspective.h
            CameraBound.cpp                     CameraBound.h
            CullOctree.cpp                      CullOctree.h
			CullOctreeNodeFactory.cpp			CullOctreeNodeFactory.h
            DynamicBuffer.cpp                   DynamicBuffer.h                     
            ICamera.h                        
            IRenderer.h
            RenderTarget.cpp                    RenderTarget.h
            Scene.cpp                           Scene.h
            View.cpp                            View.h
            Window.cpp                          Window.h)
            
    set (CODE_GRAPHICS_RENDERING_2D_FILTER
                Canvas2D.cpp                        Canvas2D.h
                Renderer2D.cpp                      Renderer2D.h
                Simple2DEffect.cpp                  Simple2DEffect.h
                Simple2DFontEffect.cpp              Simple2DFontEffect.h
                Simple2DTextureEffect.cpp           Simple2DTextureEffect.h
                WebView.cpp                         WebView.h
				WebListener.cpp					    WebListener.h)
                
    set (CODE_GRAPHICS_RENDERING_DEFERRED_FILTER
                Deferred3DRenderer.cpp              Deferred3DRenderer.h)
                
    set (CODE_GRAPHICS_RENDERING_FORWARD_FILTER
                BillboardEffect.cpp                 BillboardEffect.h
                ShapeRenderer.cpp                   ShapeRenderer.h
                Forward3DRenderer.cpp               Forward3DRenderer.h
                SimpleColorEffect.cpp               SimpleColorEffect.h)
                
    set (CODE_GRAPHICS_RENDERING_INSTANCING_FILTER                      
                IInstanceFiller.h
                IInstancible.h   
                InstancingBuffer.cpp                InstancingBuffer.h
                InstancingController.cpp            InstancingController.h
                InstancingManager.cpp               InstancingManager.h)
                
    set (CODE_GRAPHICS_RENDERING_MANAGER_FILTER
                DrawListContainer.cpp               DrawListContainer.h
                GraphicsEngine.cpp                  GraphicsEngine.h 
                RenderSettings.h)
                
    set (CODE_GRAPHICS_RENDERING_POST_FILTER
                AutoExposure.cpp                    AutoExposure.h
                Bloom.cpp                           Bloom.h
                PostProcesser.cpp                   PostProcesser.h)
    
    set (CODE_GRAPHICS_RENDERING_SHADOW_FILTER
                ShadowCaster.cpp                    ShadowCaster.h
				ShadowCasterSpotlight.cpp			ShadowCasterSpotlight.h)
    
    
foreach(f ${CODE_GRAPHICS_NO_FILTER}) 
    LIST(APPEND CODE_GRAPHICS_NO_FILTER_SRCS code/graphics/${f})
endforeach(f) 

foreach(f ${CODE_GRAPHICS_GUI_FILTER}) 
    LIST(APPEND CODE_GRAPHICS_GUI_FILTER_SRCS code/graphics/${f})
endforeach(f) 

foreach(f ${CODE_GRAPHICS_LIGHT_FILTER}) 
    LIST(APPEND CODE_GRAPHICS_LIGHT_FILTER_SRCS code/graphics/${f})
endforeach(f) 

foreach(f ${CODE_GRAPHICS_MATERIAL_FILTER}) 
    LIST(APPEND CODE_GRAPHICS_MATERIAL_FILTER_SRCS code/graphics/${f})
endforeach(f) 

foreach(f ${CODE_GRAPHICS_MODEL_FILTER}) 
    LIST(APPEND CODE_GRAPHICS_MODEL_FILTER_SRCS code/graphics/${f})
endforeach(f) 

foreach(f ${CODE_GRAPHICS_PICKING_FILTER}) 
    LIST(APPEND CODE_GRAPHICS_PICKING_FILTER_SRCS code/graphics/${f})
endforeach(f) 

foreach(f ${CODE_GRAPHICS_RENDERING_FILTER}) 
    LIST(APPEND CODE_GRAPHICS_RENDERING_FILTER_SRCS code/graphics/${f})
endforeach(f) 
    foreach(f ${CODE_GRAPHICS_RENDERING_2D_FILTER}) 
        LIST(APPEND CODE_GRAPHICS_RENDERING_2D_FILTER_SRCS code/graphics/${f})
    endforeach(f) 
    foreach(f ${CODE_GRAPHICS_RENDERING_DEFERRED_FILTER}) 
        LIST(APPEND CODE_GRAPHICS_RENDERING_DEFERRED_FILTER_SRCS code/graphics/${f})
    endforeach(f) 
    foreach(f ${CODE_GRAPHICS_RENDERING_FORWARD_FILTER}) 
        LIST(APPEND CODE_GRAPHICS_RENDERING_FORWARD_FILTER_SRCS code/graphics/${f})
    endforeach(f) 
    foreach(f ${CODE_GRAPHICS_RENDERING_INSTANCING_FILTER}) 
        LIST(APPEND CODE_GRAPHICS_RENDERING_INSTANCING_FILTER_SRCS code/graphics/${f})
    endforeach(f) 
    foreach(f ${CODE_GRAPHICS_RENDERING_MANAGER_FILTER}) 
        LIST(APPEND CODE_GRAPHICS_RENDERING_MANAGER_FILTER_SRCS code/graphics/${f})
    endforeach(f) 
    foreach(f ${CODE_GRAPHICS_RENDERING_POST_FILTER}) 
        LIST(APPEND CODE_GRAPHICS_RENDERING_POST_FILTER_SRCS code/graphics/${f})
    endforeach(f) 
    foreach(f ${CODE_GRAPHICS_RENDERING_SHADOW_FILTER}) 
        LIST(APPEND CODE_GRAPHICS_RENDERING_SHADOW_FILTER_SRCS code/graphics/${f})
    endforeach(f) 

source_group (src\\graphics FILES ${CODE_GRAPHICS_NO_FILTER_SRCS})
source_group (src\\graphics\\gui FILES ${CODE_GRAPHICS_GUI_FILTER_SRCS})
source_group (src\\graphics\\light FILES ${CODE_GRAPHICS_LIGHT_FILTER_SRCS})
source_group (src\\graphics\\material FILES ${CODE_GRAPHICS_MATERIAL_FILTER_SRCS})
source_group (src\\graphics\\model FILES ${CODE_GRAPHICS_MODEL_FILTER_SRCS})
source_group (src\\graphics\\picking FILES ${CODE_GRAPHICS_PICKING_FILTER_SRCS})
source_group (src\\graphics\\rendering FILES ${CODE_GRAPHICS_RENDERING_FILTER_SRCS})
    source_group (src\\graphics\\rendering\\2d FILES ${CODE_GRAPHICS_RENDERING_2D_FILTER_SRCS})
    source_group (src\\graphics\\rendering\\deferred FILES ${CODE_GRAPHICS_RENDERING_DEFERRED_FILTER_SRCS})
    source_group (src\\graphics\\rendering\\forward FILES ${CODE_GRAPHICS_RENDERING_FORWARD_FILTER_SRCS})
    source_group (src\\graphics\\rendering\\instancing FILES ${CODE_GRAPHICS_RENDERING_INSTANCING_FILTER_SRCS})
    source_group (src\\graphics\\rendering\\manager FILES ${CODE_GRAPHICS_RENDERING_MANAGER_FILTER_SRCS})
    source_group (src\\graphics\\rendering\\post FILES ${CODE_GRAPHICS_RENDERING_POST_FILTER_SRCS})
    source_group (src\\graphics\\rendering\\shadow FILES ${CODE_GRAPHICS_RENDERING_SHADOW_FILTER_SRCS})

LIST(APPEND HAPPYENGINE_CODE_GRAPHICS_SRCS 
        ${CODE_GRAPHICS_NO_FILTER_SRCS}
        ${CODE_GRAPHICS_GUI_FILTER_SRCS}
        ${CODE_GRAPHICS_LIGHT_FILTER_SRCS}
        ${CODE_GRAPHICS_MATERIAL_FILTER_SRCS}
        ${CODE_GRAPHICS_MODEL_FILTER_SRCS}
        ${CODE_GRAPHICS_PICKING_FILTER_SRCS}
        ${CODE_GRAPHICS_RENDERING_FILTER_SRCS}
        ${CODE_GRAPHICS_RENDERING_2D_FILTER_SRCS}
        ${CODE_GRAPHICS_RENDERING_DEFERRED_FILTER_SRCS}
        ${CODE_GRAPHICS_RENDERING_FORWARD_FILTER_SRCS}
        ${CODE_GRAPHICS_RENDERING_INSTANCING_FILTER_SRCS}
        ${CODE_GRAPHICS_RENDERING_MANAGER_FILTER_SRCS}
        ${CODE_GRAPHICS_RENDERING_POST_FILTER_SRCS}
        ${CODE_GRAPHICS_RENDERING_SHADOW_FILTER_SRCS}
        )
