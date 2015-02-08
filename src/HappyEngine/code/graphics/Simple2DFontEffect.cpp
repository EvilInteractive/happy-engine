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
//Created: 07/05/2012
#include "HappyPCH.h" 
#include "Simple2DFontEffect.h"

#include "ContentManager.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialParameter.h"

namespace he {
namespace gfx {

Simple2DFontEffect::Simple2DFontEffect() 
    : m_Material(nullptr)
    , m_WVP(-1)
    , m_DiffTex(-1)
    , m_BlendColor(-1)
{
}


Simple2DFontEffect::~Simple2DFontEffect()
{
    HEDelete(m_Material);
}

void Simple2DFontEffect::init(const VertexLayout& layout)
{
    Material* mat(CONTENT->loadMaterial("engine/2D/font.hm"));
    m_Material = mat->createMaterialInstance(eShaderRenderType_Normal);
    mat->release();
    m_Material->calculateMaterialLayout(layout);
    
    m_DiffTex = m_Material->findParameter(HEFS::strdiffuseMap);
    m_BlendColor = m_Material->findParameter(HEFS::strblendColor);
    m_WVP = m_Material->findParameter(HEFS::strmatWVP);

    setWorldMatrix(mat44::Identity);
}

void Simple2DFontEffect::apply(const DrawContext& context) const
{
    m_Material->apply(context);
}

void Simple2DFontEffect::setWorldMatrix(const he::mat44& mat) const
{
    m_Material->getParameter(m_WVP).setFloat44(mat);
}

void Simple2DFontEffect::setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const
{
    m_Material->getParameter(m_DiffTex).setTexture2D(diffuseMap);
}

void Simple2DFontEffect::setBlendColor( const Color& col ) const
{
    m_Material->getParameter(m_BlendColor).setFloat4(col.rgba());
}

} } //end namespace
