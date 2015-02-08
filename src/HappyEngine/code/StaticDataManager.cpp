//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
#include "thread/ThreadTicket.h"
#include "ResourceFactory.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Mesh2D.h"
#include "ShapeMesh.h"
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
#include "SpriteCreator.h"
#include "PickingManager.h"
#include "BinaryVisitor.h"
#include "EntityManager.h"
#include "SystemStats.h"
#include "GlobalStringTable.h"
#include "GlobalSettings.h"
#include "NetworkManager.h"
#include <HappyMessageBox.h>
#ifdef USE_WEB
#pragma warning(disable:4100)
#include <Awesomium/WebCore.h>
#pragma warning(default:4100)
#endif

namespace he {

void StaticDataManager::init()
{
#ifdef HE_DEBUG
    he::err::details::sdmInitAsserts();
#endif
#ifdef USE_WEB
    Awesomium::WebCore::Initialize(Awesomium::WebConfig());
#endif
    he::ThreadTicketManager::sdmInit();
    he::Path msbbox(he::Path::getDataPath().append("gui/messageBox.html"));
    he::HappyMessageBox::init(1024, 512);
    tools::Logger::sdmInit();
    he::GlobalStringTable::sdmInit();
    he::HEFS::sdmInit();
    he::tools::SystemStats::sdmInit();
    details::ObjectFactoryTypeManager::sdmInit();
    ResourceFactory<gfx::Material>::sdmInit();
    ResourceFactory<gfx::Material>::getInstance()->init(0, 16, "MaterialFactory");
    ResourceFactory<gfx::Texture2D>::sdmInit();
    ResourceFactory<gfx::Texture2D>::getInstance()->init(0, 16, "TextureFactory");
    ResourceFactory<gfx::TextureCube>::sdmInit();
    ResourceFactory<gfx::TextureCube>::getInstance()->init(0, 5, "TextureCubeFactory");
    ResourceFactory<gfx::Model>::sdmInit();
    ResourceFactory<gfx::Model>::getInstance()->init(0, 16, "ModelFactory");
    ResourceFactory<gfx::ModelMesh>::sdmInit();
    ResourceFactory<gfx::ModelMesh>::getInstance()->init(0, 16, "ModelMeshFactory");
    ResourceFactory<gui::Font>::sdmInit();
    ResourceFactory<gui::Font>::getInstance()->init(0, 2, "FontFactory");
    ResourceFactory<gfx::Shader>::sdmInit();
    ResourceFactory<gfx::Shader>::getInstance()->init(0, 8, "ShaderFactory");
    ResourceFactory<px::PhysicsConvexMesh>::sdmInit();
    ResourceFactory<px::PhysicsConvexMesh>::getInstance()->init(0, 16, "PhysicsConvexMeshFactory");
    ResourceFactory<px::PhysicsConcaveMesh>::sdmInit();
    ResourceFactory<px::PhysicsConcaveMesh>::getInstance()->init(0, 16, "PhysicsConcaveMeshFactory");
    gui::SpriteFactory::sdmInit();
    gfx::ViewFactory::sdmInit();
    gfx::SceneFactory::sdmInit();
    gfx::WindowFactory::sdmInit();
    gfx::LightFactory::sdmInit();
#ifdef HE_USE_OCTREE
    gfx::CullOctreeNodeFactory::sdmInit();
#endif
    ct::ShaderGeneratorVariableFactory::sdmInit();
    ge::PickingManager::sdmInit();
    ge::EntityManager::sdmInit();
    GlobalSettings::sdmInit();
    gfx::Mesh2D::sdmInit();
    gfx::ShapeMesh::sdmInit();
    net::NetworkManager::sdmInit();
}

void StaticDataManager::destroy()
{
    net::NetworkManager::sdmDestroy();
    gfx::ShapeMesh::sdmDestroy();
    gfx::Mesh2D::sdmDestroy();
    GlobalSettings::sdmDestroy();
    ge::EntityManager::sdmDestroy();
    ge::PickingManager::sdmDestroy();
    ct::ShaderGeneratorVariableFactory::sdmDestroy();
#ifdef HE_USE_OCTREE
    gfx::CullOctreeNodeFactory::sdmDestroy();
#endif
    gfx::LightFactory::sdmDestroy();
    ResourceFactory<gui::Font>::sdmDestroy();
    ResourceFactory<gfx::Material>::sdmDestroy();
    ResourceFactory<gfx::TextureCube>::sdmDestroy();
    ResourceFactory<gfx::Texture2D>::sdmDestroy();
    ResourceFactory<gfx::Model>::sdmDestroy();
    ResourceFactory<gfx::ModelMesh>::sdmDestroy();
    ResourceFactory<gfx::Shader>::sdmDestroy();
    ResourceFactory<px::PhysicsConvexMesh>::sdmDestroy();
    ResourceFactory<px::PhysicsConcaveMesh>::sdmDestroy();
    gui::SpriteFactory::sdmDestroy();
    gfx::ViewFactory::sdmDestroy();
    gfx::SceneFactory::sdmDestroy();
    gfx::WindowFactory::sdmDestroy();
    details::ObjectFactoryTypeManager::sdmDestroy();
    he::tools::SystemStats::sdmDestroy();
    he::HEFS::sdmDestroy();
    he::GlobalStringTable::sdmDestroy();
    tools::Logger::sdmDestroy();
    he::HappyMessageBox::destroy();
    he::ThreadTicketManager::sdmDestroy();
#ifdef USE_WEB
    Awesomium::WebCore::Shutdown();
#endif
#ifdef HE_DEBUG
    he::err::details::sdmDestroyAsserts();
#endif
}

} //end namespace
