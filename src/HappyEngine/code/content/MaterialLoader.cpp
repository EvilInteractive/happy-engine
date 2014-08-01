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

MaterialDesc::MaterialDesc()
    : m_FragmentShader("")
    , m_VertexShader("")
    , m_IsBlended(false)
    , m_NoPost(false)
    , m_CullFrontFace(false)
    , m_DepthRead(true)
    , m_DepthWrite(true)
    , m_BlendEquation(gfx::BlendEquation_Add)
    , m_SourceBlend(gfx::BlendFunc_One)
    , m_DestBlend(gfx::BlendFunc_Zero)
{

}

void MaterialDesc::visit( he::io::StructuredVisitor* const visitor )
{
    visitor->visit(HEFS::strFragmentShader, m_FragmentShader);
    visitor->visit(HEFS::strVertexShader, m_VertexShader);
    visitor->visit(HEFS::strIsBlended, m_IsBlended);
    visitor->visit(HEFS::strNoPost, m_NoPost);
    visitor->visit(HEFS::strCullFrontFace, m_CullFrontFace);
    visitor->visit(HEFS::strDepthRead, m_DepthRead);
    visitor->visit(HEFS::strDepthWrite, m_DepthWrite);
    visitor->visitCasted<he::FixedString, gfx::BlendEquation>(HEFS::strBlendEquation, m_BlendEquation, NULL, ::blendEquationToString, ::blendEquationFromString);
    visitor->visitCasted<he::FixedString, gfx::BlendFunc>(HEFS::strSourceBlend, m_SourceBlend, NULL, ::blendFuncToString, ::blendFuncFromString);
    visitor->visitCasted<he::FixedString, gfx::BlendFunc>(HEFS::strDestBlend, m_DestBlend, NULL, ::blendFuncToString, ::blendFuncFromString);
    visitor->visitList(HEFS::strDefines, m_Defines);
    visitor->visitList(HEFS::strOutputLayout, m_OutputLayout);
}

MaterialLoader::MaterialLoader()
{
}

MaterialLoader::~MaterialLoader()
{
    ResourceFactory<gfx::Material>::getInstance()->garbageCollect();
    m_AssetContainer.removeAllAssets();
}

gfx::Material* MaterialLoader::load(const he::Path& path)
{
    ResourceFactory<gfx::Material>* factory(ResourceFactory<gfx::Material>::getInstance());
    if (m_AssetContainer.isAssetPresent(path.str()) && factory->isAlive(m_AssetContainer.getAsset(path.str())))
    {
        const ObjectHandle material(m_AssetContainer.getAsset(path.str()));
        factory->instantiate(material);
        return factory->get(material);
    }
    else
    {
        gfx::Material* const material(factory->get(factory->create()));
        m_AssetContainer.addAsset(path.str(), material->getHandle());
        material->setName(path.getFileName());

        io::JsonFileReader reader;

        if (reader.open(path) == false)
        {
            HE_ERROR("Error loading material: %s, Could not open file!", path.str().c_str());
            material->setLoaded(eLoadResult_Failed);
            return material;
        }
        else
        {
            MaterialDesc desc;
            desc.visit(&reader);
            
            material->setIsBlended(desc.m_IsBlended, desc.m_BlendEquation, desc.m_SourceBlend, desc.m_DestBlend);
            material->setNoPost(desc.m_NoPost);
            material->setCullFrontFace(desc.m_CullFrontFace);
            material->setDepthReadEnabled(desc.m_DepthRead);
            material->setDepthWriteEnabled(desc.m_DepthWrite);
            if (desc.m_FragmentShader.empty() == false && desc.m_VertexShader.empty() == false)
            {
                gfx::Shader* const shader(CONTENT->loadShader(desc.m_VertexShader, desc.m_FragmentShader, &desc.m_Defines, &desc.m_OutputLayout));
                material->setNormalShader(shader);
                material->init();
            }
            else
            {
                material->setLoaded(eLoadResult_Failed);
                HE_ERROR("Material %s has no shader!", path.str().c_str());
            }

            reader.close();
        }
        return material;
    }
}

} } //end namespace
