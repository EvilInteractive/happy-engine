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

#ifndef _HE_DEFAULT_SINGLE_DRAWABLE_H_
#define _HE_DEFAULT_SINGLE_DRAWABLE_H_
#pragma once

#include "IDrawable.h"
#include "Bound.h"

namespace he {
namespace gfx {

class DefaultSingleDrawable : public SingleDrawable
{
public:
    DefaultSingleDrawable();
    virtual ~DefaultSingleDrawable();

    virtual const Material* getMaterial() const = 0;
    virtual void applyMaterial(const ICamera* pCamera) const; 
    virtual void applyMaterial(const Material* customMaterial, const ICamera* pCamera) const;

    virtual const ModelMesh* getModelMesh() const = 0;

    virtual bool getCastsShadow() const;
    virtual void setCastsShadow(bool castShadow);

    virtual void calculateBound();
    virtual const Bound& getBound() const;

    virtual void draw();
    virtual void drawShadow();

    virtual void detachFromScene();
    virtual void attachToScene(Scene* scene);
    virtual void setScene(Scene* scene);
    virtual Scene* getScene() const;
    virtual bool isAttachedToScene() const;

    virtual void nodeReevaluated() { m_NeedsReevalute = false; }
    
protected:
    virtual void setWorldMatrixDirty(uint8 cause);
    
private:
    bool m_CastsShadow;
    Scene* m_Scene;
    Bound m_Bound;
    bool m_NeedsReevalute;

    //Disable default copy constructor and default assignment operator
    DefaultSingleDrawable(const DefaultSingleDrawable&);
    DefaultSingleDrawable& operator=(const DefaultSingleDrawable&);
};

} } //end namespace

#endif
