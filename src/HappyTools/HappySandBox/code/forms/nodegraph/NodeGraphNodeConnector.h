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

#include <DefaultLayoutable.h>

namespace hs {
class NodeGraphNodeAttachment;
struct NodeGraphDrawContext;

enum ENodeGraphNodeConnectorType
{
    eNodeGraphNodeConnectorType_Input,
    eNodeGraphNodeConnectorType_Output
};

class NodeGraphNodeConnector : public he::gui::DefaultLayoutable
{
public:
    enum EConnectorState
    {
        eConnectorState_Normal,
        eConnectorState_Hover,
        eConnectorState_Selected,

        eConnectorState_MAX
    };
    struct ConnectorStyle
    {
        ConnectorStyle();
        he::vec2 m_ConnectorSize;
        he::Color m_ConnectorBackgroundColor[eConnectorState_MAX];
        he::Color m_ConnectorBorderColor[eConnectorState_MAX];
    };

    NodeGraphNodeConnector();
    virtual ~NodeGraphNodeConnector();

    // Style
    void setConnectorStyle(const ConnectorStyle& style);
    const ConnectorStyle& getConnectorStyle() const { return m_Style; }

    EConnectorState getState() const { return m_State; }
    void setState(const EConnectorState state) { m_State = state; }

    ENodeGraphNodeConnectorType getType() const { return m_Type; }
    virtual void setType(const ENodeGraphNodeConnectorType type) { m_Type = type; }

    // Returns true if something actually changed
    virtual bool connect(NodeGraphNodeConnector* other);
    virtual bool disconnect(NodeGraphNodeConnector* other);
    
    void disconnectAll();

    bool isConnected() const { return m_Connections.size() != 0; }
    const he::PrimitiveList<NodeGraphNodeConnector*>& getConnections() const { return m_Connections; }

    // Picking
    bool pick(const he::vec2& worldPos) const;

    void draw(const NodeGraphDrawContext& context);
    void drawConnection(const NodeGraphDrawContext& context, const NodeGraphNodeConnector& other);
    void drawConnection(const NodeGraphDrawContext& context, const he::vec2& other);
    
private:
    NodeGraphNodeAttachment* m_Parent;
    he::PrimitiveList<NodeGraphNodeConnector*> m_Connections;
    EConnectorState m_State;
    ConnectorStyle m_Style;
    ENodeGraphNodeConnectorType m_Type;

    //Disable default copy constructor and default assignment operator
    NodeGraphNodeConnector(const NodeGraphNodeConnector&);
    NodeGraphNodeConnector& operator=(const NodeGraphNodeConnector&);
};

}

#endif
