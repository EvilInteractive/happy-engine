//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 2013/12/27

#ifndef _HE_MaterialEnums_H_
#define _HE_MaterialEnums_H_
#pragma once

namespace he {
namespace gfx {
    
    enum EShaderRenderType
    {
        eShaderRenderType_Unknown = -1,

        eShaderRenderType_Normal,
        eShaderRenderType_Skinned,
        eShaderRenderType_Instanced,
        
        eShaderRenderType_MAX
    };

    enum EShaderPassType
    {
        eShaderPassType_Unknown = -1,

        eShaderPassType_Normal,
        eShaderPassType_Shadow,

        eShaderPassType_MAX
    };
    
    namespace details
    {
        struct MaterialLayoutElement
        {
            MaterialLayoutElement(): m_ElementIndex(UINT16_MAX), m_Components(0), m_Type(0), m_Stride(0), m_ByteOffset(0) {}
            ~MaterialLayoutElement() {}
            
            uint16 m_ElementIndex;
            uint8 m_Components;
            uint16 m_Type;
            uint16 m_Stride;
            uint8 m_ByteOffset;
        };
    }
    class MaterialLayout
    {
        friend class Material;
        friend class MaterialInstance;
    public:
        typedef he::ObjectList<details::MaterialLayoutElement> layout;
    private:
        layout m_Layout[eShaderPassType_MAX][eShaderRenderType_MAX];
    };

    enum EMaterialFlags
    {
        eMaterialFlags_Blended = 1 << 0,
        eMaterialFlags_NoPost = 1 << 1,
        eMaterialFlags_CullFrontFace = 1 << 2,
        eMaterialFlags_DepthRead = 1 << 3,
        eMaterialFlags_DepthWrite = 1 << 4
    };
    
} } //end namespace

#endif
