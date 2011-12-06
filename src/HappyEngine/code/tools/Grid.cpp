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
//Created: 06/11/2011

#include "HappyPCH.h" 

#include "Grid.h"
#include "HappyNew.h"
#include "Vertex.h"
#include <vector>
#include "VertexLayout.h"
#include "SimpleForward3DRenderer.h"

namespace he {
namespace tools {

Grid::Grid(const vec3& pos, float size, float tileSize) :	m_Position(pos),
															m_Color(Color(1.0f,1.0f,1.0f,1.0f))
{
	gfx::VertexLayout layout;
	layout.addElement(gfx::VertexElement(0, gfx::VertexElement::Type_Vec3, gfx::VertexElement::Usage_Position, 12, 0));

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

	m_pModelMesh = gfx::ModelMesh::pointer(NEW gfx::ModelMesh(""));
	m_pModelMesh->init();
	m_pModelMesh->setVertices(&vertices[0], vertices.size(), layout);
	m_pModelMesh->setIndices(&indices[0], indices.size(), gfx::IndexStride_UInt);
}

Grid::~Grid()
{
}

/* GENERAL */
void Grid::draw()
{
	mat44 world(mat44::createTranslation(m_Position));

	HE3D->drawSpline(m_pModelMesh, world, m_Color);
}

/* SETTERS */
void Grid::setPosition(const vec3& pos)
{
	m_Position = pos;
}

void Grid::setColor(const Color& color)
{
	m_Color = color;
}

} } //end namespace