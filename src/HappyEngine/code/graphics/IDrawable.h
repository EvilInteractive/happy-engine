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
namespace gfx {

class ICamera;
class ModelMesh;
class Material;

class IDrawable : public IPickable
{
public:
    virtual ~IDrawable() {}

    virtual const Material* getMaterial() const = 0;
    virtual void applyMaterial(const ICamera* pCamera) const = 0;
    virtual void applyMaterial(const Material* customMaterial, const ICamera* pCamera) const = 0;

    virtual const ModelMesh* getModelMesh() const = 0;
    
    virtual bool getCastsShadow() const = 0;
    virtual void setCastsShadow(bool castShadow) = 0;

    virtual bool isVisible() const = 0;
    virtual void setVisible(bool visible) = 0;

    virtual bool isInCamera(const ICamera* pCamera) const = 0; 
    virtual float getDrawPriority(const ICamera* pCamera) const = 0; 

    virtual void draw() = 0;
    virtual void drawShadow() = 0;

    virtual bool isSingle() const = 0;
    virtual bool isInstanced() const = 0;
    virtual bool isSkinned() const = 0;
}; 

class ISingleDrawable : public I3DObject, public IDrawable
{
public:
    virtual ~ISingleDrawable() {}

    virtual const Material* getMaterial() const = 0;
    virtual void applyMaterial(const ICamera* pCamera) const = 0; 
    virtual void applyMaterial(const Material* customMaterial, const ICamera* pCamera) const = 0;

    virtual const ModelMesh* getModelMesh() const = 0;

    virtual mat44 getWorldMatrix() const = 0;

    virtual bool getCastsShadow() const = 0;
    virtual void setCastsShadow(bool castShadow) = 0;

    virtual bool isVisible() const = 0;
    virtual void setVisible(bool visible) = 0;

    virtual bool isInCamera(const ICamera* pCamera) const = 0; 
    virtual float getDrawPriority(const ICamera* pCamera) const = 0; 

    virtual void draw() = 0;
    virtual void drawShadow() = 0;

    virtual bool isSingle() const { return true; }
    virtual bool isInstanced() const { return false; }
    virtual bool isSkinned() const { return false; }

}; 

class IInstancedDrawable : public IDrawable
{
public:
    virtual ~IInstancedDrawable() {}

    virtual const Material* getMaterial() const = 0;
    virtual void applyMaterial(const ICamera* pCamera) const = 0;
    virtual void applyMaterial(const Material* customMaterial, const ICamera* pCamera) const = 0;

    virtual const ModelMesh* getModelMesh() const = 0;

    virtual bool getCastsShadow() const = 0;
    virtual void setCastsShadow(bool castShadow) = 0;

    virtual bool isVisible() const = 0;
    virtual void setVisible(bool visible) = 0;

    virtual bool isInCamera(const ICamera* pCamera) const = 0; 
    virtual float getDrawPriority(const ICamera* pCamera) const = 0; 

    virtual uint getCount() const = 0;

    virtual void draw() = 0;
    virtual void drawShadow() = 0;

    virtual bool isSingle() const { return false; }
    virtual bool isInstanced() const { return true; }
    virtual bool isSkinned() const { return false; }

};

class ISkinnedDrawable : public I3DObject, public IDrawable
{
public:
    virtual ~ISkinnedDrawable() {}

    virtual const Material* getMaterial() const = 0;
    virtual void applyMaterial(const ICamera* pCamera) const = 0;
    virtual void applyMaterial(const Material* customMaterial, const ICamera* pCamera) const = 0;

    virtual const ModelMesh* getModelMesh() const = 0;
    virtual const std::vector<mat44>& getBoneTransforms() const = 0;

    virtual mat44 getWorldMatrix() const = 0;

    virtual bool getCastsShadow() const = 0;
    virtual void setCastsShadow(bool castShadow) = 0;

    virtual bool isVisible() const = 0;
    virtual void setVisible(bool visible) = 0;

    virtual bool isInCamera(const ICamera* pCamera) const = 0; 
    virtual float getDrawPriority(const ICamera* pCamera) const = 0; 

    virtual void draw() = 0;
    virtual void drawShadow() = 0;

    virtual bool isSingle() const { return true; }
    virtual bool isInstanced() const { return false; }
    virtual bool isSkinned() const { return true; }

};

} } //end namespace

#endif
