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
class RenderTarget;
class MaterialInstance;

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

    const static int s_BlurPasses = 2;
    const static int s_DownSamples = 4;

    he::PrimitiveList<RenderTarget*> m_RenderTarget;
    he::PrimitiveList<Texture2D*> m_Texture[s_BlurPasses];

    UniformBuffer* m_ToneMapBuffer;
    ModelMesh* m_Quad;
    View* m_View;

    MaterialInstance* m_DownSampleBrightPassMaterial;
    MaterialInstance* m_DownSampleMaterial;
    MaterialInstance* m_BlurPassMaterial[s_BlurPasses];

    int8 m_DownSampleBrightPassMap;
    int8 m_DownSampleBrightPassLumMap;
    int8 m_DownSampleMap;
    int8 m_BlurMapPos[s_BlurPasses];
        
    bool m_Hdr;

    eventCallback0<void> m_ViewportSizeChangedHandler;

    //Disable default copy constructor and default assignment operator
    Bloom(const Bloom&);
    Bloom& operator=(const Bloom&);
};

} } //end namespace

#endif
