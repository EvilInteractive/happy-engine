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
#include "BezierEffect.h"

#include "ContentManager.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialParameter.h"

namespace he {
namespace gfx {

BezierEffect::BezierEffect() 
    : m_Material(nullptr)
    , m_WVP(-1)
    , m_Color(-1)
    , m_Radius(-1)
    , m_BeginEnd(-1)
    , m_Tangents(-1)
{ 
}


BezierEffect::~BezierEffect()
{
    delete m_Material;
}

void BezierEffect::init(const VertexLayout& layout)
{
    Material* mat(CONTENT->loadMaterial("engine/2D/bezier.hm"));
    m_Material = mat->createMaterialInstance(eShaderRenderType_Normal);
    mat->release();
    m_Material->calculateMaterialLayout(layout);

    m_WVP = m_Material->findParameter(HEFS::strmatWVP);
    m_Color = m_Material->findParameter(HEFS::strcolor);
    m_Radius = m_Material->findParameter(HEFS::strradius);
    m_BeginEnd = m_Material->findParameter(HEFS::strbeginEndPos);
    m_Tangents = m_Material->findParameter(HEFS::strtangents);

    setWVPMatrix(mat44::Identity);
    setColor(Color(1.0f,1.0f,1.0f,1.0f));
    setRadius(4.0f);
    setCurve(vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));
}

void BezierEffect::apply(const DrawContext& context) const
{
    m_Material->apply(context);
}

void BezierEffect::setColor(const Color& color) const
{
    m_Material->getParameter(m_Color).setFloat4(color.rgba());
}

void BezierEffect::setWVPMatrix(const he::mat44 &mat) const
{
    m_Material->getParameter(m_WVP).setFloat44(mat);
}

void BezierEffect::setRadius(float radius) const
{
    m_Material->getParameter(m_Radius).setFloat(radius);
}


void BezierEffect::setCurve( const he::vec2& begin, const he::vec2& tangent1, const he::vec2& tangent2, const he::vec2& endPos ) const
{
    m_Material->getParameter(m_BeginEnd).setFloat4(vec4(begin, endPos));
    m_Material->getParameter(m_Tangents).setFloat4(vec4(tangent1, tangent2));
}

} } //end namespace
