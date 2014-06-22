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
//Created: 30/09/2011
#include "HappyPCH.h" 

#include "MaterialLoader.h"
#include "HappyNew.h"
#include "IniReader.h"
#include "HappyEngine.h"
#include "BufferLayout.h"
#include "Texture2D.h"
#include "ContentManager.h"
#include "Bone.h"
#include "ShaderVar.h"
#include "GlobalSettings.h"

namespace he {
namespace ct {

MaterialLoader::MaterialLoader()
{
}


MaterialLoader::~MaterialLoader()
{
    ResourceFactory<gfx::Material>::getInstance()->garbageCollect();
    m_AssetContainer.removeAllAssets();
}

gfx::BlendEquation blendEquationFromString(const he::String& str)
{
    if (str == "ADD")
    {
        return gfx::BlendEquation_Add;
    }
    else if (str == "SUBTRACT")
    {
        return gfx::BlendEquation_Subtract;
    }
    else if (str == "REVERSE_SUBTRACT" || str == "INVERSE_SUBTRACT")
    {
        return gfx::BlendEquation_ReverseSubtract;
    }
    else if (str == "MIN")
    {
        return gfx::BlendEquation_Min;
    }
    else if (str == "MAX")
    {
        return gfx::BlendEquation_Max;
    }
    else 
    {
        HE_ERROR("unknown blendEquationFromString: %s", str.c_str());
        return gfx::BlendEquation_Add;
    }
}
gfx::BlendFunc blendFuncFromString(const he::String& str)
{
    if (str == "ZERO")
    {
        return gfx::BlendFunc_Zero;
    }
    else if (str == "ONE")
    {
        return gfx::BlendFunc_One;
    }
    else if (str == "SRC_COLOR")
    {
        return gfx::BlendFunc_SrcColor;
    }
    else if (str == "INV_SRC_COLOR" || str == "ONE_MIN_SRC_COLOR")
    {
        return gfx::BlendFunc_OneMinusSrcColor;
    }
    else if (str == "DEST_COLOR")
    {
        return gfx::BlendFunc_DestColor;
    }
    else if (str == "INV_DEST_COLOR" || str == "ONE_MIN_DEST_COLOR")
    {
        return gfx::BlendFunc_OneMinusDestColor;
    }
    else if (str == "SRC_ALPHA")
    {
        return gfx::BlendFunc_SrcAlpha;
    }
    else if (str == "INV_SRC_ALPHA" || str == "ONE_MIN_SRC_ALPHA" )
    {
        return gfx::BlendFunc_OneMinusSrcAlpha;
    }
    else if (str == "DEST_ALPHA")
    {
        return gfx::BlendFunc_DestAlpha;
    }
    else if (str == "INV_DEST_ALPHA" || str == "ONE_MIN_DEST_ALPHA" )
    {
        return gfx::BlendFunc_OneMinusDestAlpha;
    }
    else if (str == "SRC_ALPHA_SAT")
    {
        return gfx::BlendFunc_SrcAlphaSaturate;
    }
    else 
    {
        HE_ERROR("unknown blendFuncFromString: %s", str.c_str());
        return gfx::BlendFunc_One;
    }
}

ObjectHandle MaterialLoader::load(const he::String& path)
{
    ResourceFactory<gfx::Material>* factory(ResourceFactory<gfx::Material>::getInstance());
    if (m_AssetContainer.isAssetPresent(path) && factory->isAlive(m_AssetContainer.getAsset(path)))
    {
        ObjectHandle material(m_AssetContainer.getAsset(path));
        factory->instantiate(material);
        return material;
    }
    else
    {
        io::IniReader reader;
        
        gfx::Material* material(factory->get(factory->create()));
        m_AssetContainer.addAsset(path, material->getHandle());
        material->setName(path);

        if (reader.open(path) == false)
        {
            HE_ERROR("Error loading material: %s", path.c_str());
            return material->getHandle();
        }
        
        if (reader.isOpen())
        {     
            const gfx::RenderSettings& settings(GlobalSettings::getInstance()->getRenderSettings());
            gfx::BufferLayout vertexLayout;
            // [Shader]
            {
                he::String file;
                file = reader.readString(L"Forward", L"shader", "");
                if (settings.enableDeferred)
                {
                    he::String temp = reader.readString(L"Deferred", L"shader", file);
                    if (temp != file)
                    {
                        file = temp;
                    }
                }

                io::IniReader shaderReader;
                if (shaderReader.open(CONTENT->getShaderFolderPath().str() + file) == false)
                { 
                    HE_ERROR("Error loading material shader: %s", path.c_str());
                    return material->getHandle();
                }

                he::ObjectList<he::String> shaderOutputs;

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
                                shaderOutputs[0] = he::String(p.first.cbegin(), p.first.cend());
                            else if (p.second == L"GBUFFER_SG")
                                shaderOutputs[1] = he::String(p.first.cbegin(), p.first.cend());
                            else if (p.second == L"GBUFFER_NORMALDEPTH")
                                shaderOutputs[2] = he::String(p.first.cbegin(), p.first.cend());
                            else
                                LOG(LogType_ProgrammerAssert, "unknow semantic");
                        });
                    }
                    else if (outNodes.size() == 2)
                    {
                        shaderOutputs.resize(2);
                        std::for_each(outNodes.cbegin(), outNodes.cend(), [&](const std::pair<std::wstring, std::wstring>& p)
                        {
                            if (p.second == L"GBUFFER_COLOR")
                                shaderOutputs[0] = he::String(p.first.cbegin(), p.first.cend());
                            else if (p.second == L"GBUFFER_NORMALDEPTH")
                                shaderOutputs[1] = he::String(p.first.cbegin(), p.first.cend());
                            else
                                LOG(LogType_ProgrammerAssert, "unknow semantic");
                        });
                    }
                }

