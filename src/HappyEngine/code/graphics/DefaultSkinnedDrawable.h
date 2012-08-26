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

#ifndef _HE_DEFAULT_SKINNED_DRAWABLE_H_
#define _HE_DEFAULT_SKINNED_DRAWABLE_H_
#pragma once

#include "IDrawable.h"
#include "Bound.h"

namespace he {
namespace gfx {

class DefaultSkinnedDrawable : public ISkinnedDrawable
{
public:
    DefaultSkinnedDrawable();
    virtual ~DefaultSkinnedDrawable();

    virtual const Material* getMaterial() const = 0;
    virtual void applyMaterial(const ICamera* pCamera) const; 
    virtual void applyMaterial(const Material* customMaterial, const ICamera* pCamera) const;

    virtual const ModelMesh* getModelMesh() const = 0;
    virtual const std::vector<mat44>& getBoneTransforms() const = 0;
    virtual mat44 getWorldMatrix() const = 0;

    virtual void calculateBound();
    virtual const Bound& getBound() const { return m_Bound; }

    virtual bool getCastsShadow() const;
    virtual void setCastsShadow(bool castShadow);

    virtual void draw();
    virtual void drawShadow();

    virtual void detachFromScene();
    virtual void attachToScene(Scene* scene, bool dynamic);
    virtual void setScene(Scene* scene);
    virtual Scene* getScene() const;
    virtual bool isAttachedToScene() const;

protected:
    Bound m_Bound;

private:
    bool m_CastsShadow;
    Scene* m_Scene;

    //Disable default copy constructor and default assignment operator
    DefaultSkinnedDrawable(const DefaultSkinnedDrawable&);
    DefaultSkinnedDrawable& operator=(const DefaultSkinnedDrawable&);
};

} } //end namespace

#endif
