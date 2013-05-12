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
//Created: 2011/09/30
//Major change: 2013/04/28

#ifndef _HE_MATERIAL_H_
#define _HE_MATERIAL_H_
#pragma once

#include "ShaderLayout.h"
#include "Resource.h"

namespace he {
namespace gfx {

class Drawable;
class ICamera;
class ShaderVar;
class Shader;
class ModelMesh;
struct DrawContext;


enum EShaderType
{
    eShaderType_Normal,
    eShaderType_Skinned,
    eShaderType_Instanced,

    eShaderType_MAX
};

namespace details {
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

class Material;
class MaterialLayout
{
    friend class Material;
public:
    typedef he::ObjectList<details::MaterialLayoutElement> layout;
private:
    layout m_Layout[eShaderType_MAX];
};

class Material : public Resource<Material>
{
    enum EMaterialFlags
    {
        eMaterialFlags_Blended = 1 << 0, 
        eMaterialFlags_NoPost = 1 << 1, 
        eMaterialFlags_CullFrontFace = 1 << 2,
        eMaterialFlags_DepthRead = 1 << 3, 
        eMaterialFlags_DepthWrite = 1 << 4
    };
public:
    Material();
    virtual ~Material();
        
    void registerCommonVar(ShaderVar* const var);
    void registerSpecificVar(const EShaderType type, ShaderVar* const var);
    ShaderVar* getVar(const he::FixedString& var); // Only from user vars

    void setNormalShader(Shader* const shader);
    void setSkinnedShader(Shader* const shader);
    void setInstancedShader(Shader* const shader, const BufferLayout& instancingLayout);
    
    void applyNormal(const DrawContext& context) const;
    void applySkinned(const DrawContext& context) const;
    void applyInstanced(const DrawContext& context) const;

    void setIsBlended(bool isBlended, BlendEquation equation = BlendEquation_Add, 
                                      BlendFunc sourceBlend  = BlendFunc_One,
                                      BlendFunc destBlend    = BlendFunc_Zero);
    void setNoPost(const bool noPost) { noPost? raiseFlag(eMaterialFlags_NoPost) : clearFlag(eMaterialFlags_NoPost); }

    bool isBlended() const { return checkFlag(eMaterialFlags_Blended); }
    BlendEquation getBlendEquation() const { return m_BlendEquation; }
    BlendFunc getSourceBlend() const { return m_SourceBlend; }
    BlendFunc getDestBlend() const { return m_DestBlend; }
    bool noPost() const { return checkFlag(eMaterialFlags_NoPost); }

    void setDepthWriteEnabled(const bool enable) { enable? raiseFlag(eMaterialFlags_DepthWrite) : clearFlag(eMaterialFlags_DepthWrite); }
    void setDepthReadEnabled(const bool enable) { enable? raiseFlag(eMaterialFlags_DepthRead) : clearFlag(eMaterialFlags_DepthRead); }

    void setCullFrontFace(const bool enable) { enable? raiseFlag(eMaterialFlags_CullFrontFace) : clearFlag(eMaterialFlags_CullFrontFace); }

    void calculateMaterialLayout(const BufferLayout& bufferLayout, MaterialLayout& outMaterialLayout);

private:
    HE_FORCEINLINE bool checkFlag(const EMaterialFlags flag) const { return (m_Flags & flag) != 0; }
    HE_FORCEINLINE void raiseFlag(const EMaterialFlags flag) { m_Flags |= flag; }
    HE_FORCEINLINE void clearFlag(const EMaterialFlags flag) { m_Flags &= ~flag; }

    void applyShader(const EShaderType type, const DrawContext& context) const;
    void applyMesh(const EShaderType type, const DrawContext& context) const;

    uint8 m_Flags;

    BlendEquation m_BlendEquation;
    BlendFunc m_SourceBlend, m_DestBlend;

    Shader* m_Shader[eShaderType_MAX];
    he::PrimitiveList<ShaderVar*> m_ShaderCommonVars;
    he::PrimitiveList<ShaderVar*> m_ShaderSpecificVars[eShaderType_MAX];
    
    // Disabled
    Material(const Material* other);
    Material* operator=(const Material* other);
};

} } //end namespace

#endif
