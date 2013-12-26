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

#include "ShaderLoader.h"
#include "IniReader.h"
#include "HappyEngine.h"
#include "BufferLayout.h"
#include "Shader.h"
#include "StructuredVisitor.h"
#include "ShaderVar.h"
#include "GlobalSettings.h"

namespace
{
    struct ShaderAttributeDesc
    {
        he::FixedString m_Name;
        he::gfx::EShaderAttributePropertyType m_Type;

        bool visit(he::io::StructuredVisitor* const visitor)
        {
            visitor->visit(he::HEFS::strName, m_Name);
            visitor->visitCasted<he::uint8>(he::HEFS::strAttributeType, m_Type);
            return true;
        }
    };
    class ShaderUniformDesc
    {
    public:

        ShaderUniformDesc()
            : m_Name(he::HEFS::str)
            , m_UniformType(eShaderVariableType_Invalid)

        {
            he_memset(&m_VariableType, 0, sizeof(VariableType));
            he_memset(&m_DefaultValue, 0, sizeof(DefaultValue));
        }

        const he::FixedString getName() const { return m_Name; }

        EShaderVariableType getUniformType() const { return m_UniformType; }
        he::gfx::EShaderObjectPropertyType getObjectPropertyType() const
        {
            HE_ASSERT(m_UniformType == eShaderVariableType_ObjectProperty, "Getting object property type, but uniform type is not eUniformType_ObjectProperty!");
            return m_VariableType.m_ObjectType;
        }
        he::gfx::EShaderGlobalPropertyType getGlobalPropertyType() const
        {
            HE_ASSERT(m_UniformType == eShaderVariableType_GlobalProperty, "Getting global property type, but uniform type is not eUniformType_GlobalProperty!");
            return m_VariableType.m_GlobalType;
        }
        he::gfx::EShaderUserPropertyType getUserPropertyType() const
        {
            HE_ASSERT(m_UniformType == eShaderVariableType_UserProperty, "Getting user property type, but uniform type is not eUniformType_UserProperty!");
            return m_VariableType.m_UserType;
        }

        void setType(const he::gfx::EShaderObjectPropertyType type) 
        { 
            m_UniformType = eShaderVariableType_ObjectProperty; 
            m_VariableType.m_ObjectType = type;
        }
        void setType(const he::gfx::EShaderGlobalPropertyType type) 
        { 
            m_UniformType = eShaderVariableType_GlobalProperty; 
            m_VariableType.m_GlobalType = type;
        }
        void setType(const he::gfx::EShaderUserPropertyType type) 
        { 
            m_UniformType = eShaderVariableType_UserProperty; 
            m_VariableType.m_UserType = type;
        }

        void setDefaultValue(const float float1)
        {
            HE_ASSERT(m_VariableType.m_UserType == he::gfx::eShaderUserPropertyType_UserFloat, "Setting default value while user type is of other type");
            m_DefaultValue.m_Float[0] = float1;
        }
        void setDefaultValue(const he::vec2& float2)
        {
            HE_ASSERT(m_VariableType.m_UserType == he::gfx::eShaderUserPropertyType_UserFloat2, "Setting default value while user type is of other type");
            m_DefaultValue.m_Float[0] = float2.x;
            m_DefaultValue.m_Float[1] = float2.y;
        }
        void setDefaultValue(const he::vec3& float3)
        {
            HE_ASSERT(m_VariableType.m_UserType == he::gfx::eShaderUserPropertyType_UserFloat3, "Setting default value while user type is of other type");
            m_DefaultValue.m_Float[0] = float3.x;
            m_DefaultValue.m_Float[1] = float3.y;
            m_DefaultValue.m_Float[2] = float3.z;
        }
        void setDefaultValue(const he::vec4& float4)
        {
            HE_ASSERT(m_VariableType.m_UserType == he::gfx::eShaderUserPropertyType_UserFloat4, "Setting default value while user type is of other type");
            m_DefaultValue.m_Float[0] = float4.x;
            m_DefaultValue.m_Float[1] = float4.y;
            m_DefaultValue.m_Float[3] = float4.z;
            m_DefaultValue.m_Float[3] = float4.w;
        }
        void setDefaultValue(const he::Guid& resourceID)
        {
            HE_ASSERT(m_VariableType.m_UserType == he::gfx::eShaderUserPropertyType_Texture1D || m_VariableType.m_UserType == he::gfx::eShaderUserPropertyType_Texture2D || m_VariableType.m_UserType == he::gfx::eShaderUserPropertyType_TextureCube, "Setting default value while user type is of other type");
            resourceID.toString(m_DefaultValue.m_ResourceID);
        }

    private:

        he::FixedString m_Name;
        EShaderVariableType m_UniformType;
        union VariableType
        {
            he::gfx::EShaderObjectPropertyType m_ObjectType;
            he::gfx::EShaderGlobalPropertyType m_GlobalType;
            he::gfx::EShaderUserPropertyType m_UserType;
        } m_VariableType;

        union DefaultValue
        {
            float m_Float[4];
            char m_ResourceID[he::Guid::s_CharbufferSize];
        } m_DefaultValue;

