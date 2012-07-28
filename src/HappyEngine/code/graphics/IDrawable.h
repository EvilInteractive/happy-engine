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

#ifndef _HE_IDRAWABLE_H_
#define _HE_IDRAWABLE_H_
#pragma once

#include "I3DObject.h"
#include "IPickable.h"

namespace he {
class Bound;
namespace gfx {

class ICamera;
class ModelMesh;
class Material;
class CullOctreeNode;
class Scene;

class IDrawable : public IPickable
{
public:
    IDrawable(): m_Node(nullptr) {}
    virtual ~IDrawable() {}

    virtual const Material* getMaterial() const = 0;
    virtual void applyMaterial(const ICamera* pCamera) const = 0;
    virtual void applyMaterial(const Material* customMaterial, const ICamera* pCamera) const = 0;

    virtual const ModelMesh* getModelMesh() const = 0;
    
    virtual bool getCastsShadow() const = 0;
    virtual void setCastsShadow(bool castShadow) = 0;
    
    virtual const Bound& getBound() const = 0;

    virtual void detachFromScene() = 0;
    virtual void attachToScene(Scene* scene, bool dynamic) = 0;
    virtual Scene* getScene() const = 0;
    virtual bool isAttachedToScene() const = 0;
        
    virtual void draw() = 0;
    virtual void drawShadow() = 0;

    virtual bool isSingle() const = 0;
    virtual bool isInstanced() const = 0;
    virtual bool isSkinned() const = 0;

    // needs to reevaluate octree node every draw when dynamicly inserted
    // if sleeping this object will be ignored when updating dynamic bounds
    // Hint: set this value to physics->isSleeping()
    virtual bool isSleeping() const = 0; 

    //////////////////////////////////////////////////////////////////////////
    CullOctreeNode* getNode() const { return m_Node; };
    void setNode(CullOctreeNode* node) { m_Node = node; };

private:
    CullOctreeNode* m_Node;
}; 

class ISingleDrawable : public I3DObject, public IDrawable
{
public:
    virtual ~ISingleDrawable() {}

    virtual bool isSingle() const { return true; }
    virtual bool isInstanced() const { return false; }
    virtual bool isSkinned() const { return false; }

}; 

class IInstancedDrawable : public IDrawable
{
public:
    virtual ~IInstancedDrawable() {}
    
    virtual bool isSingle() const { return false; }
    virtual bool isInstanced() const { return true; }
    virtual bool isSkinned() const { return false; }

};

class ISkinnedDrawable : public I3DObject, public IDrawable
{
public:
    virtual ~ISkinnedDrawable() {}

    virtual const std::vector<mat44>& getBoneTransforms() const = 0;

    virtual bool isSingle() const { return true; }
    virtual bool isInstanced() const { return false; }
    virtual bool isSkinned() const { return true; }

};

} } //end namespace

#endif
