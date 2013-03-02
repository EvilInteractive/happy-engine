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
//Created: 14/01/2012
#include "HappyPCH.h" 

#include "SkyBox.h"
#include "ContentManager.h"

#include "ModelMesh.h"
#include "Shader.h"
#include "ShaderVar.h"
#include "Texture2D.h"
#include "Material.h"
#include "TextureCube.h"

#include "Entity.h"
#include "ModelComponent.h"
#include "ShaderVar.h"

namespace he {
namespace gfx {

SkyBox::SkyBox()
    : m_CubeMap(nullptr)
    , m_Cube(nullptr)
    , m_Material(nullptr)
{
    const float largeNumber(1000000000);
    m_Bound.fromAABB(AABB(vec3(-largeNumber, -largeNumber, -largeNumber), vec3(largeNumber, largeNumber, largeNumber)));
}


SkyBox::~SkyBox()
{
    unload();
}

void SkyBox::unload()
{
    if (m_CubeMap != nullptr)
    {
        m_CubeMap->release();
        m_CubeMap = nullptr;
    }
    if (m_Cube != nullptr)
    {
        m_Cube->release();
        m_Cube = nullptr;
    }
    if (m_Material != nullptr)
    {
        m_Material->release();
        m_Material = nullptr;
    }
}

void SkyBox::load( const std::string& asset )
{
    //////////////////////////////////////////////////////////////////////////
    /// Load Model
    //////////////////////////////////////////////////////////////////////////
    m_Cube = ResourceFactory<gfx::ModelMesh>::getInstance()->get(ResourceFactory<gfx::ModelMesh>::getInstance()->create());
    m_Cube->setName("skybox");
    he::PrimitiveList<vec3> vertices(8);
    vertices.add(vec3(-1,  1, -1));
    vertices.add(vec3( 1,  1, -1));
    vertices.add(vec3(-1, -1, -1));
    vertices.add(vec3( 1, -1, -1));

    vertices.add(vec3(-1,  1,  1));
    vertices.add(vec3( 1,  1,  1));
    vertices.add(vec3(-1, -1,  1));
    vertices.add(vec3( 1, -1,  1));

    he::PrimitiveList<uint8> indices(36);
    indices.add(0); indices.add(1); indices.add(2); //front
    indices.add(1); indices.add(3); indices.add(2);

    indices.add(5); indices.add(4); indices.add(7); //back
    indices.add(4); indices.add(6); indices.add(7);

    indices.add(4); indices.add(0); indices.add(6); //left
    indices.add(0); indices.add(2); indices.add(6);

    indices.add(1); indices.add(5); indices.add(3); //right
    indices.add(5); indices.add(7); indices.add(3);

    indices.add(4); indices.add(5); indices.add(0); //top
    indices.add(5); indices.add(1); indices.add(0);

    indices.add(3); indices.add(7); indices.add(2); //bottom
    indices.add(7); indices.add(6); indices.add(2);

    BufferLayout layout;
    layout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, sizeof(vec3), 0));
    m_Cube->init(layout, MeshDrawMode_Triangles);
    m_Cube->setVertices(&vertices[0], static_cast<uint32>(vertices.size()), MeshUsage_Static, false);
    m_Cube->setIndices(&indices[0], static_cast<uint32>(indices.size()), IndexStride_Byte, MeshUsage_Static);
    m_Cube->setLoaded();

    m_Material = he::ResourceFactory<gfx::Material>::getInstance()->get(CONTENT->loadMaterial("engine/sky.material"));
    ShaderUserVar<const TextureCube*>* cubeMap(static_cast<ShaderUserVar<const gfx::TextureCube*>*>(m_Material->getVar("cubeMap")));
    if (cubeMap != nullptr)
    {
        const TextureCube* cube(CONTENT->asyncLoadTextureCube(asset));
        cubeMap->setData(cube);
        cube->release();
    }

}

} } //end namespace