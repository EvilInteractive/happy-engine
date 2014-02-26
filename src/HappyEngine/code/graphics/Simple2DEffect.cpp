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
#include "Simple2DEffect.h"

#include "ContentManager.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialParameter.h"

namespace he {
namespace gfx {

Simple2DEffect::Simple2DEffect() 
    : m_Material(nullptr)
    , m_WVP(-1)
    , m_Color(-1)
    , m_Depth(-1)
{ 
}


Simple2DEffect::~Simple2DEffect()
{
    delete m_Material;
}

void Simple2DEffect::init(const BufferLayout& layout)
{
    Material* mat(CONTENT->loadMaterial("engine/2D/simple.hm"));
    m_Material = mat->createMaterialInstance(eShaderType_Normal);
    m_Material->calculateMaterialLayout(layout);

    m_WVP = m_Material->findParameter(HEFS::strmatWVP);
    m_Color = m_Material->findParameter(HEFS::strcolor);
    m_Depth = m_Material->findParameter(HEFS::strdepth);

    setWorldMatrix(mat44::Identity);
    setColor(Color(1.0f,1.0f,1.0f,1.0f));
    setDepth(0.0f);
}

void Simple2DEffect::begin(const DrawContext& context) const
{
    m_Material->apply(context);
}

void Simple2DEffect::end() const
{
}

void Simple2DEffect::setColor(const Color& color) const
{
    m_Material->getParameter(m_Color).setFloat4(color.rgba());
}

void Simple2DEffect::setWorldMatrix(const he::mat44 &mat) const
{
    m_Material->getParameter(m_WVP).setFloat44(mat);
}

void Simple2DEffect::setDepth(float depth) const
{
    m_Material->getParameter(m_Depth).setFloat(depth);
}

} } //end namespace
