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
#include "HappyPCH.h" 

#include "Hitregion.h"
#include "HappyNew.h"

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Hitregion::Hitregion(TYPE hitregionType, const vec2& centerPos, const vec2& size) :	m_Type(hitregionType),
																										m_Size(size),
																										m_Pos(centerPos)
{
	m_matWorld = mat33::createTranslaton(centerPos);
}

Hitregion::~Hitregion()
{
}

/* DEFAULT COPY & ASSIGNMENT OPERATOR */
Hitregion::Hitregion(const Hitregion& second)
{
	m_Type = second.m_Type;
	m_Size = second.m_Size;
	m_matWorld = second.m_matWorld;
}

Hitregion& Hitregion::operator=(const Hitregion& second)
{
	m_Type = second.m_Type;
	m_Size = second.m_Size;
	m_matWorld = second.m_matWorld;

	return *this;
}

/* GETTERS */
bool Hitregion::hitTest(const Hitregion* pHitrect) const
{
	if (m_Type == TYPE_RECTANGLE && pHitrect->getType() == TYPE_RECTANGLE)
	{
		//if (
	}

	return false;
}

bool Hitregion::hitTest(const vec2& point) const
{
	if (m_Type == TYPE_RECTANGLE)
	{
		if (point.x < m_Pos.x + m_Size.x / 2 &&
			point.x > m_Pos.x - m_Size.x / 2 &&
			point.y < m_Pos.y + m_Size.y / 2 &&
			point.y > m_Pos.y - m_Size.y / 2)
		{
			return true;
		}
	}

	return false;
}

vec2 Hitregion::getSize() const
{
	return m_Size;
}

vec2 Hitregion::getPosition() const
{
	return m_Pos;
}

Hitregion::TYPE Hitregion::getType() const
{
	return m_Type;
}

/* SETTERS */
void Hitregion::setPosition(const vec2& pos)
{
	m_Pos = pos;
}

void Hitregion::setSize(const vec2& size)
{
	m_Size = size;
}

void Hitregion::move(const vec2& translation)
{
	setPosition(getPosition() + translation);
}

void Hitregion::setTransformationMatrix(const mat33& mat)
{
	m_matWorld = mat;
}

} } //end namespace