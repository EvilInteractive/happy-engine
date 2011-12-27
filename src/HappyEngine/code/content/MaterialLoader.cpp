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
#include "HappyPCH.h" 

#include "MaterialLoader.h"
#include "HappyNew.h"
#include "IniReader.h"
#include "HappyEngine.h"
#include "VertexLayout.h"
#include "Texture2D.h"
#include "FileNotFoundException.h"
#include "ContentManager.h"
#include "Bone.h"

namespace he {
namespace ct {

MaterialLoader::MaterialLoader(): m_pAssetContainer(NEW AssetContainer<gfx::Material>())
{
}


MaterialLoader::~MaterialLoader()
{
    delete m_pAssetContainer;
}

gfx::Material MaterialLoader::load(const std::string& path)
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
        
        gfx::Material material;
        if (reader.isOpen())
        {     
            gfx::VertexLayout vertexLayout;
            // [Shader]
            {
                std::string file;
                file = reader.readString(L"Forward", L"shader", "");
                if (m_RenderSettings.enableDeferred)
                {
                    std::string temp = reader.readString(L"Deferred", L"shader", file);
                    if (temp != file)
                    {
                        file = temp;
                    }
                }

                io::IniReader shaderReader;
                try 
                { 
                    shaderReader.open(CONTENT->getRootDir() + CONTENT->getShaderFolder() + file); 
                }
                catch (err::FileNotFoundException& e)
                { std::wcout << e.getMsg() << L": in " + std::wstring(path.cbegin(), path.cend()) + L"\n"; }

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
                    else if (outNodes.size() == 2)
                    {
                        shaderOutputs.resize(2);
                        std::for_each(outNodes.cbegin(), outNodes.cend(), [&](const std::pair<std::wstring, std::wstring>& p)
                        {
                            if (p.second == L"GBUFFER_COLOR")
                                shaderOutputs[0] = std::string(p.first.cbegin(), p.first.cend());
                            else if (p.second == L"GBUFFER_NORMAL")
                                shaderOutputs[1] = std::string(p.first.cbegin(), p.first.cend());
                            else
                                ASSERT(false, "unknow semantic");
                        });
                    }
                }

                // [in]
                gfx::ShaderLayout shaderLayout;
                uint count(0);
                uint offset(0);
                std::string usedForInstancing("");
                bool isTranslucent(false);
                const std::map<std::wstring, std::wstring>& inNodes(shaderReader.getNodes(L"in"));
                std::for_each(inNodes.cbegin(), inNodes.cend(), [&](const std::pair<std::wstring, std::wstring>& p)
                {
                    if (p.second == L"POSITION")
                    {
                        vertexLayout.addElement(gfx::VertexElement(count, gfx::VertexElement::Type_Vec3, gfx::VertexElement::Usage_Position, sizeof(vec3), offset));
                        offset += sizeof(vec3);
                    }
                    else if (p.second == L"TEXCOORD")
                    {
                        vertexLayout.addElement(gfx::VertexElement(count, gfx::VertexElement::Type_Vec2, gfx::VertexElement::Usage_TextureCoordinate, sizeof(vec2), offset));
                        offset += sizeof(vec2);
                    }
                    else if (p.second == L"NORMAL")
                    {
                        vertexLayout.addElement(gfx::VertexElement(count, gfx::VertexElement::Type_Vec3, gfx::VertexElement::Usage_Normal, sizeof(vec3), offset));
                        offset += sizeof(vec3);
                    }
                    else if (p.second == L"TANGENT")
                    {
                        vertexLayout.addElement(gfx::VertexElement(count, gfx::VertexElement::Type_Vec3, gfx::VertexElement::Usage_Tangent, sizeof(vec3), offset));
                        offset += sizeof(vec3);
                    }
                    else if (p.second == L"BONEIDS")
                    {
                        vertexLayout.addElement(gfx::VertexElement(count, gfx::VertexElement::Type_Vec4, gfx::VertexElement::Usage_BoneIDs, sizeof(vec4), offset));
                        offset += sizeof(vec4);
                    }
                    else if (p.second == L"BONEWEIGHTS")
                    {
                        ASSERT(gfx::Bone::MAX_BONEWEIGHTS == 4, "layout incompatible");
                        vertexLayout.addElement(gfx::VertexElement(count, gfx::VertexElement::Type_Vec4, gfx::VertexElement::Usage_BoneWeights, sizeof(vec4), offset));
                        offset += sizeof(vec4);
                    }
                    else if (p.second == L"WORLD")
                    {
                        usedForInstancing = std::string(p.first.cbegin(), p.first.cend());
                    }
                    else
                    {
                        std::wcout << "**Material Error**: unkown attribute " << p.second << "\n";
                    } 
                    if (p.second != L"WORLD")
                        shaderLayout.addElement(gfx::ShaderLayoutElement(count++, std::string(p.first.cbegin(), p.first.cend())));
                }); 

                //Instancing matrices must be last in layout
                if (usedForInstancing != "")
                    shaderLayout.addElement(gfx::ShaderLayoutElement(count++, usedForInstancing));

                // [Shader]
                gfx::Shader::pointer pShader(CONTENT->loadShader(shaderReader.readString(L"Shader", L"vsPath", ""),
                                                                 shaderReader.readString(L"Shader", L"fsPath", ""),
                                                                 shaderLayout,
                                                                 shaderOutputs));

                // [info]
                if (shaderReader.containsRoot(L"info"))
                {
                    isTranslucent = shaderReader.readBool(L"info", L"translucent", false);
                }

                material.setIsTranslucent(isTranslucent);
                material.setShader(pShader, vertexLayout, usedForInstancing != "");


                // [uniform]
                if (shaderReader.containsRoot(L"uniform"))
                {
                    const std::map<std::wstring, std::wstring>& uniformNodes(shaderReader.getNodes(L"uniform"));
                    std::for_each(uniformNodes.cbegin(), uniformNodes.cend(), [&](const std::pair<std::wstring, std::wstring> node)
                    {
                        // Camera
                        if (node.second == L"WORLDVIEWPROJECTION")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_WorldViewProjection)));
                        }
                        else if (node.second == L"WORLDVIEW")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_WorldView)));
                        }
                        else if (node.second == L"VIEWPROJECTION")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_ViewProjection)));
                        }
                        else if (node.second == L"VIEW")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_View)));
                        }
                        else if (node.second == L"WORLD")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_World)));
                        }
                        else if (node.second == L"WORLDPOSITION")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_WorldPosition)));
                        }

                        // Light
                        else if (node.second == L"AMBIENT_COLOR")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_AmbientColor)));
                        }
                        else if (node.second == L"DIRECTIONAL_COLOR")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_DirectionalColor)));
                        }
                        else if (node.second == L"DIRECTIONAL_DIRECTION")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_DirectionalDirection)));
                        }

                        // Shadow
                        else if (node.second == L"SHADOW_CASCADE_MATRIX0")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_ShadowCascadeMatrix0)));
                        }
                        else if (node.second == L"SHADOW_CASCADE_MATRIX1")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_ShadowCascadeMatrix1)));
                        }
                        else if (node.second == L"SHADOW_CASCADE_MATRIX2")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_ShadowCascadeMatrix2)));
                        }
                        else if (node.second == L"SHADOW_CASCADE_MATRIX3")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_ShadowCascadeMatrix3)));
                        }

                        else if (node.second == L"SHADOW_CASCADE0")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderSamplerId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_ShadowCascade0)));
                        }
                        else if (node.second == L"SHADOW_CASCADE1")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderSamplerId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_ShadowCascade1)));
                        }
                        else if (node.second == L"SHADOW_CASCADE2")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderSamplerId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_ShadowCascade2)));
                        }
                        else if (node.second == L"SHADOW_CASCADE3")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderSamplerId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_ShadowCascade3)));
                        }

                        // Skinning
                        else if (node.second == L"BONETRANSFORMS")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(std::string(node.first.cbegin(), node.first.cend())), gfx::ShaderVarType_BoneTransforms)));
                        }

                        // Texture
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
                            material.addVar(var);
                        }
                        else
                        {
                            std::wcout << "**Material Error**: unkown semantic " << node.second << "\n";
                        }
                    });
                }
            }
        }
        m_pAssetContainer->addAsset(path, material);
        return material;
    }
}

void MaterialLoader::setRenderSettings( const gfx::RenderSettings& settings )
{
    m_RenderSettings = settings;
}

} } //end namespace