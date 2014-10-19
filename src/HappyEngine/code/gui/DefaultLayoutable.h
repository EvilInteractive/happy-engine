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

#ifndef _HE_DefaultLayoutable_H_
#define _HE_DefaultLayoutable_H_
#pragma once

#include <ILayoutable.h>

namespace he {
namespace gui {
class ILayoutContainer;
class HAPPY_ENTRY DefaultLayoutable : public he::gui::ILayoutable
{
public:
    DefaultLayoutable();
    virtual ~DefaultLayoutable();

    virtual void initLayout(ILayoutContainer* const container) override;

    virtual ELayoutHAlignment getLayoutHAlignment() const override { return m_HAlignment; }
    virtual void setLayoutHAlignment(const ELayoutHAlignment halign);
    virtual ELayoutVAlignment getLayoutVAlignment() const override { return m_VAlignment; } 
    virtual void setLayoutVAlignment(const ELayoutVAlignment valign);

    virtual const he::vec4& getLayoutPadding() override { return m_Padding; }
    virtual void setLayoutPadding(const he::vec4& padding);

    virtual const he::vec2 getLayoutMinSize() override { return m_MinSize; }
    virtual void setLayoutMinSize(const he::vec2& minSize);
    virtual const he::vec2 getLayoutMaxSize() override { return m_MaxSize; }
    virtual void setLayoutMaxSize(const he::vec2& maxSize);

    virtual void suspendLayout() override;
    virtual void resumeLayout() override;

    virtual void setLayoutBound(const he::RectF& bound) override;
    virtual const he::RectF& getLayoutBound() const { return m_LayoutBound; }

    virtual void setLayoutClipBound(const he::RectF& bound) override;
    virtual const he::RectF& getLayoutClipBound() const { return m_LayoutClipBound; }

    virtual void performLayout() override;

protected:
    virtual void setLayoutDirty();

    RectF m_LayoutBound;
    RectF m_LayoutClipBound;

    uint8 m_LayoutFlags;
    ELayoutHAlignment m_HAlignment : 4;
    ELayoutVAlignment m_VAlignment : 4;

    vec4 m_Padding;
    vec2 m_MinSize;
    vec2 m_MaxSize;

    ILayoutContainer* m_LayoutParent;
};

} } //end namespace

#endif
