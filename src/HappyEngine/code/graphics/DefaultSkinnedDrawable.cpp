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
//Created: 18/12/2011
#include "HappyPCH.h" 

#include "DefaultSkinnedDrawable.h"
#include "DrawManager.h"
#include "ICamera.h"

namespace he {
namespace gfx {

DefaultSkinnedDrawable::DefaultSkinnedDrawable()
{
}


DefaultSkinnedDrawable::~DefaultSkinnedDrawable()
{
}

void DefaultSkinnedDrawable::applyMaterial( const ICamera* pCamera ) const
{
    getMaterial().apply(this, pCamera);
}

void DefaultSkinnedDrawable::applyMaterial( const Material& customMaterial, const ICamera* pCamera ) const
{
    customMaterial.apply(this, pCamera);
}

bool DefaultSkinnedDrawable::getCastsShadow() const
{
    return m_castsShadow;
}

void DefaultSkinnedDrawable::setCastsShadow( bool castShadow )
{
    m_castsShadow = castShadow;
}

bool DefaultSkinnedDrawable::isVisible() const
{
    return m_isVisible && getModelMesh()->isLoaded();
}

void DefaultSkinnedDrawable::setVisible( bool visible )
{
    m_isVisible = visible;
}

bool DefaultSkinnedDrawable::isInCamera( const ICamera* pCamera ) const
{
    if (isVisible() == false)
        return false;

    float radius(max<float>(max<float>(getWorldMatrix()(0, 0), getWorldMatrix()(1, 1)), getWorldMatrix()(2, 2)) * getModelMesh()->getBoundingSphere().getRadius());
    vec3 position(getWorldMatrix() * getModelMesh()->getBoundingSphere().getPosition());

    shapes::Sphere sphere(position, radius);

    return !DrawManager::viewClip(pCamera, sphere);
}

float DefaultSkinnedDrawable::getDrawPriority( const ICamera* pCamera ) const
{
    return FLT_MAX - lengthSqr(pCamera->getPosition() - vec3(getWorldMatrix()(0, 3), getWorldMatrix()(1, 3), getWorldMatrix()(2, 3)));
}

void DefaultSkinnedDrawable::draw()
{
    GL::heBindVao(getModelMesh()->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, getModelMesh()->getNumIndices(), getModelMesh()->getIndexType(), 0);
}

void DefaultSkinnedDrawable::drawShadow()
{
    GL::heBindVao(getModelMesh()->getVertexShadowArraysID());
    glDrawElements(GL_TRIANGLES, getModelMesh()->getNumIndices(), getModelMesh()->getIndexType(), 0);
}

} } //end namespace