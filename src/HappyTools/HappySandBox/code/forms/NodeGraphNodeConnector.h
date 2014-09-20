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

#ifndef _HS_NODEGRAPHNODECONNECTOR_H_
#define _HS_NODEGRAPHNODECONNECTOR_H_
#pragma once

#include "INodeGraphNodeAttachment.h"

namespace hs {

class NodeGraphNodeConnector : public INodeGraphNodeAttachment
{
protected:
    enum EFlags
    {
        eFlags_NeedsLayoutUpdate,
        eFlags_User
    };
public:
    NodeGraphNodeConnector();
    virtual ~NodeGraphNodeConnector();

    // INodeGraphNodeAttachment
    virtual bool needsLayoutUpdate() const override { return (m_Flags & eFlags_NeedsLayoutUpdate) != 0; }
    virtual void setLayoutUpdated() override { m_Flags &= ~eFlags_NeedsLayoutUpdate; }

    virtual ELayoutAlignment getLayoutAlignment() const override = 0;
    virtual const he::vec4& getLayoutMargin() const override = 0;

    virtual void setBound(const he::RectF& bound) override { m_ConnectorBound = bound; }
    virtual const he::RectF& getBound() const override { return m_ConnectorBound; }

    virtual bool hasConnector() const override { return true; }
    virtual bool isInsideConnector(const he::vec2& worldPos) const override;
    virtual void setConnectorState(const EConnectorState state) override { m_State = state; }
    virtual EConnectorState getConnectorState() const override { return m_State; }

    virtual void draw(const NodeGraphDrawContext& context) override;

private:
    he::uint8 m_Flags;
    EConnectorState m_State : 8;
    he::RectF m_ConnectorBound;
};

}

#endif
