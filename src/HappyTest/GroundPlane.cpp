//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.

#include "GroundPlane.h"
#include "HappyEngine.h"
#include "HappyNew.h"
#include "Vertex.h"
#include "ModelLoaderFunctions.h"
#include "ModelMesh.h"

namespace happytest {

GroundPlane::GroundPlane(const he::game::EntityDesc& desc): Entity(desc)
{
    using namespace he;
    int width = 1024, depth = 1024;
    float texRes = 4;
    
    std::vector<gfx::VertexPTNT> vertices;
    vertices.push_back(gfx::VertexPTNT(vec3(-width / 2.0f, 0.0f, -depth / 2.0f), vec2(0, 0), vec3(0, 1, 0), vec3()));
    vertices.push_back(gfx::VertexPTNT(vec3(width / 2.0f, 0.0f, -depth / 2.0f), vec2(width/texRes, 0), vec3(0, 1, 0), vec3()));
    vertices.push_back(gfx::VertexPTNT(vec3(-width / 2.0f, 0.0f, depth / 2.0f), vec2(0, depth/texRes), vec3(0, 1, 0), vec3()));
    vertices.push_back(gfx::VertexPTNT(vec3(width / 2.0f, 0.0f, depth / 2.0f), vec2(width/texRes, depth/texRes), vec3(0, 1, 0), vec3()));

    std::vector<byte> indices;
    indices.push_back(2); indices.push_back(1); indices.push_back(0);
    indices.push_back(2); indices.push_back(3); indices.push_back(1);

    std::vector<vec3> tan(he::ct::models::calculateTangents(&vertices[0], 4, 0, 12, 20, 44, &indices[0], 6, gfx::IndexStride_Byte));

    for (int i = 0; i < 4; ++i)
    {
        vertices[i].tangent = tan[i];
    }

    gfx::ModelMesh::pointer pModelMesh(NEW gfx::ModelMesh("groundPlane"));
    pModelMesh->init();
    pModelMesh->setVertices(&vertices[0], 4, getModel()->getVertexLayout());
    pModelMesh->setIndices(&indices[0], 6, gfx::IndexStride_Byte);
    
    getModel()->addMesh(pModelMesh);
}


GroundPlane::~GroundPlane()
{
}

} //end namespace