                // [inPerVertex]
                gfx::ShaderLayout shaderLayout;
                uint32 count(0);
                uint32 offset(0);
                const std::map<std::wstring, std::wstring>& inNodes(shaderReader.getNodes(L"inPerVertex"));
                std::for_each(inNodes.cbegin(), inNodes.cend(), [&](const std::pair<std::wstring, std::wstring>& p)
                {
                    if (p.second == L"POSITION")
                    {
                        vertexLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Position, sizeof(vec3), offset));
                        offset += sizeof(vec3);
                    }
                    else if (p.second == L"TEXCOORD")
                    {
                        vertexLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec2, gfx::BufferElement::Usage_TextureCoordinate, sizeof(vec2), offset));
                        offset += sizeof(vec2);
                    }
                    else if (p.second == L"NORMAL")
                    {
                        vertexLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Normal, sizeof(vec3), offset));
                        offset += sizeof(vec3);
                    }
                    else if (p.second == L"TANGENT")
                    {
                        vertexLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Tangent, sizeof(vec3), offset));
                        offset += sizeof(vec3);
                    }
                    else if (p.second == L"BONEIDS")
                    {
                        vertexLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_BoneIDs, sizeof(vec4), offset));
                        offset += sizeof(vec4);
                    }
                    else if (p.second == L"BONEWEIGHTS")
                    {
                        HE_COMPILE_ASSERT(gfx::Bone::MAX_BONEWEIGHTS == 4, "layout incompatible");
                        vertexLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_BoneWeights, sizeof(vec4), offset));
                        offset += sizeof(vec4);
                    }
                    else
                    {
                        HE_ERROR("Material: unknown attribute %s", he::String(p.second.cbegin(), p.second.cend()).c_str());
                    } 
                    shaderLayout.addElement(gfx::ShaderLayoutElement(static_cast<uint32>(shaderLayout.getElements().size()), he::String(p.first.cbegin(), p.first.cend())));
                }); 

                gfx::BufferLayout instancingLayout;
                offset = 0;
                count = 0;
                if (shaderReader.containsRoot(L"inPerInstance"))
                {
                    const std::map<std::wstring, std::wstring>& inNodes(shaderReader.getNodes(L"inPerInstance"));
                    std::for_each(inNodes.cbegin(), inNodes.cend(), [&](const std::pair<std::wstring, std::wstring>& p)
                    {
                        if (p.second == L"MAT44")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_Instancing, sizeof(vec4), offset));
                            offset += sizeof(vec4);
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_Instancing, sizeof(vec4), offset));
                            offset += sizeof(vec4);
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_Instancing, sizeof(vec4), offset));
                            offset += sizeof(vec4);
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_Instancing, sizeof(vec4), offset));
                            offset += sizeof(vec4);
                        }
                        else if (p.second == L"FLOAT")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Float, gfx::BufferElement::Usage_Instancing, sizeof(float), offset));
                            offset += sizeof(float);
                        }
                        else if (p.second == L"VEC2")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec2, gfx::BufferElement::Usage_Instancing, sizeof(vec2), offset));
                            offset += sizeof(vec2);
                        }
                        else if (p.second == L"VEC3")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Instancing, sizeof(vec3), offset));
                            offset += sizeof(vec3);
                        }
                        else if (p.second == L"VEC4")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_Instancing, sizeof(vec4), offset));
                            offset += sizeof(vec4);
                        }
                        else if (p.second == L"INT")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Int, gfx::BufferElement::Usage_Instancing, sizeof(int), offset));
                            offset += sizeof(int);
                        }
                        else if (p.second == L"UINT")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_UInt, gfx::BufferElement::Usage_Instancing, sizeof(uint32), offset));
                            offset += sizeof(uint32);
                        }
                        else
                        {
                            HE_ERROR("Material: instancing unknown type %s", he::String(p.second.cbegin(), p.second.cend()).c_str());
                        }
                        shaderLayout.addElement(gfx::ShaderLayoutElement(static_cast<uint32>(shaderLayout.getElements().size()), he::String(p.first.cbegin(), p.first.cend())));  
                    });
                }

                // [Shader]
                gfx::Shader* pShader(ResourceFactory<gfx::Shader>::getInstance()->get(
                    CONTENT->loadShader(shaderReader.readString(L"Shader", L"vsPath", ""),
                                        shaderReader.readString(L"Shader", L"fsPath", ""),
                                        shaderLayout,
                                        shaderOutputs)));

                // [info]
                bool isBlended(false);
                gfx::BlendEquation blendEq(gfx::BlendEquation_Add);
                gfx::BlendFunc srcBlend(gfx::BlendFunc_One), destBlend(gfx::BlendFunc_Zero);
                bool post(true);
                bool cullFrontFace(false);
                if (reader.containsRoot(L"info"))
                {
                    isBlended = reader.readBool(L"info", L"blending", false);
                    if (isBlended)
                    {
                        blendEq = blendEquationFromString(reader.readString(L"info", L"blendFunc", "ADD"));
                        srcBlend = blendFuncFromString(reader.readString(L"info", L"srcBlend", "ONE"));
                        destBlend = blendFuncFromString(reader.readString(L"info", L"destBlend", "ZERO"));
                    }
                    post = reader.readBool(L"info", L"post", true);
                    cullFrontFace = reader.readBool(L"info", L"cullFrontFace", false);
                }

                material->setIsBlended(isBlended, blendEq, srcBlend, destBlend);
                material->setNoPost(!post);
                material->setCullFrontFace(cullFrontFace);
                material->setShader(pShader->getHandle(), vertexLayout, instancingLayout);


                // [uniform]
                if (shaderReader.containsRoot(L"uniform"))
                {
                    const std::map<std::wstring, std::wstring>& uniformNodes(shaderReader.getNodes(L"uniform"));
                    std::for_each(uniformNodes.cbegin(), uniformNodes.cend(), [&](const std::pair<std::wstring, std::wstring> node)
                    {
                        he::String name = he::String(node.first.cbegin(), node.first.cend());
                        // Camera
                        if (node.second == L"WORLDVIEWPROJECTION")
                        {
                            material->registerVar(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_WorldViewProjection));
                        }
                        else if (node.second == L"WORLDVIEW")
                        {
                            material->registerVar(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_WorldView));
                        }
                        else if (node.second == L"VIEWPROJECTION")
                        {
                            material->registerVar(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_ViewProjection));
                        }
                        else if (node.second == L"VIEW")
                        {
                            material->registerVar(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_View));
                        }
                        else if (node.second == L"WORLD")
                        {
                            material->registerVar(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_World));
                        }
                        else if (node.second == L"WORLDPOSITION")
                        {
                            material->registerVar(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_WorldPosition));
                        }

                        // Light
                        else if (node.second == L"AMBIENT_COLOR")
                        {
                            material->registerVar(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_AmbientColor));
                        }
                        else if (node.second == L"DIRECTIONAL_COLOR")
                        {
                            material->registerVar(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_DirectionalColor));
                        }
                        else if (node.second == L"DIRECTIONAL_DIRECTION")
                        {
                            material->registerVar(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_DirectionalDirection));
                        }

                        else if (node.second == L"NEARFAR")
                        {
                            material->registerVar(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_NearFar));
                        }

                        // Skinning
                        else if (node.second == L"BONETRANSFORMS")
                        {
                            material->registerVar(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_BoneTransforms));
                        }

                        // Texture
                        else if (node.second == L"TEXTURE2D")
                        {
                            const gfx::Texture2D* tex; 
                            vec4 testColorMap(reader.readVector4(L"TEXTURE2D", node.first, vec4(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN)));
                            if (testColorMap == vec4(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN))
                                tex = CONTENT->asyncLoadTexture2D(reader.readString(L"TEXTURE2D", node.first, ""));
                            else
                                tex = CONTENT->asyncMakeTexture2D(Color(testColorMap));

                            material->registerVar(
                                NEW gfx::ShaderUserVar<const gfx::Texture2D*>(
                                pShader->getShaderSamplerId(name), name, tex));
                            tex->release();
                        }
                        // Texture Cube
                        else if (node.second == L"TEXTURECUBE")
                        {
                            const gfx::TextureCube* tex( 
                                CONTENT->asyncLoadTextureCube(reader.readString(L"TEXTURECUBE", node.first, "")));

                            material->registerVar(
                                NEW gfx::ShaderUserVar<const gfx::TextureCube*>(
                                pShader->getShaderSamplerId(name), name, tex));
                            tex->release();
                        }

                        // User
                        else if (node.second == L"FLOAT")
                        {
                            material->registerVar(
                                NEW gfx::ShaderUserVar<float>(pShader->getShaderVarId(name), name, 
                                    reader.readFloat(L"FLOAT", node.first, 1.0f)));
                        }
                        else if (node.second == L"VEC2")
                        {
                            material->registerVar(
                                NEW gfx::ShaderUserVar<vec2>(pShader->getShaderVarId(name), name, 
                                    reader.readVector2(L"VEC2", node.first, vec2(1.0f, 1.0f)))
                                    );
                        }
                        else if (node.second == L"VEC3")
                        {
                            material->registerVar(
                                NEW gfx::ShaderUserVar<vec3>(pShader->getShaderVarId(name), name, 
                                reader.readVector3(L"VEC3", node.first, vec3(1.0f, 1.0f, 1.0f)))
                                );
                        }
                        else if (node.second == L"VEC4")
                        {
                            material->registerVar(
                                NEW gfx::ShaderUserVar<vec4>(pShader->getShaderVarId(name), name, 
                                reader.readVector4(L"VEC4", node.first, vec4(1.0f, 1.0f, 1.0f, 1.0f)))
                                );
                        }
                        else if (node.second == L"INT")
                        {
                            material->registerVar(
                                NEW gfx::ShaderUserVar<int>(pShader->getShaderVarId(name), name, 
                                reader.readInt(L"INT", node.first, 0))
                                );
                        }
                        else if (node.second == L"UINT")
                        {
                            material->registerVar(
                                NEW gfx::ShaderUserVar<uint32>(pShader->getShaderVarId(name), name, 
                                static_cast<uint32>(reader.readInt(L"UINT", node.first, 0)))
                                );
                        }
                        else
                        {
                            HE_ERROR("Material: unknown semantic %s", he::String(node.second.cbegin(), node.second.cend()).c_str());
                        }
                    });
                }
                pShader->release();
            }
        }
        return material->getHandle();
    }
}

} } //end namespace