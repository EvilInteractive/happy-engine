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
//Created: 30/09/2011
#include "HappyPCH.h" 

#include "MaterialLoader.h"
#include "IniReader.h"
#include "Texture2D.h"
#include "ContentManager.h"
#include "Bone.h"
#include "ShaderVar.h"
#include "Material.h"
#include "ShaderLayout.h"
#include "GlobalStringTable.h"
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
            gfx::ShaderLayout materialLayout;
            // [Shader]
            {
                he::String file;
                file = reader.readString("Forward", "shader", "");
                if (settings.enableDeferred)
                {
                    he::String temp = reader.readString("Deferred", "shader", file);
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
                if (shaderReader.containsRoot("out"))
                {
                    const std::map<he::String, he::String>& outNodes(shaderReader.getNodes("out"));
                    if (outNodes.size() == 3)
                    {
                        shaderOutputs.resize(3);
                        std::for_each(outNodes.cbegin(), outNodes.cend(), [&](const std::pair<he::String, he::String>& p)
                        {
                            if (p.second == "GBUFFER_COLOR")
                                shaderOutputs[0] = p.first;
                            else if (p.second == "GBUFFER_SG")
                                shaderOutputs[1] = p.first;
                            else if (p.second == "GBUFFER_NORMALDEPTH")
                                shaderOutputs[2] = p.first;
                            else
                                LOG(LogType_ProgrammerAssert, "unknow semantic");
                        });
                    }
                    else if (outNodes.size() == 2)
                    {
                        shaderOutputs.resize(2);
                        std::for_each(outNodes.cbegin(), outNodes.cend(), [&](const std::pair<he::String, he::String>& p)
                        {
                            if (p.second == "GBUFFER_COLOR")
                                shaderOutputs[0] = p.first;
                            else if (p.second == "GBUFFER_NORMALDEPTH")
                                shaderOutputs[1] = p.first;
                            else
                                LOG(LogType_ProgrammerAssert, "unknow semantic");
                        });
                    }
                }

                // [inPerVertex]
                gfx::ShaderLayout shaderLayout;
                const std::map<he::String, he::String>& inNodes(shaderReader.getNodes("inPerVertex"));
                std::for_each(inNodes.cbegin(), inNodes.cend(), [&](const std::pair<he::String, he::String>& p)
                {
                    if (p.second == "POSITION")
                    {
                        shaderLayout.addElement(gfx::ShaderLayoutElement(p.first, gfx::BufferElement::Usage_Position));
                    }
                    else if (p.second == "TEXCOORD")
                    {
                        shaderLayout.addElement(gfx::ShaderLayoutElement(p.first, gfx::BufferElement::Usage_TextureCoordinate));
                    }
                    else if (p.second == "NORMAL")
                    {
                        shaderLayout.addElement(gfx::ShaderLayoutElement(p.first, gfx::BufferElement::Usage_Normal));
                    }
                    else if (p.second == "TANGENT")
                    {
                        shaderLayout.addElement(gfx::ShaderLayoutElement(p.first, gfx::BufferElement::Usage_Tangent));
                    }
                    else if (p.second == "BONEIDS")
                    {
                        shaderLayout.addElement(gfx::ShaderLayoutElement(p.first, gfx::BufferElement::Usage_BoneIDs));
                    }
                    else if (p.second == "BONEWEIGHTS")
                    {
                        shaderLayout.addElement(gfx::ShaderLayoutElement(p.first, gfx::BufferElement::Usage_BoneWeights));
                    }
                    else
                    {
                        HE_ERROR("Material: unknown attribute %s", p.second.c_str());
                    } 
                }); 

                gfx::BufferLayout instancingLayout;
                /*if (shaderReader.containsRoot("inPerInstance"))
                {
                    const std::map<he::String, he::String>& inNodes(shaderReader.getNodes("inPerInstance"));
                    std::for_each(inNodes.cbegin(), inNodes.cend(), [&](const std::pair<he::String, he::String>& p)
                    {
                        if (p.second == "MAT44")
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
                        else if (p.second == "FLOAT")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Float, gfx::BufferElement::Usage_Instancing, sizeof(float), offset));
                            offset += sizeof(float);
                        }
                        else if (p.second == "VEC2")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec2, gfx::BufferElement::Usage_Instancing, sizeof(vec2), offset));
                            offset += sizeof(vec2);
                        }
                        else if (p.second == "VEC3")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Instancing, sizeof(vec3), offset));
                            offset += sizeof(vec3);
                        }
                        else if (p.second == "VEC4")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_Instancing, sizeof(vec4), offset));
                            offset += sizeof(vec4);
                        }
                        else if (p.second == "INT")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Int, gfx::BufferElement::Usage_Instancing, sizeof(int), offset));
                            offset += sizeof(int);
                        }
                        else if (p.second == "UINT")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_UInt, gfx::BufferElement::Usage_Instancing, sizeof(uint32), offset));
                            offset += sizeof(uint32);
                        }
                        else
                        {
                            HE_ERROR("Material: instancing unknown type %s", he::String(p.second.cbegin(), p.second.cend()).c_str());
                        }
                        shaderLayout.addElement(gfx::ShaderLayoutElement(static_cast<uint32>(shaderLayout.getElements().size()), p.first));  
                    });
                }*/

                // [Shader]
                gfx::Shader* shader(ResourceFactory<gfx::Shader>::getInstance()->get(
                    CONTENT->loadShader(shaderReader.readString("Shader", "vsPath", ""),
                                        shaderReader.readString("Shader", "fsPath", ""),
                                        shaderLayout,
                                        shaderOutputs)));

                // [info]
                bool isBlended(false);
                gfx::BlendEquation blendEq(gfx::BlendEquation_Add);
                gfx::BlendFunc srcBlend(gfx::BlendFunc_One), destBlend(gfx::BlendFunc_Zero);
                bool post(true);
                bool cullFrontFace(false);
                if (reader.containsRoot("info"))
                {
                    isBlended = reader.readBool("info", "blending", false);
                    if (isBlended)
                    {
                        blendEq = blendEquationFromString(reader.readString("info", "blendFunc", "ADD"));
                        srcBlend = blendFuncFromString(reader.readString("info", "srcBlend", "ONE"));
                        destBlend = blendFuncFromString(reader.readString("info", "destBlend", "ZERO"));
                    }
                    post = reader.readBool("info", "post", true);
                    cullFrontFace = reader.readBool("info", "cullFrontFace", false);
                }

                material->setIsBlended(isBlended, blendEq, srcBlend, destBlend);
                material->setNoPost(!post);
                material->setCullFrontFace(cullFrontFace);
                material->setNormalShader(shader);


                // [uniform]
                if (shaderReader.containsRoot("uniform"))
                {
                    const std::map<he::String, he::String>& uniformNodes(shaderReader.getNodes("uniform"));
                    std::for_each(uniformNodes.cbegin(), uniformNodes.cend(), [&](const std::pair<he::String, he::String> node)
                    {
                        const he::FixedString name(he::GlobalStringTable::getInstance()->add(node.first.c_str(), node.first.size()));
                        // Camera
                        if (node.second == "WORLDVIEWPROJECTION")
                        {
                            material->registerSpecificVar(gfx::eShaderType_Normal,
                                NEW gfx::ShaderGlobalVar(shader->getShaderVarId(name), name, gfx::ShaderVarType_WorldViewProjection));
                        }
                        else if (node.second == "WORLDVIEW")
                        {
                            material->registerSpecificVar(gfx::eShaderType_Normal,
                                NEW gfx::ShaderGlobalVar(shader->getShaderVarId(name), name, gfx::ShaderVarType_WorldView));
                        }
                        else if (node.second == "VIEWPROJECTION")
                        {
                            material->registerSpecificVar(gfx::eShaderType_Normal,
                                NEW gfx::ShaderGlobalVar(shader->getShaderVarId(name), name, gfx::ShaderVarType_ViewProjection));
                        }
                        else if (node.second == "VIEW")
                        {
                            material->registerSpecificVar(gfx::eShaderType_Normal,
                                NEW gfx::ShaderGlobalVar(shader->getShaderVarId(name), name, gfx::ShaderVarType_View));
                        }
                        else if (node.second == "WORLD")
                        {
                            material->registerSpecificVar(gfx::eShaderType_Normal,
                                NEW gfx::ShaderGlobalVar(shader->getShaderVarId(name), name, gfx::ShaderVarType_World));
                        }
                        else if (node.second == "WORLDPOSITION")
                        {
                            material->registerCommonVar(
                                NEW gfx::ShaderGlobalVar(shader->getShaderVarId(name), name, gfx::ShaderVarType_WorldPosition));
                        }

                        // Light
                        else if (node.second == "AMBIENT_COLOR")
                        {
                            material->registerCommonVar(
                                NEW gfx::ShaderGlobalVar(shader->getShaderVarId(name), name, gfx::ShaderVarType_AmbientColor));
                        }
                        else if (node.second == "DIRECTIONAL_COLOR")
                        {
                            material->registerCommonVar(
                                NEW gfx::ShaderGlobalVar(shader->getShaderVarId(name), name, gfx::ShaderVarType_DirectionalColor));
                        }
                        else if (node.second == "DIRECTIONAL_DIRECTION")
                        {
                            material->registerCommonVar(
                                NEW gfx::ShaderGlobalVar(shader->getShaderVarId(name), name, gfx::ShaderVarType_DirectionalDirection));
                        }

                        else if (node.second == "NEARFAR")
                        {
                            material->registerCommonVar(
                                NEW gfx::ShaderGlobalVar(shader->getShaderVarId(name), name, gfx::ShaderVarType_NearFar));
                        }

                        // Skinning
                        else if (node.second == "BONETRANSFORMS")
                        {
                            material->registerSpecificVar(gfx::eShaderType_Skinned,
                                NEW gfx::ShaderGlobalVar(shader->getShaderVarId(name), name, gfx::ShaderVarType_BoneTransforms));
                        }

                        // Texture
                        else if (node.second == "TEXTURE2D")
                        {
                            const gfx::Texture2D* tex; 
                            vec4 testColorMap(reader.readVector4("TEXTURE2D", node.first, vec4(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN)));
                            if (testColorMap == vec4(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN))
                                tex = CONTENT->asyncLoadTexture2D(reader.readString("TEXTURE2D", node.first, ""));
                            else
                                tex = CONTENT->asyncMakeTexture2D(Color(testColorMap));

                            material->registerCommonVar(
                                NEW gfx::ShaderUserVar<const gfx::Texture2D*>(
                                shader->getShaderSamplerId(name), name, tex));
                            tex->release();
                        }
                        // Texture Cube
                        else if (node.second == "TEXTURECUBE")
                        {
                            const gfx::TextureCube* tex( 
                                CONTENT->asyncLoadTextureCube(reader.readString("TEXTURECUBE", node.first, "")));

                            material->registerCommonVar(
                                NEW gfx::ShaderUserVar<const gfx::TextureCube*>(
                                shader->getShaderSamplerId(name), name, tex));
                            tex->release();
                        }

                        // User
                        else if (node.second == "FLOAT")
                        {
                            material->registerCommonVar(
                                NEW gfx::ShaderUserVar<float>(shader->getShaderVarId(name), name, 
                                    reader.readFloat("FLOAT", node.first, 1.0f)));
                        }
                        else if (node.second == "VEC2")
                        {
                            material->registerCommonVar(
                                NEW gfx::ShaderUserVar<vec2>(shader->getShaderVarId(name), name, 
                                    reader.readVector2("VEC2", node.first, vec2(1.0f, 1.0f)))
                                    );
                        }
                        else if (node.second == "VEC3")
                        {
                            material->registerCommonVar(
                                NEW gfx::ShaderUserVar<vec3>(shader->getShaderVarId(name), name, 
                                reader.readVector3("VEC3", node.first, vec3(1.0f, 1.0f, 1.0f)))
                                );
                        }
                        else if (node.second == "VEC4")
                        {
                            material->registerCommonVar(
                                NEW gfx::ShaderUserVar<vec4>(shader->getShaderVarId(name), name, 
                                reader.readVector4("VEC4", node.first, vec4(1.0f, 1.0f, 1.0f, 1.0f)))
                                );
                        }
                        else if (node.second == "INT")
                        {
                            material->registerCommonVar(
                                NEW gfx::ShaderUserVar<int>(shader->getShaderVarId(name), name, 
                                reader.readInt("INT", node.first, 0))
                                );
                        }
                        else if (node.second == "UINT")
                        {
                            material->registerCommonVar(
                                NEW gfx::ShaderUserVar<uint32>(shader->getShaderVarId(name), name, 
                                static_cast<uint32>(reader.readInt("UINT", node.first, 0)))
                                );
                        }
                        else
                        {
                            HE_ERROR("Material: unknown semantic %s", he::String(node.second.cbegin(), node.second.cend()).c_str());
                        }
                    });
                }
                shader->release();
            }
        }
        material->setLoaded();
        return material->getHandle();
    }
}

} } //end namespace
