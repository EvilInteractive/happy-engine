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

#include "Matrix.h"
#include "Vector3.h"
#include "MathConstants.h"
#include "HappyEngine.h"

namespace happytest {

MainGame::MainGame() : m_pSimpleForward3DRenderer(nullptr), m_pTestObject(nullptr), m_BackgroundIndex(0)
{
    m_BackgroundColors[0] = happyengine::Color((happyengine::byte)10, (happyengine::byte)130, (happyengine::byte)131, (happyengine::byte)255);
    m_BackgroundColors[1] = happyengine::Color((happyengine::byte)122, (happyengine::byte)186, (happyengine::byte)122, (happyengine::byte)255);
    m_BackgroundColors[2] = happyengine::Color((happyengine::byte)255, (happyengine::byte)127, (happyengine::byte)80, (happyengine::byte)255);
    m_BackgroundColors[3] = happyengine::Color((happyengine::byte)255, (happyengine::byte)165, (happyengine::byte)0, (happyengine::byte)255);
    m_BackgroundColors[4] = happyengine::Color((happyengine::byte)30, (happyengine::byte)144, (happyengine::byte)255, (happyengine::byte)255);
}


MainGame::~MainGame()
{
    delete m_pSimpleForward3DRenderer;
    delete m_pTestObject;
}

void MainGame::init()
{
}
void MainGame::load()
{
    m_pSimpleForward3DRenderer = new happyengine::graphics::SimpleForward3DRenderer();
    m_pTestObject = new TestObject();
    m_pTestObject->load();
}
void MainGame::tick(float dTime)
{
    m_pTestObject->tick(dTime);
}
void MainGame::draw(float /*dTime*/)
{
    if (happyengine::HappyEngine::getPointer()->getControls()->getKeyboard()->isKeyPressed(happyengine::io::Key_Return))
    {
        ++m_BackgroundIndex;
        if (m_BackgroundIndex > 4)
            m_BackgroundIndex = 0;
        happyengine::HappyEngine::getPointer()->getGraphicsEngine()->setBackgroundColor(m_BackgroundColors[m_BackgroundIndex]);
    }
    happyengine::HappyEngine::getPointer()->getGraphicsEngine()->clearAll();

    m_pTestObject->draw(m_pSimpleForward3DRenderer);
}

} //end namespace