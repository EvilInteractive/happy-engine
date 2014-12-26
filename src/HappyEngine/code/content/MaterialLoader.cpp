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
//Redone: 22/06/2013
#include "HappyPCH.h" 

#include "MaterialLoader.h"
#include "Texture2D.h"
#include "ContentManager.h"
#include "Bone.h"
#include "Material.h"
#include "ShaderLayout.h"
#include "GlobalStringTable.h"
#include "GlobalSettings.h"
#include "ShaderEnums.h"
#include "JsonFileReader.h"
#include "Shader.h"

namespace 
{
he::FixedString blendEquationToString(const he::gfx::BlendEquation eq)
{
    he::FixedString result(he::HEFS::strAdd);
    switch (eq)
    {
    case he::gfx::BlendEquation_Add: result = he::HEFS::strAdd; break;
    case he::gfx::BlendEquation_Subtract: result = he::HEFS::strSubtract; break;
    case he::gfx::BlendEquation_ReverseSubtract: result = he::HEFS::strReverseSubtract; break;
    case he::gfx::BlendEquation_Min: result = he::HEFS::strMin; break;
    case he::gfx::BlendEquation_Max: result = he::HEFS::strMax; break;
    default: LOG(he::LogType_ProgrammerAssert, "Unknown BlendEquation %d when converting to string", eq); break;
    }
    return result;
}
he::gfx::BlendEquation blendEquationFromString(const he::FixedString& str)
{
    if (str == he::HEFS::strAdd)
    {
        return he::gfx::BlendEquation_Add;
    }
    else if (str == he::HEFS::strSubtract)
    {
        return he::gfx::BlendEquation_Subtract;
    }
    else if (str == he::HEFS::strReverseSubtract)
    {
        return he::gfx::BlendEquation_ReverseSubtract;
    }
    else if (str == he::HEFS::strMin)
    {
        return he::gfx::BlendEquation_Min;
    }
    else if (str == he::HEFS::strMax)
    {
        return he::gfx::BlendEquation_Max;
    }
    else 
    {
        HE_ERROR("Unknown blendEquationFromString: %s", str.c_str());
        return he::gfx::BlendEquation_Add;
    }
}
he::FixedString blendFuncToString(const he::gfx::BlendFunc func)
{
    he::FixedString result(he::HEFS::strOne);
    switch (func)
    {
    case he::gfx::BlendFunc_Zero: result = he::HEFS::strZero; break;
    case he::gfx::BlendFunc_One: result = he::HEFS::strOne; break;
    case he::gfx::BlendFunc_SrcColor: result = he::HEFS::strSrcColor; break;
    case he::gfx::BlendFunc_OneMinusSrcColor: result = he::HEFS::strOneMinSrcColor; break;
    case he::gfx::BlendFunc_DestColor: result = he::HEFS::strDestColor; break;
    case he::gfx::BlendFunc_OneMinusDestColor: result = he::HEFS::strOneMinDestColor; break;
    case he::gfx::BlendFunc_SrcAlpha: result = he::HEFS::strSrcAlpha; break;
    case he::gfx::BlendFunc_OneMinusSrcAlpha: result = he::HEFS::strOneMinSrcAlpha; break;
    case he::gfx::BlendFunc_DestAlpha: result = he::HEFS::strDestAlpha; break;
    case he::gfx::BlendFunc_OneMinusDestAlpha: result = he::HEFS::strOneMinDestAlpha; break;
    case he::gfx::BlendFunc_SrcAlphaSaturate: result = he::HEFS::strSrcAlphaSaturate; break;
    default: LOG(he::LogType_ProgrammerAssert, "Unknown BlendFunc %d when converting to string", func); break;
    }
    return result;
}
he::gfx::BlendFunc blendFuncFromString(const he::FixedString& str)
{
    if (str == he::HEFS::strZero)
    {
        return he::gfx::BlendFunc_Zero;
    }
    else if (str == he::HEFS::strOne)
    {
        return he::gfx::BlendFunc_One;
    }
    else if (str == he::HEFS::strSrcColor)
    {
        return he::gfx::BlendFunc_SrcColor;
    }
    else if (str == he::HEFS::strOneMinSrcColor)
    {
        return he::gfx::BlendFunc_OneMinusSrcColor;
    }
    else if (str == he::HEFS::strDestColor)
    {
        return he::gfx::BlendFunc_DestColor;
    }
    else if (str == he::HEFS::strOneMinDestColor)
    {
        return he::gfx::BlendFunc_OneMinusDestColor;
    }
    else if (str == he::HEFS::strSrcAlpha)
    {
        return he::gfx::BlendFunc_SrcAlpha;
    }
    else if (str == he::HEFS::strOneMinSrcAlpha )
    {
        return he::gfx::BlendFunc_OneMinusSrcAlpha;
    }
    else if (str == he::HEFS::strDestAlpha)
    {
        return he::gfx::BlendFunc_DestAlpha;
    }
    else if (str == he::HEFS::strOneMinDestAlpha )
    {
        return he::gfx::BlendFunc_OneMinusDestAlpha;
    }
    else if (str == he::HEFS::strSrcAlphaSaturate)
    {
        return he::gfx::BlendFunc_SrcAlphaSaturate;
    }
    else 
    {
        LOG(he::LogType_ProgrammerAssert, "Unknown blendFuncFromString: %s", str.c_str());
        return he::gfx::BlendFunc_One;
    }
}
}

