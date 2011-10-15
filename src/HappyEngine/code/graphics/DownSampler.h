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
//Created: 14/10/2011

#ifndef _HE_DOWNSAMPLER_H_
#define _HE_DOWNSAMPLER_H_
#pragma once

#include <vector>
#include "HappyTypes.h"
#include "Texture2D.h"
#include "Shader.h"
#include "ModelMesh.h"

namespace he {
namespace gfx {

class DownSampler
{
public:
	DownSampler();
    virtual ~DownSampler();

    void init(byte downScales);

    byte getDownSampleLevels() const;
    const Texture2D::pointer& getSample(byte downSampleLevel);

    void downSample(const Texture2D::pointer& pTexture);

private:
    
    std::vector<uint> m_TextureId;
    std::vector<uint> m_FboId;
    std::vector<Texture2D::pointer> m_Texture;

    Shader::pointer m_pDownSamplerShader;
    uint m_TexMapPos;
    uint m_InvScalePos;

    ModelMesh::pointer m_pMesh;

    //Disable default copy constructor and default assignment operator
    DownSampler(const DownSampler&);
    DownSampler& operator=(const DownSampler&);
};

} } //end namespace

#endif
