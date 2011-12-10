//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include "ModelMesh.h"
#include "Material.h"
#include "I3DObject.h"

namespace he {
namespace gfx {

class IDrawable : public I3DObject
{
public:
    virtual ~IDrawable() {}

    virtual const Material& getMaterial() const = 0;
    virtual const ModelMesh::pointer& getModel() const = 0;
    virtual const std::vector<mat44>& getBoneTransforms() const { return m_NoBones; };

    virtual mat44 getWorldMatrix() const = 0;

    virtual bool getCastsShadow() const = 0;

    virtual bool isVisible() const { return m_IsVisible; }
    virtual void setVisible(bool visible) { m_IsVisible = visible; }

protected:
    bool m_IsVisible;

private:
    std::vector<mat44> m_NoBones;
};  


class ISkinnedDrawable : public IDrawable
{
public:
    virtual ~ISkinnedDrawable() {}

    virtual const Material& getMaterial() const = 0;
    virtual const ModelMesh::pointer& getModel() const = 0;
    virtual const std::vector<mat44>& getBoneTransforms() const = 0;

    virtual mat44 getWorldMatrix() const = 0;

    virtual bool getCastsShadow() const = 0;

};

} } //end namespace

#endif
