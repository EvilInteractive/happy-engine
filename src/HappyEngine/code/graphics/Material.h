//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include "Shader.h"
#include "HappyTypes.h"
#include "boost/shared_ptr.hpp"
#include "ShaderVar.h"
#include "Camera.h"
#include "Color.h"
#include "VertexLayout.h"

namespace he {
namespace gfx {

class IDrawable;

class Material
{
public:
	Material();
    virtual ~Material();
    // default copy constructor and default assignment operator are OK

    void addVar(const ShaderVar::pointer& var);
    void setShader(const Shader::pointer& pShader, const VertexLayout& compatibleVL);

    const VertexLayout& getCompatibleVertexLayout() const;

    void apply(const IDrawable* pObj, const Camera* pCamera) const;

private:
    bool isTranslucent;

    Shader::pointer m_pShader;
    std::vector<ShaderVar::pointer> m_ShaderVar;

    VertexLayout m_CompatibleVL;
};

} } //end namespace

#endif
