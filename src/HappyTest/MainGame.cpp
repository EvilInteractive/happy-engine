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

#include "MainGame.h"

#include <vector>
#include "Vertex.h"
#include "Matrix.h"
#include "Vector3.h"

namespace happytest {

MainGame::MainGame() : m_pShader(0), m_pSimpleForward3DRenderer(0), m_Timer(0.0f)
{
}


MainGame::~MainGame()
{
    delete m_pShader;
    delete m_pSimpleForward3DRenderer;
}

void MainGame::init()
{
}
void MainGame::load()
{
    using namespace happyengine::graphics;
    using namespace happyengine;
    m_pModel = Model::pointer(new Model());

    std::vector<VertexPosCol> vertices;
    vertices.push_back(VertexPosCol(math::Vector3(64.0f, -64.0f, 0.0f), math::Vector3(1.0f, 0.0f, 0.0f)));
    vertices.push_back(VertexPosCol(math::Vector3(-64.0f, -64.0f, 0.0f), math::Vector3(0.0f, 1.0f, 1.0f)));
    vertices.push_back(VertexPosCol(math::Vector3(-64.0f, 64.0f, 0.0f), math::Vector3(0.0f, 0.0f, 1.0f)));

    vertices.push_back(VertexPosCol(math::Vector3(64.0f, -64.0f, 0.0f), math::Vector3(1.0f, 0.0f, 1.0f)));
    vertices.push_back(VertexPosCol(math::Vector3(64.0f, 64.0f, 0.0f), math::Vector3(0.0f, 1.0f, 1.0f)));
    vertices.push_back(VertexPosCol(math::Vector3(-64.0f, 64.0f, 0.0f), math::Vector3(1.0f, 1.0f, 0.0f)));

    VertexLayout layout;
    layout.addElement(VertexElement(0, VertexElement::Type_Vector3, sizeof(math::Vector3), 0, "inPosition"));
    layout.addElement(VertexElement(1, VertexElement::Type_Vector3, sizeof(math::Vector3), sizeof(math::Vector3), "inColor"));

    m_pModel->setVertices(vertices.data(), 6, layout);

    m_pShader = new Shader();
    m_pShader->init("../data/shaders/simpleShader.vert", "../data/shaders/simpleShader.frag", layout);

    m_pSimpleForward3DRenderer = new SimpleForward3DRenderer();
}
void MainGame::tick(float /*dTime*/)
{
}
void MainGame::draw(float dTime)
{
    m_Timer += dTime;

    using namespace happyengine;
    m_pShader->begin();
    math::Matrix persp(math::Matrix::createOrthoLH(0, 1280, 0, 720, 0, 1));
    math::Matrix world(
        math::Matrix::createTranslation(math::Vector3(256, 256, 0)) *
        math::Matrix::createRotation(math::Vector3(0, 0, 1), m_Timer) *
        math::Matrix::createScale(cosf(m_Timer/2.0f) + 1.0f)
                      );

    m_pShader->setShaderVar(m_pShader->getShaderVarId("matWVP"), persp * world);

    m_pSimpleForward3DRenderer->draw(m_pModel);

    m_pShader->end();
}

} //end namespace