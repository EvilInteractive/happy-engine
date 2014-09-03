//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
//Created: 02/09/2012
#include "HappyPCH.h"
#include "Object3D.h"

namespace he {

Object3D::Object3D(): m_Parent(nullptr), m_LocalMatrixDirty(0), m_WorldMatrixDirty(0),
    m_LocalTranslate(0, 0, 0), m_LocalScale(1, 1, 1), m_LocalRotation(mat33::Identity),
    m_WorldMatrix(mat44::Identity), m_LocalMatrix(mat44::Identity)
{

}

Object3D::~Object3D()
{

}

const mat44& Object3D::getLocalMatrix() const
{
    if (m_LocalMatrixDirty)
    {
        m_LocalMatrix = mat44::createWorld(m_LocalTranslate, m_LocalRotation, m_LocalScale);
        m_LocalMatrixDirty = 0;
    }
    return m_LocalMatrix;
}

const mat44& Object3D::getWorldMatrix() const
{
    if (m_WorldMatrixDirty)
    {
        m_WorldMatrixDirty = 0;
        const_cast<Object3D*>(this)->calculateWorldMatrix();
    }
    return m_WorldMatrix;
}
void Object3D::calculateWorldMatrix()
{
    if (m_Parent != nullptr)
        m_WorldMatrix = m_Parent->getWorldMatrix() * getLocalMatrix();
    else
        m_WorldMatrix = getLocalMatrix();
}


void Object3D::attach( IObject3D* child )
{
    HE_ASSERT(child->getParent() == nullptr, "Attaching child which is already attached somewhere!");
    m_Childs.add(child);
    child->setParent(this);
    child->setWorldMatrixDirty(DirtyFlag_Link);
}

void Object3D::detach( IObject3D* child )
{
    HE_IF_ASSERT(child->getParent() == this, "detaching child which parent is not me")
    {
        HE_IF_ASSERT(m_Childs.contains(child), "detaching child which is not in my child list")
        {
            m_Childs.remove(child);
            child->setParent(nullptr);
            child->setWorldMatrixDirty(DirtyFlag_Link);
        }
    }
}

void Object3D::setWorldMatrixDirty(const uint8 cause)
{
    if ( (m_WorldMatrixDirty & cause) != cause)
    {
        m_WorldMatrixDirty |= cause;
        m_Childs.forEach([cause](IObject3D* obj)
        {
            obj->setWorldMatrixDirty(cause);
        });
    }
}

void Object3D::setLocalMatrixDirty(uint8 cause)
{
    m_LocalMatrixDirty |= cause;
    setWorldMatrixDirty(cause);
}

void he::Object3D::setLocalTranslate( const vec3& translate )
{
    if (translate != m_LocalTranslate)
    {
        m_LocalTranslate = translate;
        setLocalMatrixDirty(DirtyFlag_Translate);
    }
}
void he::Object3D::setLocalRotate( const mat33& rotate )
{
    if (rotate != m_LocalRotation)
    {
        m_LocalRotation = rotate;
        setLocalMatrixDirty(DirtyFlag_Rotate);
    }
}
void he::Object3D::setLocalScale( const vec3& scale )
{
    if (scale != m_LocalScale)
    {
        m_LocalScale = scale;
        setLocalMatrixDirty(DirtyFlag_Scale);
    }
}

const vec3& he::Object3D::getLocalTranslate() const
{
    return m_LocalTranslate;
}
const mat33& he::Object3D::getLocalRotate() const
{
    return m_LocalRotation;
}
const vec3& he::Object3D::getLocalScale() const
{
    return m_LocalScale;
}

}
