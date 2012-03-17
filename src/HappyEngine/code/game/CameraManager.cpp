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
//Author:  Bastian Damman
//Created: 06/12/2011
#include "HappyPCH.h" 

#include "CameraManager.h"
#include "Camera.h"
#include "HappyEngine.h"
#include "Game.h"

namespace he {
namespace game {

CameraManager::CameraManager(): m_pActiveCamera(nullptr)
{
}


CameraManager::~CameraManager()
{
}

void CameraManager::tick(float dTime)
{
    if (m_pActiveCamera != nullptr)
        m_pActiveCamera->tick(dTime);
}

void CameraManager::init()
{
    GAME->addToTickList(this);
}

gfx::Camera* game::CameraManager::getActiveCamera() const
{
    return m_pActiveCamera;
}

void game::CameraManager::addCamera( const std::string& id, gfx::Camera* pCamera )
{
    HE_ASSERT(m_Cameras.find(id) == m_Cameras.cend(), "Adding camera to existing ID, memleak will occure");
    m_Cameras[id] = pCamera;
}

void game::CameraManager::deleteCamera( const std::string& id )
{
    HE_ASSERT(m_Cameras.find(id) != m_Cameras.cend(), "Deleting non existing camera");
    delete m_Cameras[id];
    m_Cameras.erase(id);
}

void game::CameraManager::deleteAllCameras()
{
    std::for_each(m_Cameras.cbegin(), m_Cameras.cend(), [](const std::pair<std::string, gfx::Camera*>& pr)
    {
        delete pr.second;
    });
    m_Cameras.clear();
}

void game::CameraManager::setActiveCamera( const std::string& id )
{
    HE_ASSERT(m_Cameras.find(id) != m_Cameras.cend(), "Selected Camera does not exist");
    m_pActiveCamera = m_Cameras.find(id)->second;
}

} } //end namespace
