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

#ifndef _HE_HorizontalLayout_H_
#define _HE_HorizontalLayout_H_
#pragma once

#include <DefaultLayoutable.h>
#include <DefaultLayoutContainer.h>

namespace he {
namespace gui {

class HAPPY_ENTRY HorizontalLayout : public DefaultLayoutContainer, public DefaultLayoutable
{
public:
    HorizontalLayout();
    ~HorizontalLayout();

    void add(ILayoutable* obj);
    bool remove(ILayoutable* obj);
    
    // ILayoutContainer
    void requestLayoutUpdate() override;

    // ILayoutable
    void performLayout() override;
    void move(const he::vec2& move) override;

protected:
    void setLayoutDirty() override;

private:
    he::PrimitiveList<ILayoutable*> m_Objects;

    //Disable default copy constructor and default assignment operator
    HorizontalLayout(const HorizontalLayout&);
    HorizontalLayout& operator=(const HorizontalLayout&);
};

} } //end namespace

#endif
