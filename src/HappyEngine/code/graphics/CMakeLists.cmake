
addFilter(CODE_SRCS src/graphics code/graphics/
            BufferLayout.cpp                    BufferLayout.h
            Color.cpp                           Color.h
            Drawable.cpp                        Drawable.h
            Hitregion.cpp                       Hitregion.h
            GLContext.cpp                       GLContext.h
            IDrawable2D.h
            IShapeDrawable.h
            Object3D.cpp                        Object3D.h
            IDrawable.h  
            SkyBox.cpp                          SkyBox.h
			Texture.cpp							Texture.h
            Texture2D.cpp                       Texture2D.h
            TextureCube.cpp                     TextureCube.h
            Vertex.cpp                          Vertex.h
            VertexLayout.cpp                    VertexLayout.h
        )
                            
addFilter(CODE_SRCS src/graphics/gui code/graphics/
            Button.cpp                          Button.h
            Font.cpp                            Font.h
            Scrollbar.cpp                       Scrollbar.h
            Text.cpp                            Text.h
            TextBox.cpp                         TextBox.h
			Sprite.cpp							Sprite.h
			SpriteCreator.cpp					SpriteCreator.h
			Gui.h
			WebView.cpp                         WebView.h
            WebWindow.cpp                       WebWindow.h
			WebListener.cpp					    WebListener.h
			JSObject.cpp						JSObject.h
			Canvas2D.cpp                        Canvas2D.h
        )
            
addFilter(CODE_SRCS src/graphics/light code/graphics/
            AmbientLight.cpp                    AmbientLight.h
            DirectionalLight.cpp                DirectionalLight.h                   
            Light.cpp                           Light.h
            LightFactory.cpp                    LightFactory.h
            LightManager.cpp                    LightManager.h
            PointLight.cpp                      PointLight.h
            SpotLight.cpp                       SpotLight.h
        )
            
addFilter(CODE_SRCS src/graphics/material code/graphics/
            Material.cpp                        Material.h
            MaterialEnums.h
            MaterialInstance.cpp                MaterialInstance.h
            MaterialParameter.cpp               MaterialParameter.h
            Shader.cpp                          Shader.h
            ShaderLayout.cpp                    ShaderLayout.h
            ShaderUniform.inl                   ShaderUniform.h         
            ShaderEnums.cpp                     ShaderEnums.h
            ShaderUniformBuffer.cpp             ShaderUniformBuffer.h
            ShaderUniformBufferManager.cpp      ShaderUniformBufferManager.h
            ShaderUniformFactory.cpp            ShaderUniformFactory.h
        )
            
addFilter(CODE_SRCS src/graphics/model code/graphics/
            Bone.cpp                            Bone.h
            Mesh2D.cpp                          Mesh2D.h
            Model.cpp                           Model.h
            ModelMesh.cpp                       ModelMesh.h
        )
                        
addFilter(CODE_SRCS src/graphics/rendering code/graphics/
            DrawContext.h
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
            Window.cpp                          Window.h
        )
            
addFilter(CODE_SRCS src/graphics/rendering/2D code/graphics/
			Canvas2DBuffer.cpp                  Canvas2DBuffer.h
			Canvas2DRendererGL.cpp              Canvas2DRendererGL.h
			Canvas2DRendererCairo.cpp           Canvas2DRendererCairo.h
			Renderer2D.cpp                      Renderer2D.h
			Simple2DEffect.cpp                  Simple2DEffect.h
			Simple2DFontEffect.cpp              Simple2DFontEffect.h
			Simple2DTextureEffect.cpp           Simple2DTextureEffect.h
			NinePatchEffect.cpp					NinePatchEffect.h
			SpriteData.h
			WebViewSurface.cpp					WebViewSurface.h
			WebViewSurfaceFactory.cpp			WebViewSurfaceFactory.h
        )
            
addFilter(CODE_SRCS src/graphics/rendering/2D/shapes code/graphics/
            BezierShape2D.cpp                   BezierShape2D.h
        )           
			
addFilter(CODE_SRCS src/graphics/rendering/deferred code/graphics/
			Deferred3DRenderer.cpp              Deferred3DRenderer.h
        )
			
addFilter(CODE_SRCS src/graphics/rendering/forward code/graphics/
			BillboardEffect.cpp                 BillboardEffect.h
			ShapeRenderer.cpp                   ShapeRenderer.h
			Forward3DRenderer.cpp               Forward3DRenderer.h
			SimpleColorEffect.cpp               SimpleColorEffect.h
        )
			
addFilter(CODE_SRCS src/graphics/rendering/instancing code/graphics/                    
			IInstanceFiller.h
			IInstancible.h   
			InstancingBuffer.cpp                InstancingBuffer.h
			InstancingController.cpp            InstancingController.h
			InstancingManager.cpp               InstancingManager.h
        )
			
addFilter(CODE_SRCS src/graphics/rendering/manager code/graphics/
			DrawListContainer.cpp               DrawListContainer.h
			GraphicsEngine.cpp                  GraphicsEngine.h 
			RenderSettings.cpp                  RenderSettings.h
        )
			
addFilter(CODE_SRCS src/graphics/rendering/post code/graphics/
			AutoExposure.cpp                    AutoExposure.h
			Bloom.cpp                           Bloom.h
			PostProcesser.cpp                   PostProcesser.h
        )

addFilter(CODE_SRCS src/graphics/rendering/shadow code/graphics/
			ShadowCaster.cpp                    ShadowCaster.h
			ShadowCasterSpotlight.cpp			ShadowCasterSpotlight.h
        )
