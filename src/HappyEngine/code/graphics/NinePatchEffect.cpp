//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Created: 27/11/2012
#include "HappyPCH.h" 
#include "NinePatchEffect.h"

#include "ContentManager.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialParameter.h"

namespace he {
namespace gfx {

NinePatchEffect::NinePatchEffect()
    : m_Material(nullptr)
    , m_WVP(-1)
    , m_DiffTex(-1)
    , m_Center(-1)
    , m_Size(-1)
    , m_OrigSize(-1)
    , m_Depth(-1)
    , m_BlendColor(-1)
{
}


NinePatchEffect::~NinePatchEffect()
{
    delete m_Material;
}

void NinePatchEffect::init(const VertexLayout& layout)
{
    Material* mat(CONTENT->loadMaterial("engine/2D/simple.hm"));
    m_Material = mat->createMaterialInstance(eShaderType_Normal);
    m_Material->calculateMaterialLayout(layout);

    m_WVP = m_Material->findParameter(HEFS::strmatWVP);
    m_DiffTex = m_Material->findParameter(HEFS::strdiffuseMap);
    m_Center = m_Material->findParameter(HEFS::strcenter);
    m_Size = m_Material->findParameter(HEFS::strsize);
    m_OrigSize = m_Material->findParameter(HEFS::stroriginalSize);
    m_Depth = m_Material->findParameter(HEFS::strdepth);
    m_BlendColor = m_Material->findParameter(HEFS::strblendColor);

    setWorldMatrix(mat44::Identity);
}

void NinePatchEffect::apply(const DrawContext& context) const
{
    m_Material->apply(context);
}

void NinePatchEffect::setWorldMatrix(const he::mat44& mat) const
{
    m_Material->getParameter(m_WVP).setFloat44(mat);
}

void NinePatchEffect::setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const
{
    m_Material->getParameter(m_DiffTex).setTexture2D(diffuseMap);
}

void NinePatchEffect::setCenter(const RectF& center) const
{
    m_Material->getParameter(m_Center).setFloat4(vec4(center.x,center.y,center.width,center.height));
}

void NinePatchEffect::setSize(const vec2& size) const
{
    m_Material->getParameter(m_Size).setFloat2(size);
}

void NinePatchEffect::setOriginalSize(const vec2& origSize) const
{
    m_Material->getParameter(m_OrigSize).setFloat2(origSize);
}

void NinePatchEffect::setDepth(float depth) const
{
    m_Material->getParameter(m_Depth).setFloat(depth);
}

void NinePatchEffect::setBlendColor( const Color& color ) const
{
    m_Material->getParameter(m_BlendColor).setFloat4(color.rgba());
}

} } //end namespace
