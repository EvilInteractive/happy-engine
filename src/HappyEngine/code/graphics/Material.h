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

#include "Resource.h"
#include "MaterialEnums.h"
#include "MaterialParameter.h"

namespace he {
namespace gfx {

class IShaderUniform;
class Shader;
class MaterialInstance;
class VertexLayout;

class Material : public Resource<Material>
{
    friend class MaterialInstance;
public:
    Material();
    ~Material();
    
    void init();

    void setNormalShader(Shader* const shader);
    void setSkinnedShader(Shader* const shader);
    void setInstancedShader(Shader* const shader, const VertexLayout& instancingLayout);
    
    Shader* getShader(const EShaderType type) const { return m_Shader[type]; }
    
    void setIsBlended(bool isBlended, BlendEquation equation = BlendEquation_Add,
                      BlendFunc sourceBlend  = BlendFunc_One,
                      BlendFunc destBlend    = BlendFunc_Zero);
    void setNoPost(const bool noPost) { noPost? raiseFlag(eMaterialFlags_NoPost) : clearFlag(eMaterialFlags_NoPost); }
    
    void setCullFrontFace(const bool enable) { enable? raiseFlag(eMaterialFlags_CullFrontFace) : clearFlag(eMaterialFlags_CullFrontFace); }
    
    void setDepthWriteEnabled(const bool enable) { enable? raiseFlag(eMaterialFlags_DepthWrite) : clearFlag(eMaterialFlags_DepthWrite); }
    void setDepthReadEnabled(const bool enable) { enable? raiseFlag(eMaterialFlags_DepthRead) : clearFlag(eMaterialFlags_DepthRead); }
    
    MaterialInstance* createMaterialInstance(const EShaderType type) const;
    
private:
    Shader* bindShader(const EShaderType type) const;
    void calculateMaterialLayout(const VertexLayout& bufferLayout, MaterialLayout& outMaterialLayout) const;
    
    HE_FORCEINLINE bool checkFlag(const EMaterialFlags flag) const { return (m_Flags & flag) != 0; }
    HE_FORCEINLINE void raiseFlag(const EMaterialFlags flag) { m_Flags |= flag; }
    HE_FORCEINLINE void clearFlag(const EMaterialFlags flag) { m_Flags &= ~flag; }
    
    Shader* m_Shader[eShaderType_MAX];
    
    uint8 m_Flags;
    
    BlendEquation m_BlendEquation;
    BlendFunc m_SourceBlend, m_DestBlend;

    // Disabled
    Material(const Material* other);
    Material* operator=(const Material* other);
};

} } //end namespace

#endif
