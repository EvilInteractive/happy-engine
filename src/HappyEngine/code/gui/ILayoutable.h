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
//Created: 2014/09/27

#ifndef _HE_ILayoutable_H_
#define _HE_ILayoutable_H_
#pragma once

#include "LayoutEnums.h"

namespace he {
namespace gui {
class ILayoutContainer;
class HAPPY_ENTRY ILayoutable
{
public:
    virtual ~ILayoutable() {}

    virtual void initLayout(ILayoutContainer* const container) = 0;

    virtual ELayoutHAlignment getLayoutHAlignment() const = 0;
    virtual ELayoutVAlignment getLayoutVAlignment() const = 0;

    virtual const he::vec4& getLayoutPadding() = 0;

    virtual const he::vec2 getLayoutMinSize() = 0;
    virtual const he::vec2 getLayoutMaxSize() = 0;

    virtual void suspendLayout() = 0;
    virtual void resumeLayout() = 0;

    virtual void setLayoutBound(const he::RectF& bound) = 0;
    virtual void setLayoutClipBound(const he::RectF& bound) = 0;
    virtual void performLayout() = 0;
};

} } //end namespace

#endif
