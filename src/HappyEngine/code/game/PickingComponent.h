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
//Created: 2013-03-02

#ifndef _HE_PickingComponent_H_
#define _HE_PickingComponent_H_
#pragma once

#include "EntityComponent.h"
#include "Pickable.h"
#include "Object3D.h"

namespace he {

namespace gfx {
    class ModelMesh;
}

namespace ge {
    
class HAPPY_ENTRY PickingComponent : public Pickable, public EntityComponent, public Object3D
{
    IMPLEMENT_IOBJECT3D_FROM(Object3D)
public:
    PickingComponent();
    virtual ~PickingComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                            ///
    //////////////////////////////////////////////////////////////////////////
    virtual void init(Entity* parent);

    virtual void visit(he::io::BinaryVisitor* const /*visitor*/) {}

    virtual void activate();
    virtual void deactivate();

    virtual const he::FixedString& getComponentID() const { return HEFS::strPickingComponent; }
    //////////////////////////////////////////////////////////////////////////
           
protected:
    virtual void getPickingData(const vec3*& outVertices, const void*& outIndices, gfx::IndexStride& outIndexStride, size_t& outTriangleCount) const;   // Local space
    virtual const Bound& getPickingBound() const; // Local space
    virtual const mat44& getPickingWorld() const;

    Entity* m_Parent;

private:
    void initPickingMesh();

    he::eventCallback0<void> m_OnNewPickingMesh;
    const gfx::ModelMesh* m_ModelMesh;
    ObjectHandle m_LinkedModelComponent;

    //Disable default copy constructor and default assignment operator
    PickingComponent(const PickingComponent&);
    PickingComponent& operator=(const PickingComponent&);
};

} } //end namespace

#endif
