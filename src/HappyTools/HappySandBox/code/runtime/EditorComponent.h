//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 2013/04/13

#ifndef _HT_EditorComponent_H_
#define _HT_EditorComponent_H_
#pragma once

#include <EntityComponent.h>
#include <Object3D.h>
#include <IShapeDrawable.h>
#include <AABB.h>

namespace he {
namespace ge {
    class PickingComponent;
} }

namespace hs {

class EditorComponent : public he::ge::EntityComponent, public he::gfx::IShapeDrawable, public he::Object3D
{
    IMPLEMENT_IOBJECT3D_FROM(Object3D)
public:
    EditorComponent();
    virtual ~EditorComponent();

    void init(he::ge::Entity* parent);  // override

    void visit(he::io::BinaryVisitor* const /*visitor*/) {}  // override

    void activate();  // override
    void deactivate();  // override
     
    const he::FixedString& getComponentID() const { return HSFS::strEditorComponent; }  // override

    const he::AABB& getBound() const { return m_AABB; }

    void computeBound();
    void setSelected(const bool selected);

    void drawShapes(he::gfx::ShapeRenderer* const renderer); // override

private:
    he::ge::Entity* m_Parent;
    he::ge::PickingComponent* m_PickingComponent;

    bool m_IsSelected;
    he::AABB m_AABB;

    /* DEFAULT COPY & ASSIGNMENT */
    EditorComponent(const EditorComponent&);
    EditorComponent& operator=(const EditorComponent&);
};

} //end namespace

#endif
