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

#include "TestObject.h"
#include "ObjLoader.h"
#include "MathConstants.h"
#include "HappyEngine.h"
#include "VertexPNT.h"
#include "TextureLoader.h"
#include "FontLoader.h"

#include <sstream>

namespace happytest {

TestObject::TestObject(): m_pShader(nullptr), m_Rotation(0), m_Position(0, 0, 0)
{
}


TestObject::~TestObject()
{
    delete m_pShader;
}


void TestObject::load()
{
    using namespace happyengine::graphics;
    using namespace happyengine;
    m_pModel = Model::pointer(new Model());

    content::models::ObjLoader loader;
    loader.load("../data/models/testModelComplex1.obj");
    //loader.load("../data/models/cube.obj");

    VertexLayout layout;
    layout.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, sizeof(math::Vector3), 0, "inPosition"));
    layout.addElement(VertexElement(1, VertexElement::Type_Vector2, VertexElement::Usage_TextureCoordinate, sizeof(math::Vector2), 12, "inTexCoord"));
    layout.addElement(VertexElement(2, VertexElement::Type_Vector3, VertexElement::Usage_Normal, sizeof(math::Vector3), 20, "inNormal"));
    std::vector<VertexPNT> vertices = loader.getVertices<VertexPNT>(layout);

    m_pModel->setVertices(vertices.data(), vertices.size(), layout);
    m_pModel->setIndices(loader.getIndices(), loader.getNumIndices(), loader.getIndexType());

    m_pShader = new Shader();
    std::vector<std::string> shaderOutputs;
    shaderOutputs.push_back("outColorIll");
    shaderOutputs.push_back("outPosSpec");
    shaderOutputs.push_back("outNormalGloss");
    m_pShader->init("../data/shaders/deferredPreShader.vert", "../data/shaders/deferredPreShader.frag", layout, shaderOutputs);


    happyengine::content::TextureLoader texLoader;
    texLoader.load("../data/textures/testTex.png", m_pDiffuseMap);

    m_ShaderWVPpos = m_pShader->getShaderVarId("matWVP");
    m_ShaderWorldPos = m_pShader->getShaderVarId("matWorld");
    m_ShaderDiffTexPos = m_pShader->getShaderSamplerId("diffuseMap");
    //m_ShaderCamPos = m_pShader->getShaderVarId("vCamPos");
    //m_ShaderOverlayTexPos = m_pShader->getShaderSamplerId("overlayMap");

    happyengine::content::FontLoader fontLoader;
    fontLoader.load("../data/fonts/Ubuntu-Regular.ttf", 14, m_pFont);
}
void TestObject::tick(float dTime)
{
    using namespace happyengine;
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Left))
        m_Rotation += math::pi * dTime;
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Right))
        m_Rotation -= math::pi * dTime;
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Up))
        m_Position += math::Vector3(cosf(m_Rotation), 0, -sinf(m_Rotation)) * dTime * 5;
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Down))
        m_Position -= math::Vector3(cosf(m_Rotation), 0, -sinf(m_Rotation)) * dTime * 5;
}
void TestObject::draw(happyengine::graphics::I3DRenderer* pRenderer, float dTime)
{
    using namespace happyengine;
    m_pShader->begin();
    math::Matrix persp(math::Matrix::createPerspectiveLH(math::piOverFour, 1280, 720, 1, 1000));
    math::Matrix view(math::Matrix::createLookAtLH(math::Vector3(-5, 5, -4), math::Vector3(0, 0, 0), math::Vector3(0, 1, 0)));
    math::Matrix world(
        math::Matrix::createTranslation(m_Position) *
        math::Matrix::createRotation(math::Vector3(0.0f, 1.0f, 0.0f), m_Rotation) *
        math::Matrix::createScale(2.0f)
                      );

    m_pShader->setShaderVar(m_ShaderWVPpos, persp * view * world);
    m_pShader->setShaderVar(m_ShaderWorldPos, world);

    std::stringstream stream;
    stream << "fps: " << (int)(1/dTime);
    /*graphics::Texture2D::pointer pTextTex(m_pFont->createTextureText(stream.str(), Color(0.0f, 0.0f, 0.0f, 1.0f),
                                                     graphics::FontHAlignment_Center, graphics::FontVAlignment_Center));*/

    //m_pShader->setShaderVar(m_ShaderOverlayTexPos, pTextTex);
    m_pShader->setShaderVar(m_ShaderDiffTexPos, m_pDiffuseMap);
    //m_pShader->setShaderVar(m_ShaderCamPos, math::Vector3(-5, 5, -4));

    pRenderer->draw(m_pModel);

    m_pShader->end();
}

} //end namespace