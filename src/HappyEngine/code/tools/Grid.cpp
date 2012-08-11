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

#include "HappyPCH.h" 

#include "Grid.h"

#include "Vertex.h"
#include "BufferLayout.h"
#include "ShapeRenderer.h"

#include "ModelMesh.h"

namespace he {
namespace tools {

Grid::Grid(const vec3& pos, float size, float tileSize) :	m_Position(pos),
                                                            m_Color(Color(0.8f,0.8f,0.8f,1.0f)),
                                                            m_Color2(Color(1.0f,1.0f,1.0f,1.0f))
{
    gfx::BufferLayout layout;
    layout.addElement(gfx::BufferElement(0, gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Position, 12, 0));

    std::vector<gfx::VertexPos> vertices;
    std::vector<uint> indices;

    uint steps(static_cast<uint>(size / tileSize));

    uint index(0);
    for (uint i(0); i <= steps; ++i)
    {
        vertices.push_back(gfx::VertexPos(vec3(pos.x - (size/2), pos.y, pos.z - (size / 2) + (i * tileSize))));
        vertices.push_back(gfx::VertexPos(vec3(pos.x + (size/2), pos.y, pos.z - (size / 2) + (i * tileSize))));

        indices.push_back(index++);
        indices.push_back(index++);

        vertices.push_back(gfx::VertexPos(vec3(pos.x - (size / 2) + (i * tileSize), pos.y, pos.z - (size / 2))));
        vertices.push_back(gfx::VertexPos(vec3(pos.x - (size / 2) + (i * tileSize), pos.y, pos.z + (size / 2))));

        indices.push_back(index++);
        indices.push_back(index++);
    }

    m_pModelMesh = ResourceFactory<gfx::ModelMesh>::getInstance()->get(ResourceFactory<gfx::ModelMesh>::getInstance()->create());
    m_pModelMesh->setName("Grid mesh 1");
    m_pModelMesh->init();
    m_pModelMesh->setVertices(&vertices[0], (uint)vertices.size(), layout);
    m_pModelMesh->setIndices(&indices[0], (uint)indices.size(), gfx::IndexStride_UInt);

    std::vector<gfx::VertexPos> vertices2;
    std::vector<uint> indices2;

    index = 0;
    vertices2.push_back(gfx::VertexPos(vec3(pos.x - (size/2), pos.y, pos.z - (size / 2) + (steps/2 * tileSize))));
    vertices2.push_back(gfx::VertexPos(vec3(pos.x + (size/2), pos.y, pos.z - (size / 2) + (steps/2 * tileSize))));

    indices2.push_back(index++);
    indices2.push_back(index++);

    vertices2.push_back(gfx::VertexPos(vec3(pos.x - (size / 2) + (steps/2 * tileSize), pos.y, pos.z - (size / 2))));
    vertices2.push_back(gfx::VertexPos(vec3(pos.x - (size / 2) + (steps/2 * tileSize), pos.y, pos.z + (size / 2))));

    indices2.push_back(index++);
    indices2.push_back(index++);

    m_pModelMesh2 = ResourceFactory<gfx::ModelMesh>::getInstance()->get(ResourceFactory<gfx::ModelMesh>::getInstance()->create());
    m_pModelMesh2->setName("Grid mesh 2");
    m_pModelMesh2->init();
    m_pModelMesh2->setVertices(&vertices2[0], (uint)vertices2.size(), layout);
    m_pModelMesh2->setIndices(&indices2[0], (uint)indices2.size(), gfx::IndexStride_UInt);
}

Grid::~Grid()
{
    m_pModelMesh->release();
    m_pModelMesh2->release();
}

/* GENERAL */
void Grid::drawShapes(gfx::ShapeRenderer* renderer)
{
    mat44 world(mat44::createTranslation(m_Position));

    renderer->drawSpline(m_pModelMesh, world, m_Color);
    renderer->drawSpline(m_pModelMesh2, world, m_Color2);
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