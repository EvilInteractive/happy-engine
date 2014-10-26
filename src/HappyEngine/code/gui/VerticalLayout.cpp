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
//Created: 2014/10/20
#include "HappyPCH.h" 

#include "VerticalLayout.h"

namespace he {
namespace gui {

VerticalLayout::VerticalLayout()
{
}

VerticalLayout::~VerticalLayout()
{
}

void VerticalLayout::add( ILayoutable* obj )
{
    obj->initLayout(this);
    m_Objects.add(obj);
}

bool VerticalLayout::remove( ILayoutable* obj )
{
    return m_Objects.remove(obj);
}

void VerticalLayout::setLayoutDirty()
{
    DefaultLayoutable::setLayoutDirty();
}

void VerticalLayout::requestLayoutUpdate()
{
    setLayoutDirty();
}

void VerticalLayout::move( const he::vec2& move )
{
    DefaultLayoutable::move(move);
    m_Objects.forEach([&](ILayoutable* const obj)
    {
        obj->move(move);
    });
}

void VerticalLayout::performLayout()
{
    if (m_Objects.size() == 0)
        return;
    const float width(m_LayoutBound.width);
    const float height(m_LayoutBound.height);
    
    const float objMaxWidth(width - m_Margin.x - m_Margin.z);
    const float objPerfHeight((height - m_Margin.y - m_Margin.w) / m_Objects.size());

    float y(0.0f);
    const ELayoutVAlignment valignment(getLayoutVAlignment());
    switch (valignment)
    {
    case eLayoutVAlignment_Top: y = m_LayoutBound.y + m_Margin.y; break;
    case eLayoutVAlignment_Center: 
    case eLayoutVAlignment_Bottom: 
        float contentHeight(0.0f);
        m_Objects.forEach([&](ILayoutable* const obj)
        {
            contentHeight += he::clamp(objPerfHeight, obj->getLayoutMinSize().y, obj->getLayoutMaxSize().y);
            contentHeight += obj->getLayoutPadding().y + obj->getLayoutPadding().w;
        });
        if (valignment == eLayoutVAlignment_Center)
            y = m_LayoutBound.y + height / 2.0f - contentHeight / 2.0f;
        else
            y = m_LayoutBound.y + height - m_Margin.w - contentHeight; 
        break;
    }
    m_Objects.forEach([&](ILayoutable* const obj)
    {
        float x(m_LayoutBound.x);
        float objWidth(he::clamp(objMaxWidth, obj->getLayoutMinSize().x, obj->getLayoutMaxSize().x));
        float objHeight(he::clamp(objPerfHeight, obj->getLayoutMinSize().y, obj->getLayoutMaxSize().y));
        const he::vec4& objPadding(obj->getLayoutPadding());
        switch (obj->getLayoutHAlignment())
        {
        case eLayoutHAlignment_Left: x += m_Margin.x + objPadding.x; break;
        case eLayoutHAlignment_Center: x += m_Margin.x + objMaxWidth / 2.0f - objWidth / 2.0f; break;
        case eLayoutHAlignment_Right: x += width - m_Margin.z - objPadding.z - objWidth; break;
        }
        
        RectF layoutBound(x, y + objPadding.y, objWidth, objHeight);
        RectF clipBound(m_LayoutBound.x, y, width, objHeight + objPadding.y + objPadding.w);
        obj->setLayoutClipBound(clipBound);
        obj->setLayoutBound(layoutBound);
        obj->performLayout();

        y += objHeight + objPadding.y + objPadding.w;
    });
}

} } //end namespace
