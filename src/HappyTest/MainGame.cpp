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

namespace happytest {

MainGame::MainGame() : m_pShader(0), m_pSimpleForward3DRenderer(0)
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
    vertices.push_back(VertexPosCol(math::Vector3(1.0f, -1.0f, 0.0f), math::Vector3(1.0f, 0.0f, 0.0f)));
    vertices.push_back(VertexPosCol(math::Vector3(-1.0f, -1.0f, 0.0f), math::Vector3(0.0f, 1.0f, 1.0f)));
    vertices.push_back(VertexPosCol(math::Vector3(-1.0f, 1.0f, 0.0f), math::Vector3(0.0f, 0.0f, 1.0f)));

    vertices.push_back(VertexPosCol(math::Vector3(1.0f, -1.0f, 0.0f), math::Vector3(1.0f, 0.0f, 1.0f)));
    vertices.push_back(VertexPosCol(math::Vector3(1.0f, 1.0f, 0.0f), math::Vector3(0.0f, 1.0f, 1.0f)));
    vertices.push_back(VertexPosCol(math::Vector3(-1.0f, 1.0f, 0.0f), math::Vector3(1.0f, 1.0f, 0.0f)));

    VertexLayout layout;
    layout.addElement(VertexElement(0, VertexElement::Type_Vector3, sizeof(math::Vector3), 0, "inPosition"));
    layout.addElement(VertexElement(1, VertexElement::Type_Vector3, sizeof(math::Vector3), sizeof(math::Vector3), "inColor"));

    m_pModel->setVertices(vertices.data(), 6, layout);

    m_pShader = new Shader();
    m_pShader->init("../data/shaders/simple2DShader.vert", "../data/shaders/simple2DShader.frag", layout);

    m_pSimpleForward3DRenderer = new SimpleForward3DRenderer();
}
void MainGame::tick(float /*dTime*/)
{
}
void MainGame::draw(float /*dTime*/)
{
    m_pShader->begin();

    m_pSimpleForward3DRenderer->draw(m_pModel);

    m_pShader->end();
}

} //end namespace