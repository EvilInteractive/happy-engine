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

namespace he {
namespace gfx {

SkyBox::SkyBox(): m_IsVisible(false), m_LoadedCount(0), m_pCubeMap(NEW TextureCube())
{
}


SkyBox::~SkyBox()
{
}

void SkyBox::load( const std::string& asset )
{

    //////////////////////////////////////////////////////////////////////////
    /// Load Textures
    //////////////////////////////////////////////////////////////////////////    
    uint extPos(asset.rfind('.'));
    std::string name(asset.substr(0, extPos));
    std::string ext(asset.substr(extPos));

    m_CubeFaces[Cube_Back] = CONTENT->asyncLoadTexture(name + "_back" + ext, true);
    m_CubeFaces[Cube_Back]->callbackIfLoaded(boost::bind(&SkyBox::faceLoaded, this));
    m_CubeFaces[Cube_Front] = CONTENT->asyncLoadTexture(name + "_front" + ext, true);
    m_CubeFaces[Cube_Front]->callbackIfLoaded(boost::bind(&SkyBox::faceLoaded, this));

    m_CubeFaces[Cube_Bottom] = CONTENT->asyncLoadTexture(name + "_bottom" + ext, true);
    m_CubeFaces[Cube_Bottom]->callbackIfLoaded(boost::bind(&SkyBox::faceLoaded, this));
    m_CubeFaces[Cube_Top] = CONTENT->asyncLoadTexture(name + "_top" + ext, true);
    m_CubeFaces[Cube_Top]->callbackIfLoaded(boost::bind(&SkyBox::faceLoaded, this));

    m_CubeFaces[Cube_Left] = CONTENT->asyncLoadTexture(name + "_left" + ext, true);
    m_CubeFaces[Cube_Left]->callbackIfLoaded(boost::bind(&SkyBox::faceLoaded, this));
    m_CubeFaces[Cube_Right] = CONTENT->asyncLoadTexture(name + "_right" + ext, true);
    m_CubeFaces[Cube_Right]->callbackIfLoaded(boost::bind(&SkyBox::faceLoaded, this));



    //////////////////////////////////////////////////////////////////////////
    /// Load Model
    //////////////////////////////////////////////////////////////////////////
    m_pCube = ModelMesh::pointer(NEW ModelMesh("skyBox"));
    std::vector<vec3> vertices;
    vertices.push_back(vec3(-1,  1, -1));
    vertices.push_back(vec3( 1,  1, -1));
    vertices.push_back(vec3(-1, -1, -1));
    vertices.push_back(vec3( 1, -1, -1));

    vertices.push_back(vec3(-1,  1,  1));
    vertices.push_back(vec3( 1,  1,  1));
    vertices.push_back(vec3(-1, -1,  1));
    vertices.push_back(vec3( 1, -1,  1));

    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(1); indices.push_back(2); //front
    indices.push_back(1); indices.push_back(3); indices.push_back(2);

    indices.push_back(5); indices.push_back(4); indices.push_back(7); //back
    indices.push_back(4); indices.push_back(6); indices.push_back(7);

    indices.push_back(4); indices.push_back(0); indices.push_back(6); //left
    indices.push_back(0); indices.push_back(2); indices.push_back(6);

    indices.push_back(1); indices.push_back(5); indices.push_back(3); //right
    indices.push_back(5); indices.push_back(7); indices.push_back(3);

    indices.push_back(4); indices.push_back(5); indices.push_back(0); //top
    indices.push_back(5); indices.push_back(1); indices.push_back(0);

    indices.push_back(3); indices.push_back(7); indices.push_back(2); //bottom
    indices.push_back(7); indices.push_back(6); indices.push_back(2);

    BufferLayout layout;
    layout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, sizeof(vec3), 0));
    m_pCube->init();
    m_pCube->setVertices(vertices.data(), vertices.size(), layout);\
    m_pCube->setIndices(indices.data(), indices.size(), IndexStride_Byte);

    //////////////////////////////////////////////////////////////////////////
    /// Load Shader
    //////////////////////////////////////////////////////////////////////////
    ShaderLayout shaderLayout;
    shaderLayout.addElement(ShaderLayoutElement(0, "inPosition"));
    Shader::pointer pShader = CONTENT->loadShader("forward/skybox.vert", "forward/skybox.frag", shaderLayout, std::vector<std::string>());

    //////////////////////////////////////////////////////////////////////////
    /// Load Material
    //////////////////////////////////////////////////////////////////////////
    BufferLayout instancingLayout;
    m_Material.setShader(pShader, layout, instancingLayout);
    m_Material.addVar(gfx::ShaderVar::pointer(
        NEW gfx::ShaderGlobalVar(pShader->getShaderVarId("matVP"), "matVP", gfx::ShaderVarType_ViewProjection)));
    m_Material.addVar(gfx::ShaderVar::pointer(
        NEW gfx::ShaderUserVar<gfx::TextureCube::pointer>(
        pShader->getShaderSamplerId("cubeMap"), "cubeMap", m_pCubeMap)));

    m_Material.setIsBlended(false);
    m_Material.setNoPost(true);
    m_Material.setIsBackground(true);

}
void SkyBox::faceLoaded()
{
    if (++m_LoadedCount == 6)
    {
        ASSERT(m_pCubeMap->isInitialized() == false, "cubemap allready initialized");

        uint cubeMapId;
        glGenTextures(1, &cubeMapId);
        m_pCubeMap->init(cubeMapId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        for (int i(0); i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, 512, 512, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_CubeFaces[i]->getPixelsIfAvailable());
        }

        m_IsVisible = true;
    }
}


const ModelMesh::pointer& SkyBox::getModelMesh() const
{
    return m_pCube;
}

void SkyBox::applyMaterial( const ICamera* pCamera ) const
{
    m_Material.apply(this, pCamera);
}

void SkyBox::applyMaterial( const Material& customMaterial, const ICamera* pCamera ) const
{
    customMaterial.apply(this, pCamera);
}

const Material& SkyBox::getMaterial() const
{
    return m_Material;
}

void SkyBox::draw()
{
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);
    GL::heSetCullFace(true);
    GL::heBindVao(m_pCube->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_pCube->getNumIndices(), m_pCube->getIndexType(), 0);
    GL::heSetCullFace(false);
    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(true);
}

bool SkyBox::isVisible() const
{
    return m_IsVisible;
}

bool SkyBox::isInCamera( const ICamera* /*pCamera*/ ) const
{
    return m_IsVisible;
}

} } //end namespace