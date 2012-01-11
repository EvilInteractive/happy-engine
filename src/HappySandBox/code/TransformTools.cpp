//HappySandBox Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappySandBox.
//
//    HappySandBox is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappySandBox is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappySandBox.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Sebastiaan Sprengers
//Created: 11/12/2011

#include "TransformTools.h"
#include "HappyPCH.h"
#include "ContentManager.h"

namespace happysandbox {

/* CONSTRUCTOR - DESTRUCTOR */
TransformTools::TransformTools() :	m_Mode(MODE_OFF)
{
    m_BufferLayout.addElement(he::gfx::BufferElement(0, he::gfx::BufferElement::Type_Vec3, he::gfx::BufferElement::Usage_Position, 12, 0));

    m_pMoveAxis = CONTENT->asyncLoadModelMesh("editor/transform_move_axis.binobj", "transform_move_axis", m_BufferLayout);
}

TransformTools::~TransformTools()
{
}

/* GENERAL */
void TransformTools::tick()
{
}

void TransformTools::draw()
{
}

void TransformTools::setMode(MODE mode)
{
    if (m_Mode != mode)
        m_Mode = mode;
}

} //end namespace