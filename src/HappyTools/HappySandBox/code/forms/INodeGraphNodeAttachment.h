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

#ifndef _HS_INODEGRAPHNODEATTACHMENT_H_
#define _HS_INODEGRAPHNODEATTACHMENT_H_
#pragma once

namespace he {
    struct RectF;
}

namespace hs {
struct  NodeGraphDrawContext;
class INodeGraphNodeAttachment
{
public:
    enum EConnectorState
    {
        eConnectorState_Normal,
        eConnectorState_Hover,
        eConnectorState_Selected
    };
    enum ELayoutAlignment
    {
        eLayoutAlignment_Left,
        eLayoutAlignment_Right,
        eLayoutAlignment_Center,
    };

    virtual ~INodeGraphNodeAttachment() {}

    // Layout
    virtual bool needsLayoutUpdate() const = 0;
    virtual void setLayoutUpdated() = 0;

    virtual ELayoutAlignment getLayoutAlignment() const = 0;
    virtual const he::vec4& getLayoutMargin() const = 0;

    virtual void setBound(const he::RectF& bound) = 0;
    virtual const he::RectF& getBound() const = 0;

    // Connector
    virtual bool hasConnector() const = 0;
    virtual bool isInsideConnector(const he::vec2& worldPos) const = 0;
    virtual void setConnectorState(const EConnectorState state) = 0;
    virtual EConnectorState getConnectorState() const = 0;

    // Draw
    virtual void draw(const NodeGraphDrawContext& context) = 0;
};

}

#endif
