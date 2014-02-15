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
#include "Drawable.h"

#include "ICamera.h"

#include "ModelMesh.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "Scene.h"

namespace he {
namespace gfx {

Drawable::Drawable()
    : m_Bound(AABB(vec3(-1, -1, -1), vec3(1, 1, 1)))
    , m_ModelMesh(nullptr)
    , m_Material(nullptr)
    , m_Scene(nullptr)
    , m_Flags(eDrawableFlags_None)
#ifdef HE_USE_OCTREE
    , m_Node(nullptr)
#endif
{
}


Drawable::~Drawable()
{
    setModelMesh(nullptr);
    setMaterial(nullptr);
}

void Drawable::setModelMesh( ModelMesh* const mesh )
{
    cancelLoad();
    if (m_ModelMesh != nullptr)
    {
        m_ModelMesh->release();
    }
    m_ModelMesh = mesh;
    if (m_ModelMesh != nullptr)
    {
        raiseFlag(eDrawableFlags_NeedsBoundUpdate);
        m_ModelMesh->instantiate();
        updateMaterialLayout(m_ModelMesh, m_Material);
    }
}

void Drawable::setMaterial(const Material* const material )
{
    cancelLoad();
    if (m_Material != nullptr)
    {
        delete m_Material;
        m_Material = nullptr;
    }
    if (material != nullptr)
    {
        m_Material = material->createMaterialInstance(eShaderType_Normal);
        updateMaterialLayout(m_ModelMesh, m_Material);
    }
}

void Drawable::cancelLoad()
{
    if (m_ModelMesh != nullptr)
        m_ModelMesh->cancelLoadCallback(this);
    if (m_Material != nullptr)
        m_Material->cancelLoadCallback(this);
    clearFlag(eDrawableFlags_IsLoaded);
}

void Drawable::updateMaterialLayout(ModelMesh* const mesh, MaterialInstance* const material)
{
    if (m_ModelMesh == mesh && m_Material == material) // Threading recall check
    {
        if ( m_ModelMesh != nullptr && m_Material != nullptr)
        {
            if (m_ModelMesh->isLoaded())
            {
                if (m_Material->isLoaded())
                {
                    m_Material->calculateMaterialLayout(m_ModelMesh->getVertexLayout());
                    raiseFlag(eDrawableFlags_IsLoaded);
                    reevaluate();
                }
                else
                {
                    m_Material->callbackOnceIfLoaded(this, std::bind(&Drawable::updateMaterialLayout, this, mesh, material));
                }
            }
            else
            {
                m_ModelMesh->callbackOnceIfLoaded(this, std::bind(&Drawable::updateMaterialLayout, this, mesh, material));
            }
        }
    }
}

void Drawable::detachFromScene()
{
    HE_IF_ASSERT(m_Scene != nullptr, "Object not attached to scene")
    {
        m_Scene->detachFromScene(this);
        m_Scene = nullptr;
    }
}

void Drawable::attachToScene( Scene* scene )
{
    HE_IF_ASSERT(isAttachedToScene() == false, "Object already attached to scene")
    {
        m_Scene = scene;
        m_Scene->attachToScene(this);
    }
}

void Drawable::calculateBound()
{
    if (checkFlag(eDrawableFlags_NeedsBoundUpdate))
    {
        HE_IF_ASSERT(m_ModelMesh != nullptr, "ModelMesh is nullptr when getting bound")
        HE_IF_ASSERT(m_ModelMesh->isLoaded(), "ModelMesh is not loaded when getting bound, wrong octree insertion will happen!")
        {
            const AABB& aabb(m_ModelMesh->getBound().getAABB());
            mat44 world(getWorldMatrix());
            AABB newAABB(world * aabb.getTopFrontLeft(),
                         world * aabb.getBottomBackRight());
            m_Bound.fromAABB(newAABB);
            clearFlag(eDrawableFlags_NeedsBoundUpdate);
        }
    }
}

void Drawable::setWorldMatrixDirty( const uint8 cause )
{
    Object3D::setWorldMatrixDirty(cause);
    raiseFlag(eDrawableFlags_NeedsBoundUpdate);
    reevaluate();
}

void Drawable::reevaluate()
{
    if (checkFlag(eDrawableFlags_NeedsSceneReevaluate) == false && isAttachedToScene())
    {
        raiseFlag(eDrawableFlags_NeedsSceneReevaluate);
        m_Scene->doReevalute(this);
    }
}

} } //end namespace
