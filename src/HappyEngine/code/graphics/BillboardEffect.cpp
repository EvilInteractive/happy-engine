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
//Created: 26/08/2011
#include "HappyPCH.h" 
#include "BillboardEffect.h"

#include "ContentManager.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialParameter.h"

namespace he {
namespace gfx {

BillboardEffect::BillboardEffect()
    : m_Material(nullptr)
    , m_WVP(-1)
    , m_DiffTex(-1)
    , m_TcScale(-1)
{
}


BillboardEffect::~BillboardEffect()
{
    delete m_Material;
}

void BillboardEffect::init(const BufferLayout& layout)
{
    Material* mat(CONTENT->loadMaterial("engine/2D/billboard.hm"));
    m_Material = mat->createMaterialInstance(eShaderType_Normal);
    m_Material->calculateMaterialLayout(layout);

    m_WVP = m_Material->findParameter(HEFS::strmatWVP);
    m_DiffTex = m_Material->findParameter(HEFS::strdiffuseMap);
    m_TcScale = m_Material->findParameter(HEFS::strtexCoordScale);

    setWorldViewProjection(mat44::Identity);
    setTCScale(vec2(1.0f,1.0f));
}

void BillboardEffect::begin(const DrawContext& context) const
{
    m_Material->apply(context);
}

void BillboardEffect::end() const
{
}

void BillboardEffect::setWorldViewProjection(const he::mat44& mat) const
{
    m_Material->getParameter(m_WVP).setFloat44(mat);
}

void BillboardEffect::setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const
{
    m_Material->getParameter(m_DiffTex).setTexture2D(diffuseMap);
}

void BillboardEffect::setTCScale(const vec2& scale) const
{
    m_Material->getParameter(m_TcScale).setFloat2(scale);
}

} } //end namespace
