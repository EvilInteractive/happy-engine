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
#include <set>
#include "ShaderLayout.h"

#include "mat44.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "Texture2D.h"
#include "HappyTypes.h"

#include "boost/shared_ptr.hpp"

namespace he {
namespace gfx {

class Shader
{
public:
	Shader();
    virtual ~Shader();

    bool init(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout);
    bool init(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout, const std::vector<std::string>& outputs);
    bool init(const std::string& vsPath, const std::string& fsPath, const ShaderLayout& shaderLayout, const std::set<std::string>& defines, const std::vector<std::string>& outputs = std::vector<std::string>());

    void begin();
    void end();

    uint getShaderVarId(const std::string& name) const;
    uint getShaderSamplerId(const std::string& name);
    
    void setShaderVar(uint id, int value) const;
    void setShaderVar(uint id, float value) const;
    void setShaderVar(uint id, const vec2& vec) const;
    void setShaderVar(uint id, const vec3& vec) const;
    void setShaderVar(uint id, const vec4& vec) const;
    void setShaderVar(uint id, const mat44& matrix) const;
    void setShaderVar(uint id, const gfx::Texture2D::pointer& tex2D) const;

    typedef boost::shared_ptr<Shader> pointer;

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
