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
#include <Pickable.h>
#include <Bound.h>

namespace he {
    class Ray;
namespace gfx {
    class ShapeMesh;
} 
namespace ge {
    class PickResult;
}
}


namespace hs {

class EntityMoveInteractionMode : public he::ge::Pickable, public IInteractionMode, public he::gfx::IShapeDrawable
{
public:
    EntityMoveInteractionMode();
    ~EntityMoveInteractionMode();

    void activate() final;
    void deactivate() final;

    void tick(const float dTime) final;

private:
    enum EAxis
    {
        eAxis_X,
        eAxis_Y,
        eAxis_Z,
        eAxis_MAX
    };

    enum ELookat
    {
        eLookat_X,
        eLookat_Y,
        eLookat_Z,
        eLookat_XZ,
        eLookat_MAX
    };

    enum EPickingPass
    {
        ePickPass_XArrow,
        ePickPass_XAxis,
        ePickPass_XYAxis,
        ePickPass_YArrow,
        ePickPass_YAxis,
        ePickPass_YZAxis,
        ePickPass_ZArrow,
        ePickPass_ZAxis,
        ePickPass_XZAxis,
        ePickPass_MAX
    };

    bool getPickingData(const he::vec3*& outVertices, const void*& outIndices, he::gfx::IndexStride& outIndexStride, size_t& outTriangleCount) const final;   // Local space
    const he::Bound& getPickingBound() const final; // Local space
    const he::mat44& getPickingWorld() const final;

    bool onPick(const he::Ray& ray, he::ge::PickResult& result);
    bool onHover(const he::Ray& ray, he::ge::PickResult& result);

    void createGizmo();
    void destroyGizmo();
    void onSelectionChanged();

    he::vec3 getMouseWorldPos(const he::uint8 axi, const he::vec2& mousePos) const;

    void drawShapes(he::gfx::ShapeRenderer* const renderer) final;

    bool m_IsActive;
    he::eventCallback0<void> m_SelectionChangedCallback;
    he::eventCallback2<bool, const he::Ray&, he::ge::PickResult&> m_OnHover;
    he::eventCallback2<bool, const he::Ray&, he::ge::PickResult&> m_OnPick;

    he::vec3 m_Position;
    he::mat44 m_AxisWorld[eLookat_MAX];
    float m_Size;
    bool m_Visible;

    he::vec3 m_StartMovePos;
    he::vec3 m_LastMovePos;
    bool m_MoveActive;

    he::gfx::ShapeMesh* m_Arrow;
    he::gfx::PickingData* m_ArrowPicking;
    he::gfx::PickingData* m_AxisPicking;
    he::gfx::PickingData* m_DoubleAxisPicking;
    he::Color m_AxisColor[eAxis_MAX];
    he::Color m_SelectedAxisColor;
    he::Bound m_Bound;
    he::uint8 m_SelectedAxi;
    EPickingPass m_PickPass;

    inline bool isSelected(const EAxis axis) const { return (m_SelectedAxi & BIT(axis)) != 0; }

    /* DEFAULT COPY & ASSIGNMENT */
    EntityMoveInteractionMode(const EntityMoveInteractionMode&);
    EntityMoveInteractionMode& operator=(const EntityMoveInteractionMode&);
};

} //end namespace

#endif
