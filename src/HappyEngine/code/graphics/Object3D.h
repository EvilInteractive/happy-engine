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
//Created: 30/09/2011

#ifndef _HE_I3D_OBJECT_H_
#define _HE_I3D_OBJECT_H_
#pragma once

namespace he {
class mat44;

class IObject3D
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

    virtual void setWorldMatrixDirty(byte cause) = 0; // sets all childs to dirty as well, if dirty it presumes all childs are already dirty
    virtual void setLocalMatrixDirty(byte cause) = 0; // sets all childs to dirty as well, if dirty it presumes all childs are already dirty

    virtual void calculateWorldMatrix() = 0;
};

#define IMPLEMENT_IOBJECT3D_FROM(X) \
public:\
virtual void setLocalTranslate(const vec3& translate)  { X::setLocalTranslate(translate); } \
virtual void setLocalRotate(const mat33& rotate) { X::setLocalRotate(rotate); } \
virtual void setLocalScale(const vec3& scale) { X::setLocalScale(scale); } \
\
virtual const vec3&  getLocalTranslate() const { return X::getLocalTranslate(); } \
virtual const mat33& getLocalRotate() const { return X::getLocalRotate(); } \
virtual const vec3&  getLocalScale() const { return X::getLocalScale(); } \
\
virtual const mat44& getLocalMatrix() const { return X::getLocalMatrix(); } \
virtual const mat44& getWorldMatrix() const { return X::getWorldMatrix(); } \
\
virtual void attach(IObject3D* child) { X::attach(child); } \
virtual void detach(IObject3D* child) { X::detach(child); } \
\
protected:\
virtual IObject3D* getParent() const { return X::getParent(); } \
virtual void setParent(IObject3D* parent) { X::setParent(parent); } \
\
virtual void setWorldMatrixDirty(byte cause) { X::setWorldMatrixDirty(cause); } \
virtual void setLocalMatrixDirty(byte cause) { X::setLocalMatrixDirty(cause); } \
\
virtual void calculateWorldMatrix() { X::calculateWorldMatrix(); }


class Object3D : public IObject3D
{
public:
    Object3D();
    virtual ~Object3D();
    
    virtual void setLocalTranslate(const vec3& translate);
    virtual void setLocalRotate(const mat33& rotate);
    virtual void setLocalScale(const vec3& scale);

    virtual const vec3&  getLocalTranslate() const;
    virtual const mat33& getLocalRotate() const;
    virtual const vec3&  getLocalScale() const;

    virtual const mat44& getLocalMatrix() const;
    virtual const mat44& getWorldMatrix() const;

    virtual void attach(IObject3D* child);
    virtual void detach(IObject3D* child);

protected:
    virtual void setWorldMatrixDirty(byte cause); // sets all childs to dirty as well, if dirty it presumes all childs are already dirty
    virtual void setLocalMatrixDirty(byte cause); // sets all childs to dirty as well, if dirty it presumes all childs are already dirty

    virtual void calculateWorldMatrix() const;

    virtual IObject3D* getParent() const { return m_Parent; }
    virtual void setParent(IObject3D* parent) { m_Parent = parent; }
    
    enum DirtyFlag // 4 bit MAX
    {
        DirtyFlag_Translate = 1 << 0,
        DirtyFlag_Rotate = 1 << 1,
        DirtyFlag_Scale = 1 << 2,
        DirtyFlag_Link = DirtyFlag_Translate | DirtyFlag_Rotate | DirtyFlag_Scale | 1 << 3
    };

    IObject3D* m_Parent;
    std::vector<IObject3D*> m_Childs;

    mutable byte m_LocalMatrixDirty : 4;
    mutable byte m_WorldMatrixDirty : 4;

    mutable mat44 m_WorldMatrix;
    mutable mat44 m_LocalMatrix;

    vec3  m_LocalTranslate;
    mat33 m_LocalRotation;
    vec3  m_LocalScale;
};   

} //end namespace

#endif
