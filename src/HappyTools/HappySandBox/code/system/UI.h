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
//Author:
//Created: //

#ifndef _HS_UI_H_
#define _HS_UI_H_
#pragma once

namespace hs {

class UI
{
public:
    UI();
    virtual ~UI();
    
    virtual void load() = 0;
    virtual void unload() = 0;

    virtual void show();
    virtual void hide();

    bool isVisible() const { return m_IsVisible; }

private:
    bool m_IsVisible;
    
    //Disable default copy constructor and default assignment operator
    UI(const UI&);
    UI& operator=(const UI&);
};

} //end namespace

#endif
