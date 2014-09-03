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

#ifndef _HE_MaterialInstance_H_
#define _HE_MaterialInstance_H_
#pragma once

#include "MaterialEnums.h"
#include "MaterialParameter.h"
#include "AsyncLoadable.h"

namespace he {
namespace gfx {
struct DrawContext;
class Material;
class VertexLayout;
class MaterialInstance : public AsyncLoadable
{
public:
    MaterialInstance(const Material* const parent, const EShaderRenderType type);
    ~MaterialInstance();

    const Material* getParent() const { return m_Material; }

    int8 findParameter(const FixedString& name) const;
    int8 tryFindParameter(const FixedString& name) const;
    MaterialParameter& getParameter(const int8 index);
    
    void apply(const DrawContext& context, const EShaderPassType pass = eShaderPassType_Normal);
    
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
    void setDepthFunc(const DepthFunc func);
    
    void setCullFrontFace(const bool enable) { enable? raiseFlag(eMaterialFlags_CullFrontFace) : clearFlag(eMaterialFlags_CullFrontFace); }
    
    void calculateMaterialLayout(const VertexLayout& bufferLayout);

private:
    HE_FORCEINLINE bool checkFlag(const EMaterialFlags flag) const { return (m_Flags & flag) != 0; }
    HE_FORCEINLINE void raiseFlag(const EMaterialFlags flag) { m_Flags |= flag; }
    HE_FORCEINLINE void clearFlag(const EMaterialFlags flag) { m_Flags &= ~flag; }

    void init();

    void applyShader(const EShaderPassType pass, const DrawContext& context);
    void applyMesh(const EShaderPassType pass, const DrawContext& context) const;

    EShaderRenderType m_Type : 8;
    uint8 m_Flags;
    BlendEquation m_BlendEquation : 16;
    BlendFunc m_SourceBlend : 16;
    BlendFunc m_DestBlend : 16;
    DepthFunc m_DepthFunc : 16;
    const Material* m_Material;
    MaterialLayout m_Layout;

    he::ObjectList<MaterialParameter> m_Parameters;


    //Disable default copy constructor and default assignment operator
    MaterialInstance(const MaterialInstance&);
    MaterialInstance& operator=(const MaterialInstance&);
};

} } //end namespace

#endif
