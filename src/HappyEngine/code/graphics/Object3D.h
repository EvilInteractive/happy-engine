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
//Created: 30/09/2011

#ifndef _HE_I3D_OBJECT_H_
#define _HE_I3D_OBJECT_H_
#pragma once

namespace he {
class mat44;

class HAPPY_ENTRY IObject3D
{
    friend class Object3D;
public:
    virtual ~IObject3D() {}

    enum DirtyFlag // 4 bit MAX
    {
        DirtyFlag_Translate = 1 << 0,
        DirtyFlag_Rotate = 1 << 1,
        DirtyFlag_Scale = 1 << 2,
        DirtyFlag_Link = DirtyFlag_Translate | DirtyFlag_Rotate | DirtyFlag_Scale | 1 << 3
    };

    virtual void setLocalTranslate(const vec3& translate) = 0;
    virtual void setLocalRotate(const mat33& rotate) = 0;
    virtual void setLocalScale(const vec3& scale) = 0;

    virtual const vec3&  getLocalTranslate() const = 0;
    virtual const mat33& getLocalRotate() const = 0;
    virtual const vec3&  getLocalScale() const = 0;

    virtual const mat44& getLocalMatrix() const = 0;
    virtual const mat44& getWorldMatrix() const = 0;

    virtual void attach(IObject3D* child) = 0;
    virtual void detach(IObject3D* child) = 0;

protected:
    virtual IObject3D* getParent() const = 0;
    virtual void setParent(IObject3D* parent) = 0;

    virtual void setWorldMatrixDirty(const uint8 cause) = 0; // sets all childs to dirty as well, if dirty it presumes all childs are already dirty
    virtual void setLocalMatrixDirty(const uint8 cause) = 0; // sets all childs to dirty as well, if dirty it presumes all childs are already dirty

    virtual void calculateWorldMatrix() = 0;
};

#define IMPLEMENT_IOBJECT3D_FROM(X) \
public:\
virtual void setLocalTranslate(const he::vec3& translate)  { X::setLocalTranslate(translate); } \
virtual void setLocalRotate(const he::mat33& rotate) { X::setLocalRotate(rotate); } \
virtual void setLocalScale(const he::vec3& scale) { X::setLocalScale(scale); } \
\
virtual const he::vec3&  getLocalTranslate() const { return X::getLocalTranslate(); } \
virtual const he::mat33& getLocalRotate() const { return X::getLocalRotate(); } \
virtual const he::vec3&  getLocalScale() const { return X::getLocalScale(); } \
\
virtual const he::mat44& getLocalMatrix() const { return X::getLocalMatrix(); } \
virtual const he::mat44& getWorldMatrix() const { return X::getWorldMatrix(); } \
\
virtual void attach(he::IObject3D* child) { X::attach(child); } \
virtual void detach(he::IObject3D* child) { X::detach(child); } \
\
protected:\
virtual he::IObject3D* getParent() const { return X::getParent(); } \
virtual void setParent(he::IObject3D* parent) { X::setParent(parent); } \
\
virtual void setWorldMatrixDirty(const he::uint8 cause) { X::setWorldMatrixDirty(cause); } \
virtual void setLocalMatrixDirty(const he::uint8 cause) { X::setLocalMatrixDirty(cause); } \
\
virtual void calculateWorldMatrix() { X::calculateWorldMatrix(); }


class HAPPY_ENTRY Object3D : public IObject3D
{
public:
    Object3D();
    virtual ~Object3D();
    
    void setLocalTranslate(const vec3& translate); // override final
    void setLocalRotate(const mat33& rotate); // override final
    void setLocalScale(const vec3& scale); // override final

    const vec3&  getLocalTranslate() const; // override final
    const mat33& getLocalRotate() const; // override final
    const vec3&  getLocalScale() const; // override final

    const mat44& getLocalMatrix() const; // override final
    const mat44& getWorldMatrix() const; // override final

    void attach(IObject3D* child); // override final
    void detach(IObject3D* child); // override final

protected:
    virtual void setWorldMatrixDirty(const uint8 cause); // override // sets all childs to dirty as well, if dirty it presumes all childs are already dirty
    virtual void setLocalMatrixDirty(const uint8 cause); // override // sets all childs to dirty as well, if dirty it presumes all childs are already dirty

    virtual void calculateWorldMatrix(); // override

    IObject3D* getParent() const { return m_Parent; } // override final
    void setParent(IObject3D* parent) { m_Parent = parent; } // override final
    
    enum DirtyFlag // 4 bit MAX
    {
        DirtyFlag_Translate = 1 << 0,
        DirtyFlag_Rotate = 1 << 1,
        DirtyFlag_Scale = 1 << 2,
        DirtyFlag_Link = DirtyFlag_Translate | DirtyFlag_Rotate | DirtyFlag_Scale | 1 << 3
    };

    IObject3D* m_Parent;
    he::PrimitiveList<IObject3D*> m_Childs;

    mutable uint8 m_LocalMatrixDirty : 4;
    mutable uint8 m_WorldMatrixDirty : 4;

    mutable mat44 m_WorldMatrix;
    mutable mat44 m_LocalMatrix;

    vec3  m_LocalTranslate;
    mat33 m_LocalRotation;
    vec3  m_LocalScale;
};   

} //end namespace

#endif
