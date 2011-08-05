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
#include <sstream>

#include "Vertex.h"
#include "Matrix.h"
#include "Vector3.h"
#include "ObjLoader.h"
#include "MathConstants.h"
#include "HappyEngine.h"

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

    content::models::ObjLoader loader;
    loader.load("../data/models/testModelComplex1.obj");

    VertexLayout layout;
    layout.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, sizeof(math::Vector3), 0, "inPosition"));
    layout.addElement(VertexElement(1, VertexElement::Type_Vector3, VertexElement::Usage_Normal, sizeof(math::Vector3), sizeof(math::Vector3), "inColor"));
    std::vector<VertexPosCol> vertices = loader.getVertices<VertexPosCol>(layout);

    m_pModel->setVertices(vertices.data(), vertices.size(), layout);
    m_pModel->setIndices(loader.getIndices(), loader.getNumIndices(), loader.getIndexType());

    m_pShader = new Shader();
    m_pShader->init("../data/shaders/simpleShader.vert", "../data/shaders/simpleShader.frag", layout);

    m_pSimpleForward3DRenderer = new SimpleForward3DRenderer();
}
void MainGame::tick(float /*dTime*/)
{
}
void MainGame::draw(float dTime)
{
    happyengine::HappyEngine::getPointer()->getGraphicsEngine()->clearAll();
    m_Timer += dTime;

    using namespace happyengine;
    m_pShader->begin();
    math::Matrix persp(math::Matrix::createPerspectiveLH(math::piOverFour, 1280, 720, 1, 1000));
    math::Matrix view(math::Matrix::createLookAtLH(math::Vector3(-5, 5, -4), math::Vector3(0, 0, 0), math::Vector3(0, 1, 0)));
    math::Matrix world(
        math::Matrix::createTranslation(math::Vector3(0, -1, 0)) *
        math::Matrix::createRotation(math::Vector3(0.0f, 1.0f, 0.0f), m_Timer) *
        math::Matrix::createScale(2.0f)
                      );

    m_pShader->setShaderVar(m_pShader->getShaderVarId("matWVP"), persp * view * world);

    m_pSimpleForward3DRenderer->draw(m_pModel);

    m_pShader->end();
}

} //end namespace