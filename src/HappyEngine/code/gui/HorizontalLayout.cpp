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
//Created: 2014/10/23
#include "HappyPCH.h" 
#include "HorizontalLayout.h"

namespace he {
namespace gui {

HorizontalLayout::HorizontalLayout()
{
}

HorizontalLayout::~HorizontalLayout()
{
}

void HorizontalLayout::add( ILayoutable* obj )
{
    obj->initLayout(this);
    m_Objects.add(obj);
}

bool HorizontalLayout::remove( ILayoutable* obj )
{
    return m_Objects.remove(obj);
}

void HorizontalLayout::setLayoutDirty()
{
    DefaultLayoutable::setLayoutDirty();
}

void HorizontalLayout::requestLayoutUpdate()
{
    setLayoutDirty();
}

void HorizontalLayout::move( const he::vec2& move )
{
    DefaultLayoutable::move(move);
    m_Objects.forEach([&](ILayoutable* const obj)
    {
        obj->move(move);
    });
}

void HorizontalLayout::performLayout()
{
    if (m_Objects.size() == 0)
        return;

    const float width(m_LayoutBound.width);
    const float height(m_LayoutBound.height);

    const float objMaxHeight(height - m_Margin.y - m_Margin.w);
    const float objPerfWidth((width - m_Margin.x - m_Margin.z) / m_Objects.size());

    float x(0.0f);
    const ELayoutHAlignment halignment(getLayoutHAlignment());
    switch (halignment)
    {
    case eLayoutVAlignment_Top: x = m_LayoutBound.x + m_Margin.x; break;
    case eLayoutVAlignment_Center: 
    case eLayoutVAlignment_Bottom: 
        float contentWidth(0.0f);
        m_Objects.forEach([&](ILayoutable* const obj)
        {
            contentWidth += he::clamp(objPerfWidth, obj->getLayoutMinSize().x, obj->getLayoutMaxSize().x);
            contentWidth += obj->getLayoutPadding().x + obj->getLayoutPadding().z;
        });
        if (halignment == eLayoutVAlignment_Center)
            x = m_LayoutBound.x + width / 2.0f - contentWidth / 2.0f;
        else
            x = m_LayoutBound.x + width - m_Margin.z - contentWidth; 
        break;
    }
    m_Objects.forEach([&](ILayoutable* const obj)
    {
        float y(m_LayoutBound.y);
        float objWidth(he::clamp(objPerfWidth, obj->getLayoutMinSize().x, obj->getLayoutMaxSize().x));
        float objHeight(he::clamp(objMaxHeight, obj->getLayoutMinSize().y, obj->getLayoutMaxSize().y));
        const he::vec4& objPadding(obj->getLayoutPadding());
        switch (obj->getLayoutVAlignment())
        {
        case eLayoutVAlignment_Top: y += m_Margin.y + objPadding.y; break;
        case eLayoutVAlignment_Center: y += m_Margin.y + objMaxHeight / 2.0f - objHeight / 2.0f; break;
        case eLayoutVAlignment_Bottom: y += height - m_Margin.w - objPadding.w - objHeight; break;
        }
        
        RectF layoutBound(x + objPadding.x, y, objWidth, objHeight);
        RectF clipBound(x, m_LayoutBound.y, objWidth + objPadding.x + objPadding.z, height);
        obj->setLayoutClipBound(clipBound);
        obj->setLayoutBound(layoutBound);
        obj->performLayout();

        x += objWidth + objPadding.x + objPadding.z;
    });
}

} } //end namespace
