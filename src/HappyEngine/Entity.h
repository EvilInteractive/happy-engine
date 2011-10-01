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

#ifndef _HE_ENTITY_H_
#define _HE_ENTITY_H_
#pragma once

#include "Texture2D.h"
#include "Model.h"
#include "Color.h"
#include "Matrix.h"
#include <string>
#include "Material.h"
#include "IDrawable.h"
#include "PhysicsMaterial.h"

namespace happyengine {
namespace game {


struct EntityPhysicsDesc
{
    bool usePhysics;
    bool isDynamic;
    float density;
    std::string shape;
    physics::PhysicsMaterial::pointer pMaterial;
};
struct EntityDesc
{
    graphics::Model::pointer pModel;
    graphics::Material material;
    std::string className;
    EntityPhysicsDesc physicsDesc;
};


class Entity : public graphics::IDrawable
{
public:
    virtual ~Entity();

    virtual void tick(float /*dTime*/) {}

    virtual const math::Matrix& getWorldMatrix() const;
    virtual const graphics::Material& getMaterial() const;
    virtual const graphics::Model::pointer getModel() const;

protected:
    //Protected constructor
	Entity(const EntityDesc& desc);

    void setModel(const graphics::Model::pointer& pModel);
    void setWorldMatrix(const math::Matrix& mtxWorld);

private:
    graphics::Model::pointer m_pModel;
    graphics::Material m_Material;

    math::Matrix m_mtxWorld;

    //Disable default copy constructor and default assignment operator
    Entity(const Entity&);
    Entity& operator=(const Entity&);
};

} } //end namespace

#endif
