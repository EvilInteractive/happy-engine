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
//Author:  Sebastiaan Sprengers
//Created: 26/08/2011
#include "HappyPCH.h" 
#include "Simple2DTextureEffect.h"

#include "ContentManager.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialParameter.h"

namespace he {
namespace gfx {

Simple2DTextureEffect::Simple2DTextureEffect()
    : m_Material(nullptr)
    , m_WVP(-1)
    , m_DiffTex(-1)
    , m_Alpha(-1)
    , m_TcOffset(-1)
    , m_TcScale(-1)
    , m_Depth(-1)
{
}


Simple2DTextureEffect::~Simple2DTextureEffect()
{
    delete m_Material;
}

void Simple2DTextureEffect::init(const VertexLayout& layout)
{
    Material* mat(CONTENT->loadMaterial("engine/2D/texture.hm"));
    m_Material = mat->createMaterialInstance(eShaderRenderType_Normal);
    mat->release();
    m_Material->calculateMaterialLayout(layout);
    
    m_WVP = m_Material->findParameter(HEFS::strmatWVP);
    m_DiffTex = m_Material->findParameter(HEFS::strdiffuseMap);
    m_Alpha = m_Material->findParameter(HEFS::strinAlpha);
    m_TcOffset = m_Material->findParameter(HEFS::strtexCoordOffset);
    m_TcScale = m_Material->findParameter(HEFS::strtexCoordScale);
    m_Depth = m_Material->findParameter(HEFS::strdepth);

    setWorldMatrix(mat44::Identity);
}

void Simple2DTextureEffect::apply(const DrawContext& context) const
{
    m_Material->apply(context);
}

void Simple2DTextureEffect::setWorldMatrix(const he::mat44& mat) const
{
    m_Material->getParameter(m_WVP).setFloat44(mat);
}

void Simple2DTextureEffect::setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const
{
    m_Material->getParameter(m_DiffTex).setTexture2D(diffuseMap);
}

void Simple2DTextureEffect::setAlpha(const float alpha) const
{
    m_Material->getParameter(m_Alpha).setFloat(alpha);
}

void Simple2DTextureEffect::setTCOffset(const vec2& offset) const
{
    m_Material->getParameter(m_TcOffset).setFloat2(offset);
}

void Simple2DTextureEffect::setTCScale(const vec2& scale) const
{
    m_Material->getParameter(m_TcScale).setFloat2(scale);
}

void Simple2DTextureEffect::setDepth(float depth) const
{
    m_Material->getParameter(m_Depth).setFloat(depth);
}

void Simple2DTextureEffect::setBlended( const bool blended ) const
{
    if (blended)
    {
        m_Material->setIsBlended(true, BlendEquation_Add, BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
    }
    else
    {
        m_Material->setIsBlended(false);
    }
}

} } //end namespace
