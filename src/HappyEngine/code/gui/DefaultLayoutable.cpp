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
//Created: 2014/10/19
#include "HappyPCH.h"
#include "DefaultLayoutable.h"

#include "ILayoutContainer.h"

namespace he {
namespace gui {

enum ELayoutFlags
{
    eLayoutFlags_LayoutSuspended = 1 << 0,
    eLayoutFlags_LayoutDirty = 1 << 1
};

DefaultLayoutable::DefaultLayoutable()
    : m_LayoutBound(0, 0, 0, 0)
    , m_LayoutClipBound(0, 0, 1000000, 1000000)
    , m_LayoutFlags(0)
    , m_HAlignment(eLayoutHAlignment_Left)
    , m_VAlignment(eLayoutVAlignment_Top)
    , m_Padding(0, 0, 0, 0)
    , m_MinSize(0, 0)
    , m_MaxSize(1000000, 1000000)
    , m_LayoutParent(nullptr)
{

}

DefaultLayoutable::~DefaultLayoutable()
{

}

void DefaultLayoutable::initLayout( ILayoutContainer* const container )
{
    m_LayoutParent = container;
}

void DefaultLayoutable::setLayoutHAlignment( const ELayoutHAlignment halign )
{
    m_HAlignment = halign;
    setLayoutDirty();
}

void DefaultLayoutable::setLayoutVAlignment( const ELayoutVAlignment valign )
{
    m_VAlignment = valign;
    setLayoutDirty();
}

void DefaultLayoutable::setLayoutPadding( const he::vec4& padding )
{
    m_Padding = padding;
    setLayoutDirty();
}

void DefaultLayoutable::setLayoutMinSize( const he::vec2& minSize )
{
    m_MinSize = minSize;
    setLayoutDirty();
}

void DefaultLayoutable::setLayoutMaxSize( const he::vec2& maxSize )
{
    m_MaxSize = maxSize;
    setLayoutDirty();
}

bool DefaultLayoutable::isLayoutSuspended() const 
{
    return (m_LayoutFlags & eLayoutFlags_LayoutSuspended) != 0;
}

void DefaultLayoutable::suspendLayout()
{
    m_LayoutFlags |= eLayoutFlags_LayoutSuspended;
}

void DefaultLayoutable::resumeLayout()
{
    m_LayoutFlags &= ~eLayoutFlags_LayoutSuspended;
    if (m_LayoutFlags & eLayoutFlags_LayoutDirty)
        performLayout();
}

void DefaultLayoutable::setLayoutBound( const he::RectF& bound )
{
    m_LayoutBound = bound;
}

void DefaultLayoutable::setLayoutDirty()
{
    m_LayoutFlags |= eLayoutFlags_LayoutDirty;
    if (m_LayoutParent)
    {
        m_LayoutParent->requestLayoutUpdate();
    }
    if (m_LayoutFlags & eLayoutFlags_LayoutDirty)
    {
        if ((m_LayoutFlags & eLayoutFlags_LayoutSuspended) == 0)
            performLayout();
    }
}

void DefaultLayoutable::performLayout()
{
    m_LayoutFlags &= ~eLayoutFlags_LayoutDirty;
}

void DefaultLayoutable::setLayoutClipBound( const he::RectF& bound )
{
    m_LayoutClipBound = bound;
}

void DefaultLayoutable::move( const he::vec2& move )
{
    m_LayoutClipBound.move(move);
    m_LayoutBound.move(move);
}

} } //end namespace
