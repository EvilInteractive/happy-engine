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

#ifndef _HE_MATERIAL_H_
#define _HE_MATERIAL_H_
#pragma once

#include "BufferLayout.h"
#include "Resource.h"

namespace he {
namespace gfx {

class IInstancedDrawable;
class ISingleDrawable;
class ISkinnedDrawable;
class ICamera;
class ShaderVar;

class Material : public Resource<Material>
{
public:
    Material();
    virtual ~Material();
    
    void registerVar(ShaderVar* var);
    ShaderVar* getVar(const std::string& var);
    void setShader(const ObjectHandle& shaderHandle, const BufferLayout& compatibleVertexLayout, const BufferLayout& compatibleInstancingLayout);

    const BufferLayout& getCompatibleVertexLayout() const;
    const BufferLayout& getCompatibleInstancingLayout() const;

    void apply(const IInstancedDrawable* pObj,  const ICamera* pCamera) const;
    void apply(const ISingleDrawable*    pObj,  const ICamera* pCamera) const;
    void apply(const ISkinnedDrawable*   pObj,  const ICamera* pCamera) const;

    void setIsBlended(bool isBlended, BlendEquation equation = BlendEquation_Add, 
                                      BlendFunc sourceBlend  = BlendFunc_One,
                                      BlendFunc destBlend    = BlendFunc_Zero);
    void setNoPost(bool noPost);
    void setIsBackground(bool isBackground);

    bool isBlended() const;
    BlendEquation getBlendEquation() const;
    BlendFunc getSourceBlend() const;
    BlendFunc getDestBlend() const;
    bool noPost() const;
    bool isBackground() const;
    bool isUsedForInstancing() const;

    void setDepthWriteEnabled(bool enable);
    void setDepthReadEnabled(bool enable);

private:

    BlendEquation m_BlendEquation;
    BlendFunc m_SourceBlend, m_DestBlend;
    bool m_IsBlended, m_UsedForInstancing, m_NoPost, m_IsBackground;

    ObjectHandle m_ShaderHandle;
    std::vector<ShaderVar*> m_ShaderVar;

    bool m_DepthRead, m_DepthWrite;

    BufferLayout m_CompatibleVL; // vertex layout
    BufferLayout m_CompatibleIL; // instance layout

    // Disabled
    Material(const Material* other);
    Material* operator=(const Material* other);
};

} } //end namespace

#endif
