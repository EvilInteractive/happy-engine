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
//Created: 30/09/2011

#include "EntityLoader.h"
#include "HappyNew.h"
#include "IniReader.h"
#include "HappyEngine.h"
#include "VertexLayout.h"
#include "Texture2D.h"
#include "FileNotFoundException.h"

namespace happyengine {
namespace content {

EntityLoader::EntityLoader(): m_pAssetContainer(NEW AssetContainer<game::EntityDesc>())
{
}


EntityLoader::~EntityLoader()
{
    delete m_pAssetContainer;
}

game::EntityDesc EntityLoader::load(const std::string& path)
{
    if (m_pAssetContainer->isAssetPresent(path))
    {
        return m_pAssetContainer->getAsset(path);
    }
    else
    {
        io::IniReader reader;
        
        try { reader.open(path); }
        catch (error::FileNotFoundException& e)
        { std::wcout << e.getMsg() << "\n"; }
        
        game::EntityDesc desc;
        if (reader.isOpen())
        {            
            // [Physics]
            if (reader.containsRoot(L"Physics"))
            {
                desc.physicsDesc.usePhysics = true;
                desc.physicsDesc.shape = reader.readString(L"Physics", L"model", "");
                math::Vector3 mat(reader.readVector3(L"Physics", L"material", math::Vector3(0.8f, 0.7f, 0.1f)));
                desc.physicsDesc.pMaterial = physics::PhysicsMaterial::pointer(NEW physics::PhysicsMaterial(mat.x, mat.y, mat.z));
                desc.physicsDesc.density = reader.readFloat(L"Physics", L"density", 1.0f);
            }

            graphics::VertexLayout vertexLayout;
            // [Shader]
            {
                io::IniReader shaderReader;               
                try { shaderReader.open(CONTENT->getRootDir() + CONTENT->getShaderFolder() + reader.readString(L"Shader", L"shader", "")); }
                catch (error::FileNotFoundException& e)
                { std::wcout << e.getMsg() << "\n"; }

                std::vector<std::string> shaderOutputs;

                // [out]
                if (shaderReader.containsRoot(L"out"))
                {
                    const std::map<std::wstring, std::wstring>& outNodes(shaderReader.getNodes(L"out"));
                    if (outNodes.size() == 3)
                    {
                        shaderOutputs.resize(3);
                        std::for_each(outNodes.cbegin(), outNodes.cend(), [&](const std::pair<std::wstring, std::wstring>& p)
                        {
                            if (p.second == L"GBUFFER_COLOR_ILLUMINATION")
                                shaderOutputs[0] = std::string(p.first.cbegin(), p.first.cend());
                            else if (p.second == L"GBUFFER_POSITION_SPECULAR")
                                shaderOutputs[1] = std::string(p.first.cbegin(), p.first.cend());
                            else if (p.second == L"GBUFFER_NORMAL_GLOSSINESS")
                                shaderOutputs[2] = std::string(p.first.cbegin(), p.first.cend());
                            else
                                ASSERT(false, "unknow semantic");
                        });
                    }
                }

                // [in]
                graphics::ShaderLayout shaderLayout;
                uint count(0);
                const std::map<std::wstring, std::wstring>& inNodes(shaderReader.getNodes(L"in"));
                std::for_each(inNodes.cbegin(), inNodes.cend(), [&](const std::pair<std::wstring, std::wstring>& p)
                {
                    if (p.second == L"POSITION")
                    {
                        vertexLayout.addElement(graphics::VertexElement(count, graphics::VertexElement::Type_Vector3, graphics::VertexElement::Usage_Position, sizeof(math::Vector3), 0));
                    }
                    else if (p.second == L"TEXCOORD")
                    {
                        vertexLayout.addElement(graphics::VertexElement(count, graphics::VertexElement::Type_Vector2, graphics::VertexElement::Usage_TextureCoordinate, sizeof(math::Vector2), 12));
                    }
                    else if (p.second == L"NORMAL")
                    {
                        vertexLayout.addElement(graphics::VertexElement(count, graphics::VertexElement::Type_Vector3, graphics::VertexElement::Usage_Normal, sizeof(math::Vector3), 20));
                    }
                    else if (p.second == L"TANGENT")
                    {
                        vertexLayout.addElement(graphics::VertexElement(count, graphics::VertexElement::Type_Vector3, graphics::VertexElement::Usage_Tangent, sizeof(math::Vector3), 32));
                    }
                    shaderLayout.addElement(graphics::ShaderLayoutElement(count++, std::string(p.first.cbegin(), p.first.cend())));
                }); 

                // [Shader]
                graphics::Shader::pointer pShader(CONTENT->loadShader(shaderReader.readString(L"Shader", L"vsPath", ""),
                                                                      shaderReader.readString(L"Shader", L"fsPath", ""),
                                                                      shaderLayout,
                                                                      shaderOutputs));
                desc.material.setShader(pShader);

                // [uniform]
                if (shaderReader.containsRoot(L"uniform"))
                {
                    const std::map<std::wstring, std::wstring>& uniformNodes(shaderReader.getNodes(L"uniform"));
                    std::for_each(uniformNodes.cbegin(), uniformNodes.cend(), [&](const std::pair<std::wstring, std::wstring> node)
                    {
                        if (node.second == L"WORLDVIEWPROJECTION")
                        {
                            desc.material.addVar(graphics::ShaderVar::pointer(
                                NEW graphics::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), graphics::ShaderVarType_WVP)));
                        }
                        else if (node.second == L"WORLD")
                        {
                            desc.material.addVar(graphics::ShaderVar::pointer(
                                NEW graphics::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), graphics::ShaderVarType_World)));
                        }
                        else if (node.second == L"TEXTURE2D")
                        {
                            graphics::Texture2D::pointer tex; 
                            math::Vector4 testColorMap(reader.readVector4(L"TEXTURE2D", node.first, math::Vector4(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN)));
                            if (testColorMap == math::Vector4(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN))
                                tex = CONTENT->asyncLoadTexture(reader.readString(L"TEXTURE2D", node.first, ""));
                            else
                                tex = CONTENT->asyncMakeTexture(Color(testColorMap));
                            graphics::ShaderVar::pointer var(
                                NEW graphics::ShaderUserVar<graphics::Texture2D::pointer>(
                                    pShader->getShaderSamplerId(std::string(node.first.cbegin(), node.first.cend())), 
                                    tex));             
                            desc.material.addVar(var);
                        }
                    });
                }
            }

            // [Entity]
            desc.className = reader.readString(L"Entity", L"class", "");
            desc.pModel = CONTENT->asyncLoadModel(reader.readString(L"Entity", L"model", ""), vertexLayout);

        }
        m_pAssetContainer->addAsset(path, desc);
        return desc;
    }
}

} } //end namespace