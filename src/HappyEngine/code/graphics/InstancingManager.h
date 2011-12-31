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

#ifndef _HE_INSTANCING_MANAGER_H_
#define _HE_INSTANCING_MANAGER_H_
#pragma once

#include <map>
#include "ModelMesh.h"
#include "Material.h"
#include "BufferLayout.h"

namespace he {
namespace gfx {

class InstancingController;

class InstancingManager
{
public:
    InstancingManager();
    virtual ~InstancingManager();

    void createController(const std::string& id, bool dynamic, const ModelMesh::pointer& pMesh, const Material& material);
    InstancingController* getController(const std::string& id);

private:

    std::map<std::string, InstancingController*> m_Controllers;

    //Disable default copy constructor and default assignment operator
    InstancingManager(const InstancingManager&);
    InstancingManager& operator=(const InstancingManager&);
};

} } //end namespace

#endif