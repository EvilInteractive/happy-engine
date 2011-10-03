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

#include "Hitregion.h"
#include "HappyNew.h"

namespace happyengine {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Hitregion::Hitregion(TYPE hitregionType, const math::Vector2& centerPos, const math::Vector2& size) :	m_Type(hitregionType),
																										m_Size(size),
																										m_Pos(centerPos)
{
	m_matWorld = math::Matrix2D::createTranslaton(centerPos);
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

bool Hitregion::hitTest(const math::Vector2& point) const
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

math::Vector2 Hitregion::getSize() const
{
	return m_Size;
}

math::Vector2 Hitregion::getPosition() const
{
	return m_matWorld.getTranslation();
}

Hitregion::TYPE Hitregion::getType() const
{
	return m_Type;
}

/* SETTERS */
void Hitregion::setPosition(const math::Vector2& pos)
{
	m_Pos = pos;
}

void Hitregion::setSize(const math::Vector2& size)
{
	m_Size = size;
}

void Hitregion::move(const math::Vector2& translation)
{
	setPosition(getPosition() + translation);
}

void Hitregion::setTransformationMatrix(const math::Matrix2D& mat)
{
	m_matWorld = mat;
}

} } //end namespace