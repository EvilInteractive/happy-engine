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

#ifndef _HE_MATERIAL_LOADER_H_
#define _HE_MATERIAL_LOADER_H_
#pragma once

#include "AssetContainer.h"
#include "MaterialEnums.h"

namespace he {
namespace io {
    class StructuredVisitor;
}
namespace gfx {
    class Material;
}
namespace ct {

struct MaterialDesc
{
private:
    // Disable
    MaterialDesc(const MaterialDesc&);
    MaterialDesc& operator=(const MaterialDesc& other);
    
public:
    MaterialDesc();
    MaterialDesc& operator=(MaterialDesc&& other); // enable move 

    template<typename T>
    struct MaterialDescParam
    {
    private:
        // Disable
        MaterialDescParam(const MaterialDescParam&);
        MaterialDescParam& operator=(const MaterialDescParam& other);

    public:
        MaterialDescParam()
            : m_Override(false)
        {}
        explicit MaterialDescParam(const T& val)
            : m_Value(val)
            , m_Override(false)
        {}
        MaterialDescParam& operator=(MaterialDescParam&& other) // enable move 
        {
            m_Value = std::move(other.m_Value);
            m_Override = other.m_Override;
            return *this;
        }
        T m_Value;
        bool m_Override;

        void visit(const he::FixedString& key, he::io::StructuredVisitor* const visitor)
        {
            bool result(visitor->visit(key, m_Value));
            if (visitor->isReading())
            {
                m_Override = result;
            }
        }
        template<typename ToType>
        void visitCasted(const he::FixedString& key, 
            he::io::StructuredVisitor* const visitor,
            const std::function<ToType(T)>& castTo = (checked_numcast<ToType, T>),
            const std::function<T(ToType)>& castFrom = (checked_numcast<T, ToType>))
        {
            bool result(visitor->visitCasted<ToType, T>(key, m_Value, NULL, castTo, castFrom));
            if (visitor->isReading())
            {
                m_Override = result;
            }
        }
    };
    struct MaterialDescShader
    {
    private:
        // Disable
        MaterialDescShader(const MaterialDescShader&);
        MaterialDescShader& operator=(const MaterialDescShader& other);

    public:
        MaterialDescShader();
        MaterialDescShader& operator=(MaterialDescShader&& other); // enable move 

        MaterialDescParam<he::String> m_FragmentShader;
        MaterialDescParam<he::String> m_VertexShader;
        MaterialDescParam<he::String> m_GeometryShader;
        MaterialDescParam<he::ObjectList<he::String>> m_Defines;
        MaterialDescParam<he::ObjectList<he::String>> m_OutputLayout;

        void visit(he::io::StructuredVisitor* const visitor);
    };
    struct MaterialDescOptions
    {
    private:
        // Disable
        MaterialDescOptions(const MaterialDescOptions&);
        MaterialDescOptions& operator=(const MaterialDescOptions& other);

    public:
        MaterialDescOptions();
        MaterialDescOptions& operator=(MaterialDescOptions&& other); // enable move 

        MaterialDescParam<bool> m_IsBlended;
        MaterialDescParam<bool> m_NoPost;
        MaterialDescParam<bool> m_CullFrontFace;
        MaterialDescParam<bool> m_DepthRead;
        MaterialDescParam<bool> m_DepthWrite;
        MaterialDescParam<bool> m_CastShadow;
        MaterialDescParam<gfx::BlendEquation> m_BlendEquation;
        MaterialDescParam<gfx::BlendFunc> m_SourceBlend;
        MaterialDescParam<gfx::BlendFunc> m_DestBlend;
        he::ObjectList<NameValuePair<he::String>> m_Params;

        void visit(he::io::StructuredVisitor* const visitor);
    };

    he::String m_Inherits;
    MaterialDescShader m_Shader[gfx::eShaderPassType_MAX][gfx::eShaderRenderType_MAX];
    MaterialDescOptions m_Options;

    void visit(he::io::StructuredVisitor* const visitor);
};

struct MaterialDescStack
{
    ObjectList<MaterialDesc> m_Stack;

    void collapse(MaterialDesc& outDesc);
    bool load(const he::String& asset);
    void visit(he::io::StructuredVisitor* const visitor);
};

class MaterialLoader
{
public:
    MaterialLoader();
    ~MaterialLoader();
    
    gfx::Material* load(const he::String& asset);
    
private:
    AssetContainer<ObjectHandle> m_AssetContainer;
    
    //Disable default copy constructor and default assignment operator
    MaterialLoader(const MaterialLoader&);
    MaterialLoader& operator=(const MaterialLoader&);
};

} } //end namespace

#endif
