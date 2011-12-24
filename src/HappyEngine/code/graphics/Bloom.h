//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include <vector>
#include "HappyTypes.h"
#include "Texture2D.h"
#include "Shader.h"
#include "ModelMesh.h"

namespace he {
namespace gfx {

class Bloom
{
public:
    Bloom();
    virtual ~Bloom();

    void init(bool hdr);
    void resize();

    const Texture2D::pointer& getBloom(byte level) const;

    void render( const Texture2D* pTexture, const Texture2D* pLumMap = nullptr );

private:
    
    std::vector<uint> m_FboId[2];
    std::vector<Texture2D::pointer> m_Texture[2];
    
    Shader::pointer m_pDownSampleBrightPassShader;
    uint m_DownSampleBrightPassMap;
    uint m_DownSampleBrightPassLumMap;

    Shader::pointer m_pDownSampleShader;
    uint m_DownSampleMap;

    Shader::pointer m_pBlurShaderPass[2];
    uint m_BlurMapPos[2];
    uint m_BlurTexelSize[2];
        
    byte m_DownSamples;

    bool m_Hdr;

    ModelMesh::pointer m_pMesh;

    //Disable default copy constructor and default assignment operator
    Bloom(const Bloom&);
    Bloom& operator=(const Bloom&);
};

} } //end namespace

#endif
