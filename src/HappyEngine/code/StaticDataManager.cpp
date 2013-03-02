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
#include "TextureCube.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Font.h"
#include "PhysicsConcaveMesh.h"
#include "PhysicsConvexMesh.h"
#include "LightFactory.h"
#include "Shader.h"
#include "Material.h"
#include "View.h"
#include "Scene.h"
#include "Window.h"
#include "CullOctreeNodeFactory.h"
#include "ShaderGeneratorVariableFactory.h"
#include "Sprite.h"
#include "materialGenerator/MaterialGeneratorNodeFactory.h"
#include "SpriteCreator.h"
#include "PickingManager.h"

namespace he {

void StaticDataManager::init()
{
    details::ObjectFactoryTypeManager::sdmInit();
    tools::Logger::sdmInit();
    ResourceFactory<gfx::Material>::init(64, 64, "MaterialFactory");
    ResourceFactory<gfx::Texture2D>::init(64, 64, "TextureFactory");
    ResourceFactory<gfx::TextureCube>::init(5, 5, "TextureCubeFactory");
    ResourceFactory<gfx::Model>::init(64, 64, "ModelFactory");
    ResourceFactory<gfx::ModelMesh>::init(64, 64, "ModelMeshFactory");
    ResourceFactory<gui::Font>::init(64, 64, "FontFactory");
    ResourceFactory<gfx::Shader>::init(16, 16, "ShaderFactory");
    ResourceFactory<px::PhysicsConvexMesh>::init(64, 64, "PhysicsConvexMeshFactory");
    ResourceFactory<px::PhysicsConcaveMesh>::init(64, 64, "PhysicsConcaveMeshFactory");
    gui::SpriteFactory::sdmInit();
    gfx::ViewFactory::sdmInit();
    gfx::SceneFactory::sdmInit();
    gfx::WindowFactory::sdmInit();
    gfx::LightFactory::sdmInit();
    gfx::CullOctreeNodeFactory::sdmInit();
    ct::ShaderGeneratorVariableFactory::sdmInit();
    tools::MaterialGeneratorNodeFactory::sdmInit();
    ge::PickingManager::sdmInit();
}

void StaticDataManager::destroy()
{
    ge::PickingManager::sdmDestroy();
    tools::MaterialGeneratorNodeFactory::sdmDestroy();
    ct::ShaderGeneratorVariableFactory::sdmDestroy();
    gfx::CullOctreeNodeFactory::sdmDestroy();
    gfx::LightFactory::sdmDestroy();
    ResourceFactory<gui::Font>::destroy();
    ResourceFactory<gfx::Material>::destroy();
    ResourceFactory<gfx::TextureCube>::destroy();
    ResourceFactory<gfx::Texture2D>::destroy();
    ResourceFactory<gfx::Model>::destroy();
    ResourceFactory<gfx::ModelMesh>::destroy();
    ResourceFactory<gfx::Shader>::destroy();
    ResourceFactory<px::PhysicsConvexMesh>::destroy();
    ResourceFactory<px::PhysicsConcaveMesh>::destroy();
    gui::SpriteFactory::sdmDestroy();
    gfx::ViewFactory::sdmDestroy();
    gfx::SceneFactory::sdmDestroy();
    gfx::WindowFactory::sdmDestroy();
    tools::Logger::sdmDestroy();
    details::ObjectFactoryTypeManager::sdmDestroy();
}

} //end namespace