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

namespace happytest {

GroundPlane::GroundPlane(): m_pModel(NEW happyengine::graphics::ModelMesh(""))
{
}


GroundPlane::~GroundPlane()
{
}

void GroundPlane::load()
{
    m_pDiffuseMap = CONTENT->asyncLoadTexture("../data/textures/gravel_diffuse.png");
    m_pNormalMap = CONTENT->asyncLoadTexture("../data/textures/gravel_normal.png");
    m_pSGIMap = CONTENT->asyncLoadTexture("../data/textures/gravel_sgi.png");

    using namespace happyengine;
    int width = 1024, depth = 1024;
    float texRes = 4;

    std::vector<graphics::VertexPTNT> vertices;
    vertices.push_back(graphics::VertexPTNT(math::Vector3(-width / 2.0f, 0.0f, -depth / 2.0f), math::Vector2(0, 0), math::Vector3(0, 1, 0), math::Vector3()));
    vertices.push_back(graphics::VertexPTNT(math::Vector3(width / 2.0f, 0.0f, -depth / 2.0f), math::Vector2(width/texRes, 0), math::Vector3(0, 1, 0), math::Vector3()));
    vertices.push_back(graphics::VertexPTNT(math::Vector3(-width / 2.0f, 0.0f, depth / 2.0f), math::Vector2(0, depth/texRes), math::Vector3(0, 1, 0), math::Vector3()));
    vertices.push_back(graphics::VertexPTNT(math::Vector3(width / 2.0f, 0.0f, depth / 2.0f), math::Vector2(width/texRes, depth/texRes), math::Vector3(0, 1, 0), math::Vector3()));

    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(1); indices.push_back(3); indices.push_back(2);

    std::vector<math::Vector3> tan(happyengine::content::models::calculateTangents(&vertices[0], 4, 0, 12, 20, 44, &indices[0], 6, graphics::IndexStride_Byte));

    for (int i = 0; i < 4; ++i)
    {
        vertices[i].tangent = tan[i];
    }

    m_pModel->init();
    m_pModel->setVertices(&vertices[0], 4, DeferredPreEffect::getVertexLayout());
    m_pModel->setIndices(&indices[0], 6, graphics::IndexStride_Byte);
}
void GroundPlane::draw(happyengine::graphics::I3DRenderer* pRenderer, DeferredPreEffect* m_pEffect, const happyengine::graphics::Camera* pCamera)
{
    m_pEffect->setDiffuseMap(m_pDiffuseMap);
    m_pEffect->setNormalMap(m_pNormalMap);
    m_pEffect->setSpecGlossIllMap(m_pSGIMap);
    m_pEffect->setWorld(happyengine::math::Matrix::Identity);
    m_pEffect->setWVP(pCamera->getViewProjection());

    pRenderer->draw(m_pModel);
}

} //end namespace