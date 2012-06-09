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
//Created: 18/12/2011
#include "HappyPCH.h" 

#include "DefaultSingleDrawable.h"
#include "DrawManager.h"
#include "ICamera.h"

#include "ModelMesh.h"
#include "Material.h"

namespace he {
namespace gfx {

DefaultSingleDrawable::DefaultSingleDrawable(): m_isVisible(true), m_castsShadow(true)
{
}


DefaultSingleDrawable::~DefaultSingleDrawable()
{
}

void DefaultSingleDrawable::applyMaterial( const ICamera* pCamera ) const
{
    getMaterial()->apply(this, pCamera);
}

void DefaultSingleDrawable::applyMaterial( const Material* customMaterial, const ICamera* pCamera ) const
{
    customMaterial->apply(this, pCamera);
}

bool DefaultSingleDrawable::getCastsShadow() const
{
    return m_castsShadow;
}

void DefaultSingleDrawable::setCastsShadow( bool castShadow )
{
    m_castsShadow = castShadow;
}

bool DefaultSingleDrawable::isVisible() const
{
    return m_isVisible && getModelMesh()->isLoaded();
}

void DefaultSingleDrawable::setVisible( bool visible )
{
    m_isVisible = visible;
}

bool DefaultSingleDrawable::isInCamera( const ICamera* pCamera ) const
{
    if (isVisible() == false)
        return false;

    float radius(max<float>(max<float>(getWorldMatrix()(0, 0), getWorldMatrix()(1, 1)), getWorldMatrix()(2, 2)) * getModelMesh()->getBoundingSphere().getRadius());
    vec3 position(getWorldMatrix() * getModelMesh()->getBoundingSphere().getPosition());

    shapes::Sphere sphere(position, radius);

    return DrawManager::viewClip(pCamera, sphere) == false;
}

float DefaultSingleDrawable::getDrawPriority( const ICamera* pCamera ) const
{
    return FLT_MAX - lengthSqr(pCamera->getPosition() - vec3(getWorldMatrix()(0, 3), getWorldMatrix()(1, 3), getWorldMatrix()(2, 3)));
}

void DefaultSingleDrawable::draw()
{
    GL::heBindVao(getModelMesh()->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, getModelMesh()->getNumIndices(), getModelMesh()->getIndexType(), 0);
}

void DefaultSingleDrawable::drawShadow()
{
    GL::heBindVao(getModelMesh()->getVertexShadowArraysID());
    glDrawElements(GL_TRIANGLES, getModelMesh()->getNumIndices(), getModelMesh()->getIndexType(), 0);
}

} } //end namespace