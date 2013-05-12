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

#include "Object3D.h"

namespace he {
class Bound;
namespace gfx {

class ICamera;
class ModelMesh;
class Material;
class CullOctreeNode;
class Scene;

class HAPPY_ENTRY Drawable
{
public:
    Drawable(): m_Node(nullptr) {}
    virtual ~Drawable() {}

    virtual const Material* getMaterial() const = 0;
    virtual void applyMaterial(const ICamera* pCamera) const = 0;
    virtual void applyMaterial(const Material* customMaterial, const ICamera* pCamera) const = 0;

    virtual const ModelMesh* getModelMesh() const = 0;
    
    virtual bool getCastsShadow() const = 0;
    virtual void setCastsShadow(bool castShadow) = 0;
    
    virtual void calculateBound() = 0;
    virtual const Bound& getBound() const = 0;

    virtual void detachFromScene() = 0;
    virtual void attachToScene(Scene* scene) = 0;
    virtual void setScene(Scene* scene) = 0;
    virtual Scene* getScene() const = 0;
    virtual bool isAttachedToScene() const = 0;
        
    virtual void draw() = 0;
    virtual void drawShadow() = 0;

    virtual bool isSingle() const = 0;
    virtual bool isInstanced() const = 0;
    virtual bool isSkinned() const = 0;
    
    //////////////////////////////////////////////////////////////////////////
    CullOctreeNode* getNode() const { return m_Node; };
    void setNode(CullOctreeNode* node) { m_Node = node; };
    virtual void nodeReevaluated() {} // callback

private:
    CullOctreeNode* m_Node;
}; 

} } //end namespace

#endif
