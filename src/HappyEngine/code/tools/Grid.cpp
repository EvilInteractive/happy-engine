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
//Author:  Sebastiaan Sprengers
//Created: 06/11/2011

#include "HappyPCH.h" 

#include "Grid.h"

#include "Vertex.h"
#include "VertexLayout.h"
#include "ShapeRenderer.h"

#include "ShapeMesh.h"

namespace he {
namespace tools {

Grid::Grid(const vec3& pos, float size, float tileSize) :	m_Position(pos),
                                                            m_Color(Color(0.8f,0.8f,0.8f,1.0f)),
                                                            m_Color2(Color(1.0f,1.0f,1.0f,1.0f))
{
    m_GridShape = HENew(gfx::ShapeMesh)();
    m_GridShape->init(gfx::MeshDrawMode_Lines);
    uint32 steps(static_cast<uint32>(size / tileSize));
    m_GridShape->beginEditing();
    for (uint32 i(0); i <= steps; ++i)
    {
        m_GridShape->addPoint(vec3(pos.x - (size/2), pos.y, pos.z - (size / 2) + (i * tileSize)), true);
        m_GridShape->addPoint(vec3(pos.x + (size/2), pos.y, pos.z - (size / 2) + (i * tileSize)), false);

        m_GridShape->addPoint(vec3(pos.x - (size / 2) + (i * tileSize), pos.y, pos.z - (size / 2)), true);
        m_GridShape->addPoint(vec3(pos.x - (size / 2) + (i * tileSize), pos.y, pos.z + (size / 2)), false);
    }
    m_GridShape->endEditing(false, false);

    m_BaseLineShape = HENew(gfx::ShapeMesh)();
    m_BaseLineShape->init(gfx::MeshDrawMode_Lines);
    m_BaseLineShape->beginEditing();

    m_BaseLineShape->addPoint(vec3(pos.x - (size/2), pos.y, pos.z - (size / 2) + (steps/2 * tileSize)), true);
    m_BaseLineShape->addPoint(vec3(pos.x + (size/2), pos.y, pos.z - (size / 2) + (steps/2 * tileSize)), false);

    m_BaseLineShape->addPoint(vec3(pos.x - (size / 2) + (steps/2 * tileSize), pos.y, pos.z - (size / 2)), true);
    m_BaseLineShape->addPoint(vec3(pos.x - (size / 2) + (steps/2 * tileSize), pos.y, pos.z + (size / 2)), false);

    m_BaseLineShape->endEditing(false, false);
}

Grid::~Grid()
{
    HEDelete(m_GridShape);
    HEDelete(m_BaseLineShape);
}

/* GENERAL */
void Grid::drawShapes(gfx::ShapeRenderer* const renderer)
{
    mat44 world(mat44::createTranslation(m_Position));

    renderer->drawShape(m_GridShape, world, m_Color);
    renderer->drawShape(m_BaseLineShape, world, m_Color2);
}

/* SETTERS */
void Grid::setPosition(const vec3& pos)
{
    m_Position = pos;
}

void Grid::setColor(const Color& color, const Color& color2)
{
    m_Color = color;
    m_Color2 = color2;
}

} } //end namespace