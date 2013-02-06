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
//Author:  Sebastiaan Sprengers
//Created: 27/11/2012

#include "HappyPCH.h" 

#include "NinePatchEffect.h"
#include "ContentManager.h"
#include "Shader.h"

namespace he {
namespace gfx {

NinePatchEffect::NinePatchEffect() : m_Shader(nullptr)
{
}


NinePatchEffect::~NinePatchEffect()
{
    if (m_Shader != nullptr)
        m_Shader->release();
}

void NinePatchEffect::load()
{
    using namespace he;
    using namespace gfx;

    ShaderLayout layout;
    layout.addElement(ShaderLayoutElement(0, "inPosition"));
    layout.addElement(ShaderLayoutElement(1, "inTexCoord"));

    m_Shader = ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create());
    he::ObjectList<std::string> shaderOutputs;
    shaderOutputs.add("outColor");
    const std::string& folder(CONTENT->getShaderFolderPath().str());
    const bool compiled = m_Shader->initFromFile(folder + "2D/ninepatchshader.vert", 
                                           folder + "2D/ninepatchshader.frag", layout, shaderOutputs);
    HE_ASSERT(compiled, ""); compiled;

    m_ShaderWVPPos = m_Shader->getShaderVarId("matWVP");
    m_ShaderDiffTexPos = m_Shader->getShaderSamplerId("diffuseMap");
    m_ShaderCenterPos = m_Shader->getShaderVarId("center");
    m_ShaderSizePos = m_Shader->getShaderVarId("size");
    m_ShaderOrigSizePos = m_Shader->getShaderVarId("originalSize");
    m_ShaderDepthPos = m_Shader->getShaderVarId("depth");
    m_ShaderBlendColor = m_Shader->getShaderVarId("blendColor");

    m_Shader->bind();
    mat44 MatWVP = mat44::createTranslation(vec3(0.0f,0.0f,0.0f));
    m_Shader->setShaderVar(m_ShaderWVPPos, MatWVP);
}

void NinePatchEffect::begin() const
{
    m_Shader->bind();
}

void NinePatchEffect::end() const
{
}

void NinePatchEffect::setWorldMatrix(const he::mat44& mat) const
{
    m_Shader->setShaderVar(m_ShaderWVPPos, mat);
}

void NinePatchEffect::setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const
{
    m_Shader->setShaderVar(m_ShaderDiffTexPos, diffuseMap);
}

void NinePatchEffect::setCenter(const RectF& center) const
{
    m_Shader->setShaderVar(m_ShaderCenterPos, vec4(center.x,center.y,center.width,center.height));
}

void NinePatchEffect::setSize(const vec2& size) const
{
    m_Shader->setShaderVar(m_ShaderSizePos, size);
}

void NinePatchEffect::setOriginalSize(const vec2& origSize) const
{
    m_Shader->setShaderVar(m_ShaderOrigSizePos, origSize);
}

void NinePatchEffect::setDepth(float depth) const
{
    m_Shader->setShaderVar(m_ShaderDepthPos, depth);
}

void NinePatchEffect::setBlendColor( const Color& color ) const
{
    m_Shader->setShaderVar(m_ShaderBlendColor, color.rgba());
}

} } //end namespace