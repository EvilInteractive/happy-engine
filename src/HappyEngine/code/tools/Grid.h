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
//Created: 06/11/2011

#ifndef _HE_GRID_H_
#define _HE_GRID_H_
#pragma once

#include "vec3.h"
#include "Entity.h"
#include "HappyEngine.h"
#include "Color.h"
#include "ModelMesh.h"

namespace he {
namespace tools {

class Grid : public he::game::Entity
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Grid(const vec3& pos, float size, float tileSize = 1.0f);
    virtual ~Grid();

    /* GENERAL */
    void draw();

    /* SETTERS */
    void setPosition(const vec3& pos);
    void setColor(const Color& color, const Color& color2);

private:

    /* DATAMEMBERS */
    vec3 m_Position;

    Color m_Color;
    Color m_Color2;

    gfx::ModelMesh::pointer m_pModelMesh;
    gfx::ModelMesh::pointer m_pModelMesh2;

    /* DEFAULT COPY & ASSIGNMENT */
    Grid(const Grid&);
    Grid& operator=(const Grid&);
};

} } //end namespace

#endif
