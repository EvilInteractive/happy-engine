//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include "Vector2.h"
#include "Matrix2D.h"

namespace happyengine {
namespace gui {

class Hitregion
{
public:

	enum TYPE
	{
		TYPE_RECTANGLE = 0,
		TYPE_ELLIPSE = 1,
		TYPE_CONVEX = 2
	};

	/* CONSTRUCTOR - DESTRUCTOR */
	Hitregion(TYPE hitregionType, const math::Vector2& centerPos, const math::Vector2& size);
    virtual ~Hitregion();

	/* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Hitregion(const Hitregion& second);
    Hitregion& operator=(const Hitregion& second);

	/* GETTERS */
	bool hitTest(const Hitregion* pHitrect) const;
	bool hitTest(const math::Vector2& point) const;

	math::Vector2 getSize() const;
	math::Vector2 getPosition() const;
	Hitregion::TYPE getType() const;

	/* SETTERS */
	void setPosition(const math::Vector2& pos);
	void setSize(const math::Vector2& size);
	void move(const math::Vector2& translation);
	void setTransformationMatrix(const math::Matrix2D& mat);

private:

	/* DATAMEMBERS */
	TYPE m_Type;

	math::Matrix2D m_matWorld;
	math::Vector2 m_Size;
	math::Vector2 m_Pos;
};

} } //end namespace

#endif
