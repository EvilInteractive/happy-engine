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
class IPickingManager;
  
struct EntityComponentDesc;  
class HAPPY_ENTRY PickingComponent : public Pickable, public EntityComponent
{
public:
    PickingComponent();
    ~PickingComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                            ///
    //////////////////////////////////////////////////////////////////////////
    void init(Entity* parent);  // override, final
    void setPickingManager(he::ge::IPickingManager* const manager);

    void visit(he::io::BinaryVisitor* const /*visitor*/) {}  // override, final

    void activate(); // override, final
    void deactivate(); // override, final

    const he::FixedString& getComponentID() const { return HEFS::strPickingComponent; }

    //// Editor //////////////////////////////////////////////////////////////
    static void fillEntityComponentDesc(EntityComponentDesc& desc);
    bool setProperty(const Property* const inProperty); // override, final
    bool getProperty(Property* const inOutProperty); // override, final
    //////////////////////////////////////////////////////////////////////////
           
private:
    void getPickingData(const vec3*& outVertices, const void*& outIndices, gfx::IndexStride& outIndexStride, size_t& outTriangleCount) const;   // Local space
    const Bound& getPickingBound() const; // Local space
    const mat44& getPickingWorld() const;

    Entity* m_Parent;

    void initPickingMesh();

    he::eventCallback0<void> m_OnNewPickingMesh;
    const gfx::ModelMesh* m_ModelMesh;
    ObjectHandle m_LinkedModelComponent;
    he::ge::IPickingManager* m_PickingManager;

    //Disable default copy constructor and default assignment operator
    PickingComponent(const PickingComponent&);
    PickingComponent& operator=(const PickingComponent&);
};

} } //end namespace

#endif
