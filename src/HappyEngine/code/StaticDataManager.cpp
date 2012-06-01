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
//Created: 24/03/2012
#include "HappyPCH.h" 

#include "StaticDataManager.h"
#include "ResourceFactory.h"
#include "Texture2D.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Font.h"
#include "PhysicsConcaveMesh.h"
#include "PhysicsConvexMesh.h"
#include "LightFactory.h"

namespace he {

void StaticDataManager::init()
{
    tools::Logger::sdmInit();
    ResourceFactory<gfx::Texture2D>::init(64, 64, "TextureFactory");
    ResourceFactory<gfx::Model>::init(64, 64, "ModelFactory");
    ResourceFactory<gfx::ModelMesh>::init(64, 64, "ModelMeshFactory");
    ResourceFactory<gfx::Font>::init(64, 64, "FontFactory");
    ResourceFactory<px::PhysicsConvexMesh>::init(64, 64, "PhysicsConvexMeshFactory");
    ResourceFactory<px::PhysicsConcaveMesh>::init(64, 64, "PhysicsConcaveMeshFactory");
    gfx::LightFactory::sdmInit();
}

void StaticDataManager::destroy()
{
    gfx::LightFactory::sdmDestroy();
    ResourceFactory<gfx::Font>::destroy();
    ResourceFactory<gfx::Texture2D>::destroy();
    ResourceFactory<gfx::Model>::destroy();
    ResourceFactory<gfx::ModelMesh>::destroy();
    ResourceFactory<px::PhysicsConvexMesh>::destroy();
    ResourceFactory<px::PhysicsConcaveMesh>::destroy();
    tools::Logger::sdmDestroy();
}

} //end namespace