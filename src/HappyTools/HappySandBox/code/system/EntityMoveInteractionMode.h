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
//Created: 2015/02/10

#ifndef _HT_EntityMoveInteractionMode_H_
#define _HT_EntityMoveInteractionMode_H_
#pragma once

#include "IInteractionMode.h"

#include <IShapeDrawable.h>
#include <PickingData.h>

namespace he {
namespace gfx {
    class ShapeMesh;
} }

namespace hs {

class EntityMoveInteractionMode : public IInteractionMode, public he::gfx::IShapeDrawable
{
public:
    EntityMoveInteractionMode();
    ~EntityMoveInteractionMode();

    void activate() final;
    void deactivate() final;

    void tick(const float dTime) final;

private:
    void createGizmo();
    void destroyGizmo();
    void onSelectionChanged();

    void drawShapes(he::gfx::ShapeRenderer* const renderer) final;

    bool m_IsActive;
    he::eventCallback0<void> m_SelectionChangedCallback;

    he::vec3 m_Position;
    float m_Size;
    bool m_Visible;

    he::gfx::ShapeMesh* m_Arrow;
    he::gfx::PickingData* m_ArrowPicking;
    he::gfx::PickingData* m_AxisPicking;
    he::gfx::PickingData* m_DoubleAxisPicking;

    enum EAxis
    {
        eAxis_X,
        eAxis_Y,
        eAxis_Z,
        eAxis_MAX
    };
    he::Color m_AxisColor[eAxis_MAX];
    he::Color m_SelectedAxisColor;

    /* DEFAULT COPY & ASSIGNMENT */
    EntityMoveInteractionMode(const EntityMoveInteractionMode&);
    EntityMoveInteractionMode& operator=(const EntityMoveInteractionMode&);
};

} //end namespace

#endif
