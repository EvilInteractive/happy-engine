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

#ifndef _HE_DefaultLayoutContainer_H_
#define _HE_DefaultLayoutContainer_H_
#pragma once

#include <ILayoutContainer.h>

namespace he {
namespace gui {

class HAPPY_ENTRY DefaultLayoutContainer : public he::gui::ILayoutContainer
{
public:
    DefaultLayoutContainer();
    virtual ~DefaultLayoutContainer();

    virtual const he::vec4& getLayoutMargin() override { return m_Margin; }
    virtual void setLayoutMargin(const he::vec4& margin);

    virtual void requestLayoutUpdate();

protected:
    virtual void setLayoutDirty() = 0;

    vec4 m_Margin;
};

} } //end namespace

#endif
