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
    virtual bool isLayoutSuspended() const = 0;

    virtual void setLayoutBound(const he::RectF& bound) = 0;
    virtual void setLayoutClipBound(const he::RectF& bound) = 0;
    virtual const he::RectF& getLayoutBound() const = 0;
    virtual const he::RectF& getLayoutClipBound() const = 0;
    virtual void performLayout() = 0;

    virtual void move(const he::vec2& move) = 0;
};

#define DECLARE_LAYOUT \
virtual void initLayout(he::gui::ILayoutContainer* const container) override;\
    \
virtual he::gui::ELayoutHAlignment getLayoutHAlignment() const override;\
virtual he::gui::ELayoutVAlignment getLayoutVAlignment() const override;\
    \
virtual const he::vec4& getLayoutPadding() override;\
    \
virtual const he::vec2 getLayoutMinSize() override;\
virtual const he::vec2 getLayoutMaxSize() override;\
    \
virtual void suspendLayout() override;\
virtual void resumeLayout() override;\
virtual bool isLayoutSuspended() const override;\
    \
virtual void setLayoutBound(const he::RectF& bound) override;\
virtual void setLayoutClipBound(const he::RectF& bound) override;\
virtual const he::RectF& getLayoutBound() const override; \
virtual const he::RectF& getLayoutClipBound() const override; \
virtual void performLayout() override;\
    \
virtual void move(const he::vec2& move) override;

#define IMPLEMENT_LAYOUT_FROM(className, member) \
void className::initLayout(he::gui::ILayoutContainer* const container) { member.initLayout(container); }\
\
he::gui::ELayoutHAlignment className::getLayoutHAlignment() const { return member.getLayoutHAlignment(); }\
he::gui::ELayoutVAlignment className::getLayoutVAlignment() const { return member.getLayoutVAlignment(); }\
\
const he::vec4& className::getLayoutPadding() { return member.getLayoutPadding(); }\
\
const he::vec2 className::getLayoutMinSize() { return member.getLayoutMinSize(); }\
const he::vec2 className::getLayoutMaxSize() { return member.getLayoutMaxSize(); }\
\
void className::suspendLayout() { member.suspendLayout(); }\
void className::resumeLayout() { member.resumeLayout(); }\
bool className::isLayoutSuspended() const { return member.isLayoutSuspended(); }\
\
void className::setLayoutBound(const he::RectF& bound) { member.setLayoutBound(bound); }\
void className::setLayoutClipBound(const he::RectF& bound) { member.setLayoutClipBound(bound); }\
const he::RectF& className::getLayoutBound() const { return member.getLayoutBound(); }\
const he::RectF& className::getLayoutClipBound()  const { return member.getLayoutClipBound(); }\
void className::performLayout() { member.performLayout(); } \
    \
void className::move(const he::vec2& move) { member.move(move); }

} } //end namespace

#endif
