//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 16/10/2011

#ifndef _HE_BLOOM_H_
#define _HE_BLOOM_H_
#pragma once

namespace he {
namespace gfx {

class Texture2D;
class ModelMesh;
class Shader;
class View;
class UniformBuffer;

class Bloom
{
public:
    Bloom();
    virtual ~Bloom();

    void init(View* view, bool hdr, UniformBuffer* toneMapBuffer);

    const Texture2D* getBloom(uint8 level) const;

    void render( const Texture2D* pTexture, const Texture2D* pLumMap = nullptr );

private:
    void resize();
    void cleanTextures();
    void cleanShaders();

    he::PrimitiveList<uint32> m_FboId[2];
    he::PrimitiveList<Texture2D*> m_Texture[2];
    
    Shader* m_DownSampleBrightPassShader;
    uint32 m_DownSampleBrightPassMap;
    uint32 m_DownSampleBrightPassLumMap;
    uint32 m_DownSampleBrightPassToneMapData;
    UniformBuffer* m_ToneMapBuffer;

    Shader* m_DownSampleShader;
    uint32 m_DownSampleMap;

    const static int s_BlurPasses = 2;
    Shader* m_BlurShaderPass[s_BlurPasses];
    uint32 m_BlurMapPos[s_BlurPasses];
    uint32 m_BlurTexelSize[s_BlurPasses];
        
    const uint8 m_DownSamples;

    bool m_Hdr;

    ModelMesh* m_Mesh;
    View* m_View;

    eventCallback0<void> m_ViewportSizeChangedHandler;

    //Disable default copy constructor and default assignment operator
    Bloom(const Bloom&);
    Bloom& operator=(const Bloom&);
};

} } //end namespace

#endif