        bool visit(he::io::StructuredVisitor* const visitor)
        {
            visitor->visit(he::HEFS::strName, m_Name);
            visitor->visitCasted<he::uint8>(he::HEFS::strUniformType, m_UniformType);
            switch (m_UniformType)
            {
            case eShaderVariableType_ObjectProperty: visitor->visitCasted<he::uint8>(he::HEFS::strVariableType, m_VariableType.m_ObjectType); break;
            case eShaderVariableType_GlobalProperty: visitor->visitCasted<he::uint8>(he::HEFS::strVariableType, m_VariableType.m_GlobalType); break;
            case eShaderVariableType_UserProperty: visitor->visitCasted<he::uint8>(he::HEFS::strVariableType, m_VariableType.m_UserType); break;
            default: LOG(he::LogType_ProgrammerAssert, "Unknown Uniform type!"); break;
            }
            if (m_UniformType == eShaderVariableType_UserProperty)
            {
                switch (m_VariableType.m_UserType)
                {
                case he::gfx::eShaderUserPropertyType_UserFloat: 
                    {
                        visitor->visit(he::HEFS::strDefaultValue, m_DefaultValue.m_Float[0]);
                    } break;
                case he::gfx::eShaderUserPropertyType_UserFloat2: 
                    {
                        he::vec2 float2(m_DefaultValue.m_Float[0], m_DefaultValue.m_Float[1]);
                        visitor->visit(he::HEFS::strDefaultValue, float2);
                        m_DefaultValue.m_Float[0] = float2.x;
                        m_DefaultValue.m_Float[1] = float2.y;
                    } break;
                case he::gfx::eShaderUserPropertyType_UserFloat3: 
                    {
                        he::vec3 float3(m_DefaultValue.m_Float[0], m_DefaultValue.m_Float[1], m_DefaultValue.m_Float[2]);
                        visitor->visit(he::HEFS::strDefaultValue, float3);
                        m_DefaultValue.m_Float[0] = float3.x;
                        m_DefaultValue.m_Float[1] = float3.y;
                        m_DefaultValue.m_Float[2] = float3.z;
                    } break;
                case he::gfx::eShaderUserPropertyType_UserFloat4: 
                    {
                        he::vec4 float4(m_DefaultValue.m_Float[0], m_DefaultValue.m_Float[1], m_DefaultValue.m_Float[2], m_DefaultValue.m_Float[3]);
                        visitor->visit(he::HEFS::strDefaultValue, float4);
                        m_DefaultValue.m_Float[0] = float4.x;
                        m_DefaultValue.m_Float[1] = float4.y;
                        m_DefaultValue.m_Float[2] = float4.z;
                        m_DefaultValue.m_Float[3] = float4.w;
                    } break;
                case he::gfx::eShaderUserPropertyType_Texture1D: 
                case he::gfx::eShaderUserPropertyType_Texture2D: 
                case he::gfx::eShaderUserPropertyType_TextureCube: 
                    {
                        he::Guid g;
                        if (m_DefaultValue.m_ResourceID != NULL)
                            g = he::Guid(m_DefaultValue.m_ResourceID);
                        visitor->visit(he::HEFS::strDefaultValue, g);
                        g.toString(m_DefaultValue.m_ResourceID);
                    } break;
                }
            }
            return true;
        }
    };
    struct ShaderDesc
    {
        he::String m_VertexShader;
        he::String m_FragmentShader;

        he::ObjectList<ShaderAttributeDesc> m_Attributes;
        he::ObjectList<ShaderUniformDesc> m_Uniforms;

        bool visit(he::io::StructuredVisitor* const visitor)
        {
            visitor->visitObjectList(he::HEFS::strAttributes, m_Attributes);
            visitor->visitObjectList(he::HEFS::strUniforms, m_Uniforms);
            return true;
        }
    };
}

namespace he {
namespace ct {

ShaderLoader::ShaderLoader()
{
}


ShaderLoader::~ShaderLoader()
{
    ResourceFactory<gfx::Shader>::getInstance()->garbageCollect();
}

ObjectHandle ShaderLoader::load(const he::String& vsPath, const he::String& fsPath, const gfx::ShaderLayout& shaderLayout, const he::ObjectList<he::String>& outputs)
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);

    ResourceFactory<gfx::Shader>* factory(ResourceFactory<gfx::Shader>::getInstance());

    he::String key(vsPath + fsPath);
    if (m_AssetContainer.isAssetPresent(key) && factory->isAlive(m_AssetContainer.getAsset(key)))
    {
        ObjectHandle shader(m_AssetContainer.getAsset(key));
        factory->instantiate(shader);
        return shader;
    }
    else
    {
        const gfx::RenderSettings& settings(GlobalSettings::getInstance()->getRenderSettings());
        gfx::Shader* shader(factory->get(factory->create()));
        std::set<he::String> defines;
        if (settings.lightingSettings.enableShadows)
            defines.insert("SHADOWS");
        if (settings.lightingSettings.enableSpecular)
            defines.insert("SPECULAR");
        if (settings.lightingSettings.enableNormalMap)
            defines.insert("NORMALMAP");

        shader->initFromFile(vsPath, fsPath, shaderLayout, defines, outputs);
        m_AssetContainer.addAsset(key, shader->getHandle());
        shader->setLoaded();
        return shader->getHandle();
    }
}

} } //end namespace
