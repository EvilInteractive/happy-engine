//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Created: 10/09/2011

#ifndef _HE_HITREGION_H_
#define _HE_HITREGION_H_
#pragma once

namespace he {
namespace gui {

class Hitregion
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Hitregion(const vec2& centerPos, const vec2& size);
    virtual ~Hitregion();

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Hitregion(const Hitregion& second);
    Hitregion& operator=(const Hitregion& second);

    /* GETTERS */
    bool hitTest(const Hitregion* pHitrect) const;
    bool hitTest(const vec2& point) const;

    vec2 getSize() const;
    vec2 getPosition() const;

    /* SETTERS */
    void setPosition(const vec2& pos);
    void setSize(const vec2& size);
    void move(const vec2& translation);

private:

    /* DATAMEMBERS */
    vec2 m_Size;
    vec2 m_Pos;
};

} } //end namespace

#endif
