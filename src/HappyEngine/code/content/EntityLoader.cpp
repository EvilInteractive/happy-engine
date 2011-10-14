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
#include "StdAfx.h" 

#include "EntityLoader.h"
#include "HappyNew.h"
#include "IniReader.h"
#include "HappyEngine.h"
#include "VertexLayout.h"
#include "Texture2D.h"
#include "FileNotFoundException.h"
#include "ContentManager.h"

namespace he {
namespace ct {

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
        catch (err::FileNotFoundException& e)
        { std::wcout << e.getMsg() << "\n"; }
        
        game::EntityDesc desc;
        if (reader.isOpen())
        {            
            // [Physics]
            if (reader.containsRoot(L"Physics"))
            {
                desc.physicsDesc.usePhysics = true;
                desc.physicsDesc.shape = reader.readString(L"Physics", L"model", "");
                vec3 mat(reader.readVector3(L"Physics", L"material", vec3(0.8f, 0.7f, 0.1f)));
                desc.physicsDesc.pMaterial = px::PhysicsMaterial::pointer(NEW px::PhysicsMaterial(mat.x, mat.y, mat.z));
                desc.physicsDesc.density = reader.readFloat(L"Physics", L"density", 1.0f);
            }

            gfx::VertexLayout vertexLayout;
            // [Shader]
            {
                io::IniReader shaderReader;               
                try { shaderReader.open(CONTENT->getRootDir() + CONTENT->getShaderFolder() + reader.readString(L"Shader", L"shader", "")); }
                catch (err::FileNotFoundException& e)
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
                            if (p.second == L"GBUFFER_COLOR")
                                shaderOutputs[0] = std::string(p.first.cbegin(), p.first.cend());
                            else if (p.second == L"GBUFFER_SGI")
                                shaderOutputs[1] = std::string(p.first.cbegin(), p.first.cend());
                            else if (p.second == L"GBUFFER_NORMAL")
                                shaderOutputs[2] = std::string(p.first.cbegin(), p.first.cend());
                            else
                                ASSERT(false, "unknow semantic");
                        });
                    }
                }

                // [in]
                gfx::ShaderLayout shaderLayout;
                uint count(0);
                const std::map<std::wstring, std::wstring>& inNodes(shaderReader.getNodes(L"in"));
                std::for_each(inNodes.cbegin(), inNodes.cend(), [&](const std::pair<std::wstring, std::wstring>& p)
                {
                    if (p.second == L"POSITION")
                    {
                        vertexLayout.addElement(gfx::VertexElement(count, gfx::VertexElement::Type_Vector3, gfx::VertexElement::Usage_Position, sizeof(vec3), 0));
                    }
                    else if (p.second == L"TEXCOORD")
                    {
                        vertexLayout.addElement(gfx::VertexElement(count, gfx::VertexElement::Type_Vector2, gfx::VertexElement::Usage_TextureCoordinate, sizeof(vec2), 12));
                    }
                    else if (p.second == L"NORMAL")
                    {
                        vertexLayout.addElement(gfx::VertexElement(count, gfx::VertexElement::Type_Vector3, gfx::VertexElement::Usage_Normal, sizeof(vec3), 20));
                    }
                    else if (p.second == L"TANGENT")
                    {
                        vertexLayout.addElement(gfx::VertexElement(count, gfx::VertexElement::Type_Vector3, gfx::VertexElement::Usage_Tangent, sizeof(vec3), 32));
                    }
                    shaderLayout.addElement(gfx::ShaderLayoutElement(count++, std::string(p.first.cbegin(), p.first.cend())));
                }); 

                // [Shader]
                gfx::Shader::pointer pShader(CONTENT->loadShader(shaderReader.readString(L"Shader", L"vsPath", ""),
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
                            desc.material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_WVP)));
                        }
                        else if (node.second == L"WORLD")
                        {
                            desc.material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_World)));
                        }
                        else if (node.second == L"TEXTURE2D")
                        {
                            gfx::Texture2D::pointer tex; 
                            vec4 testColorMap(reader.readVector4(L"TEXTURE2D", node.first, vec4(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN)));
                            if (testColorMap == vec4(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN))
                                tex = CONTENT->asyncLoadTexture(reader.readString(L"TEXTURE2D", node.first, ""));
                            else
                                tex = CONTENT->asyncMakeTexture(Color(testColorMap));
                            gfx::ShaderVar::pointer var(
                                NEW gfx::ShaderUserVar<gfx::Texture2D::pointer>(
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