namespace he {
namespace ct {

//////////////////////////////////////////////////////////////////////////
MaterialDesc::MaterialDescShader::MaterialDescShader() 
    : m_FragmentShader("")
    , m_VertexShader("")
    , m_GeometryShader("")
{
}

void MaterialDesc::MaterialDescShader::visit( he::io::StructuredVisitor* const visitor )
{
    m_FragmentShader.visit(HEFS::strFragmentShader, visitor);
    m_VertexShader.visit(HEFS::strVertexShader, visitor);
    m_GeometryShader.visit(HEFS::strGeometryShader, visitor);

    m_Defines.m_Override = visitor->visitList(HEFS::strDefines, m_Defines.m_Value);
    m_OutputLayout.m_Override = visitor->visitList(HEFS::strOutputLayout, m_OutputLayout.m_Value);
}

MaterialDesc::MaterialDescShader& MaterialDesc::MaterialDescShader::operator=( MaterialDescShader&& other )
{
    m_FragmentShader = std::move(other.m_FragmentShader);
    m_VertexShader = std::move(other.m_VertexShader);
    m_GeometryShader = std::move(other.m_GeometryShader);
    m_Defines = std::move(other.m_Defines);
    m_OutputLayout = std::move(other.m_OutputLayout);
    return *this;
}

//////////////////////////////////////////////////////////////////////////
MaterialDesc::MaterialDescOptions::MaterialDescOptions()
    : m_IsBlended(false)
    , m_NoPost(false)
    , m_CullFrontFace(false)
    , m_DepthRead(true)
    , m_DepthWrite(true)
    , m_CastShadow(true)
    , m_BlendEquation(gfx::BlendEquation_Add)
    , m_SourceBlend(gfx::BlendFunc_One)
    , m_DestBlend(gfx::BlendFunc_Zero)
{

}

void MaterialDesc::MaterialDescOptions::visit( he::io::StructuredVisitor* const visitor )
{
    m_IsBlended.visit(HEFS::strIsBlended, visitor);

    m_BlendEquation.visitCasted<he::FixedString>(HEFS::strBlendEquation, visitor, ::blendEquationToString, ::blendEquationFromString);
    m_SourceBlend.visitCasted<he::FixedString>(HEFS::strSourceBlend, visitor, ::blendFuncToString, ::blendFuncFromString);
    m_DestBlend.visitCasted<he::FixedString>(HEFS::strDestBlend, visitor, ::blendFuncToString, ::blendFuncFromString);

    m_NoPost.visit(HEFS::strNoPost, visitor);
    m_CullFrontFace.visit(HEFS::strCullFrontFace, visitor);
    m_DepthRead.visit(HEFS::strDepthRead, visitor);
    m_DepthWrite.visit(HEFS::strDepthWrite, visitor);
    m_CastShadow.visit(HEFS::strCastShadow, visitor);

    visitor->visitNameValueList(HEFS::strParameters, m_Params);
}

MaterialDesc::MaterialDescOptions& MaterialDesc::MaterialDescOptions::operator=( MaterialDescOptions&& other )
{
    m_IsBlended     = std::move(other.m_IsBlended);
    m_NoPost        = std::move(other.m_NoPost);
    m_CullFrontFace = std::move(other.m_CullFrontFace);
    m_DepthRead     = std::move(other.m_DepthRead);
    m_DepthWrite    = std::move(other.m_DepthWrite);
    m_CastShadow    = std::move(other.m_CastShadow);
    m_BlendEquation = std::move(other.m_BlendEquation);
    m_SourceBlend   = std::move(other.m_SourceBlend);
    m_DestBlend     = std::move(other.m_DestBlend);
    m_Params        = std::move(other.m_Params);
    return *this;
}

//////////////////////////////////////////////////////////////////////////
MaterialDesc::MaterialDesc()
: m_Inherits("")
{

}

//////////////////////////////////////////////////////////////////////////
void MaterialDescStack::visit( he::io::StructuredVisitor* const visitor )
{
    size_t index(m_Stack.size());
    m_Stack.resize(index + 1);
    m_Stack[index].visit(visitor);
}

void MaterialDesc::visit( he::io::StructuredVisitor* const visitor )
{
    visitor->visit(HEFS::strInherit, m_Inherits);
    if (visitor->enterNode(HEFS::strNormal))
    {
        if (visitor->enterNode(HEFS::strNormal))
        {
            m_Shader[gfx::eShaderPassType_Normal][gfx::eShaderRenderType_Normal].visit(visitor);
            visitor->exitNode(HEFS::strNormal);
        }
        if (visitor->enterNode(HEFS::strSkinned))
        {
            m_Shader[gfx::eShaderPassType_Normal][gfx::eShaderRenderType_Skinned].visit(visitor);
            visitor->exitNode(HEFS::strSkinned);
        }
        if (visitor->enterNode(HEFS::strInstanced))
        {
            m_Shader[gfx::eShaderPassType_Normal][gfx::eShaderRenderType_Instanced].visit(visitor);
            visitor->exitNode(HEFS::strInstanced);
        }
        visitor->exitNode(HEFS::strNormal);
    }
    if (visitor->enterNode(HEFS::strShadow))
    {
        if (visitor->enterNode(HEFS::strNormal))
        {
            m_Shader[gfx::eShaderPassType_Shadow][gfx::eShaderRenderType_Normal].visit(visitor);
            visitor->exitNode(HEFS::strNormal);
        }
        if (visitor->enterNode(HEFS::strSkinned))
        {
            m_Shader[gfx::eShaderPassType_Shadow][gfx::eShaderRenderType_Skinned].visit(visitor);
            visitor->exitNode(HEFS::strSkinned);
        }
        if (visitor->enterNode(HEFS::strInstanced))
        {
            m_Shader[gfx::eShaderPassType_Shadow][gfx::eShaderRenderType_Instanced].visit(visitor);
            visitor->exitNode(HEFS::strInstanced);
        }
        visitor->exitNode(HEFS::strShadow);
    }
    if (visitor->enterNode(HEFS::strOptions))
    {
        m_Options.visit(visitor);
        visitor->exitNode(HEFS::strOptions);
    }
}

MaterialDesc& MaterialDesc::operator=( MaterialDesc&& other )
{
    m_Inherits = std::move(other.m_Inherits);
    m_Options = std::move(other.m_Options);
    for (size_t pass(0); pass < gfx::eShaderPassType_MAX; ++pass)
    {
        for (size_t rtype(0); rtype < gfx::eShaderRenderType_MAX; ++rtype)
        {
            m_Shader[pass][rtype] = std::move(other.m_Shader[pass][rtype]);
        }
    }
    return *this;
}

bool MaterialDescStack::load( const he::String& asset )
{
    he::String currentAsset(asset);
    io::JsonFileReader reader;
    do 
    {
        if (reader.open(CONTENT->getMaterialFolderPath().append(currentAsset)) == false)
        {
            HE_ERROR("Error loading material: %s, Could not open file!", currentAsset.c_str());
            return false;
        }
        else
        {
            visit(&reader);
            reader.close();

            currentAsset = m_Stack[m_Stack.size() - 1].m_Inherits;
        }
    } while (currentAsset.empty() == false);

    return true;
}

namespace
{
template<typename T> 
void copyIfOverride(const MaterialDesc::MaterialDescParam<T>& from, MaterialDesc::MaterialDescParam<T>& inoutTo)
{
    if (from.m_Override)
        inoutTo.m_Value = from.m_Value;
}
}

void MaterialDescStack::collapse( MaterialDesc& outDesc )
{
    outDesc = std::move(m_Stack.back());
    m_Stack.pop();
    m_Stack.rForEach([&outDesc](MaterialDesc& desc)
    {
        // Shader
        for (size_t pass(0); pass < gfx::eShaderPassType_MAX; ++pass)
        {
            for (size_t rtype(0); rtype < gfx::eShaderRenderType_MAX; ++rtype)
            {
                MaterialDesc::MaterialDescShader& outShaderDesc(outDesc.m_Shader[pass][rtype]);
                MaterialDesc::MaterialDescShader& overrideShaderDesc(desc.m_Shader[pass][rtype]);
                copyIfOverride(overrideShaderDesc.m_FragmentShader, outShaderDesc.m_FragmentShader);
                copyIfOverride(overrideShaderDesc.m_VertexShader, outShaderDesc.m_VertexShader);
                copyIfOverride(overrideShaderDesc.m_GeometryShader, outShaderDesc.m_GeometryShader);
                if (overrideShaderDesc.m_Defines.m_Override)
                    outShaderDesc.m_Defines = std::move(overrideShaderDesc.m_Defines);
                if (overrideShaderDesc.m_OutputLayout.m_Override)
                    outShaderDesc.m_OutputLayout = std::move(overrideShaderDesc.m_OutputLayout);
            }
        }

        // Options
        copyIfOverride(desc.m_Options.m_IsBlended, outDesc.m_Options.m_IsBlended);
        copyIfOverride(desc.m_Options.m_NoPost, outDesc.m_Options.m_NoPost);
        copyIfOverride(desc.m_Options.m_CullFrontFace, outDesc.m_Options.m_CullFrontFace);
        copyIfOverride(desc.m_Options.m_DepthRead, outDesc.m_Options.m_DepthRead);
        copyIfOverride(desc.m_Options.m_DepthWrite, outDesc.m_Options.m_DepthWrite);
        copyIfOverride(desc.m_Options.m_CastShadow, outDesc.m_Options.m_CastShadow);
        copyIfOverride(desc.m_Options.m_BlendEquation, outDesc.m_Options.m_BlendEquation);
        copyIfOverride(desc.m_Options.m_SourceBlend, outDesc.m_Options.m_SourceBlend);
        copyIfOverride(desc.m_Options.m_DestBlend, outDesc.m_Options.m_DestBlend);
        desc.m_Options.m_Params.forEach([&outDesc](const NameValuePair<he::String>& pair)
        {
            size_t index(0);
            if (outDesc.m_Options.m_Params.find_if([&pair](const NameValuePair<he::String>& otherPair){ return pair.m_Name == otherPair.m_Name; }, index))
            {
                outDesc.m_Options.m_Params[index].m_Value = pair.m_Value;
            }
            else
            {
                outDesc.m_Options.m_Params.add(pair);
            }
        });
    });
    m_Stack.clear();
}

MaterialLoader::MaterialLoader()
{
}

MaterialLoader::~MaterialLoader()
{
    ResourceFactory<gfx::Material>::getInstance()->garbageCollect();
    m_AssetContainer.removeAllAssets();
}

gfx::Material* MaterialLoader::load(const he::String& asset)
{
    ResourceFactory<gfx::Material>* factory(ResourceFactory<gfx::Material>::getInstance());
    if (m_AssetContainer.isAssetPresent(asset) && factory->isAlive(m_AssetContainer.getAsset(asset)))
    {
        const ObjectHandle material(m_AssetContainer.getAsset(asset));
        factory->instantiate(material);
        return factory->get(material);
    }
    else
    {
        gfx::Material* const material(factory->get(factory->create()));
        m_AssetContainer.addAsset(asset, material->getHandle());
        material->setName(asset);


        MaterialDescStack matDescStack;
        if (matDescStack.load(asset))
        {
            MaterialDesc desc;
            matDescStack.collapse(desc);

            material->setIsBlended(desc.m_Options.m_IsBlended.m_Value, desc.m_Options.m_BlendEquation.m_Value, 
                                   desc.m_Options.m_SourceBlend.m_Value, desc.m_Options.m_DestBlend.m_Value);
            material->setNoPost(desc.m_Options.m_NoPost.m_Value);
            material->setCullFrontFace(desc.m_Options.m_CullFrontFace.m_Value);
            material->setDepthReadEnabled(desc.m_Options.m_DepthRead.m_Value);
            material->setDepthWriteEnabled(desc.m_Options.m_DepthWrite.m_Value);
            material->setDefaultParams(std::move(desc.m_Options.m_Params));
            for (size_t pass(0); pass < gfx::eShaderPassType_MAX; ++pass)
            {
                for (size_t rtype(0); rtype < gfx::eShaderRenderType_MAX; ++rtype)
                {
                    const MaterialDesc::MaterialDescShader& shaderDesc(desc.m_Shader[pass][rtype]);
                    if (shaderDesc.m_FragmentShader.m_Value.empty() == false && shaderDesc.m_VertexShader.m_Value.empty() == false)
                    {
                        gfx::Shader* const shader(CONTENT->loadShader(
                            shaderDesc.m_VertexShader.m_Value, shaderDesc.m_GeometryShader.m_Value, shaderDesc.m_FragmentShader.m_Value, 
                            &shaderDesc.m_Defines.m_Value, &shaderDesc.m_OutputLayout.m_Value));
                        material->setShader(checked_numcast<gfx::EShaderPassType>(pass), checked_numcast<gfx::EShaderRenderType>(rtype), shader);
                        shader->release();
                    }
                }
            }
            material->init();
        }
        return material;
    }
}

} } //end namespace
