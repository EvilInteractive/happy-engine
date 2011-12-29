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
//Author:  Bastian Damman
//Created: 07/12/2011
#include "HappyPCH.h" 

#include "InstancingManager.h"
#include "InstancingController.h"

namespace he {
namespace gfx {

InstancingManager::InstancingManager()
{
}


InstancingManager::~InstancingManager()
{
    std::for_each(m_Controllers.cbegin(), m_Controllers.cend(), [](const std::pair<std::string, InstancingController*>& pair)
    {
        delete pair.second;
    });
}

void InstancingManager::createController( const std::string& id, bool dynamic, const ModelMesh::pointer& pMesh, const Material& material )
{
    InstancingController* pController(NEW InstancingController(dynamic, pMesh, material));
    m_Controllers[id] = pController;
}

InstancingController* InstancingManager::getController( const std::string& id )
{
    auto it(m_Controllers.find(id));
    return (it != m_Controllers.cend())? it->second : nullptr;
}

} } //end namespace