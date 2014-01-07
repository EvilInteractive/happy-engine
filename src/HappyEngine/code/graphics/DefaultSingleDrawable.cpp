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
#include "ICamera.h"

#include "ModelMesh.h"
#include "Material.h"
#include "Scene.h"

namespace he {
namespace gfx {

DefaultSingleDrawable::DefaultSingleDrawable()
    : m_CastsShadow(true)
    , m_Bound(AABB(vec3(-1, -1, -1), vec3(1, 1, 1)))
    , m_Scene(nullptr)
    , m_NeedsReevalute(false)
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
    return m_CastsShadow;
}

void DefaultSingleDrawable::setCastsShadow( bool castShadow )
{
    m_CastsShadow = castShadow;
}

void DefaultSingleDrawable::draw()
{
    const ModelMesh* mesh(getModelMesh());
    GL::heBindVao(mesh->getVertexArraysID());
    glDrawElements(mesh->getDrawMode(), mesh->getNumIndices(), mesh->getIndexType(), 0);
}

void DefaultSingleDrawable::drawShadow()
{
    const ModelMesh* mesh(getModelMesh());
    GL::heBindVao(mesh->getVertexShadowArraysID());
    glDrawElements(mesh->getDrawMode(), mesh->getNumIndices(), mesh->getIndexType(), 0);
}

void DefaultSingleDrawable::detachFromScene()
{
    HE_IF_ASSERT(m_Scene != nullptr, "Object not attached to scene")
    {
        m_Scene->detachFromScene(this);
        m_Scene = nullptr;
        nodeReevaluated();
    }
}

void DefaultSingleDrawable::attachToScene( Scene* scene )
{
    HE_IF_ASSERT(m_Scene == nullptr, "Object already attached to scene")
    {
        m_Scene = scene;
        m_Scene->attachToScene(this);
        nodeReevaluated();
    }
}

void DefaultSingleDrawable::setScene( Scene* scene )
{
    m_Scene = scene;
}

Scene* DefaultSingleDrawable::getScene() const
{
    return m_Scene;
}

bool DefaultSingleDrawable::isAttachedToScene() const
{
    return m_Scene != nullptr;
}

const Bound& DefaultSingleDrawable::getBound() const
{
    return m_Bound;
}

void DefaultSingleDrawable::calculateBound()
{
    HE_ASSERT(getModelMesh() != nullptr, "ModelMesh is nullptr when getting bound");
    HE_ASSERT(getModelMesh()->isLoaded(), "ModelMesh is not loaded when getting bound, wrong octree insertion will happen!");
    const AABB& aabb(getModelMesh()->getBound().getAABB());
    mat44 world(getWorldMatrix());
    AABB newAABB(world * aabb.getTopFrontLeft(),
                 world * aabb.getBottomBackRight());
    m_Bound.fromAABB(newAABB);
}

void DefaultSingleDrawable::setWorldMatrixDirty( uint8 cause )
{
    SingleDrawable::setWorldMatrixDirty(cause);
    if (m_NeedsReevalute == false && isAttachedToScene())
    {
        m_NeedsReevalute = true;
        m_Scene->doReevalute(this);
    }
}

} } //end namespace
