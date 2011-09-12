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

#ifndef _HE_SHADER_H_
#define _HE_SHADER_H_
#pragma once

#include <string>
#include <map>
#include <vector>
#include "VertexLayout.h"

#include "Matrix.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Texture2D.h"
#include "HappyTypes.h"

namespace happyengine {
namespace graphics {

class Shader
{
public:
	Shader();
    virtual ~Shader();

    bool init(const std::string& vsPath, const std::string& fsPath, const VertexLayout& vertexLayout, 
              const std::vector<std::string>& outputs = std::vector<std::string>());

    void begin();
    void end();

    uint getShaderVarId(const std::string& name) const;
    uint getShaderSamplerId(const std::string& name);
    
    void setShaderVar(uint id, int value) const;
    void setShaderVar(uint id, float value) const;
    void setShaderVar(uint id, const math::Vector2& vec) const;
    void setShaderVar(uint id, const math::Vector3& vec) const;
    void setShaderVar(uint id, const math::Vector4& vec) const;
    void setShaderVar(uint id, const math::Matrix& matrix) const;
    void setShaderVar(uint id, const graphics::Texture2D::pointer& tex2D) const;

private:
    uint m_Id;
    uint m_VsId;
    uint m_FsId;

    std::map<std::string, uint> m_SamplerLocationMap;

    std::string m_FragShaderName;

    static uint s_CurrentBoundShader;
    uint m_prevBoundShader;

    //Disable default copy constructor and default assignment operator
    Shader(const Shader&);
    Shader& operator=(const Shader&);
};

} } //end namespace

#endif
