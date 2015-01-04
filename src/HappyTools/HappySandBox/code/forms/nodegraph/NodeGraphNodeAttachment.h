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

#ifndef _HS_NODEGRAPHNODEATTACHMENT_H_
#define _HS_NODEGRAPHNODEATTACHMENT_H_
#pragma once

#include <DefaultLayoutable.h>

namespace he {
    struct RectF;
namespace io {
    class StructuredVisitor;
}
}

namespace hs {
struct NodeGraphDrawContext;
class NodeGraphNode;
class NodeGraphNodeConnector;
class NodeGraphNodeAttachment : public he::gui::ILayoutable
{
public:
    explicit NodeGraphNodeAttachment(NodeGraphNode* parent);
    virtual ~NodeGraphNodeAttachment() {}

    // Connector
    virtual NodeGraphNodeConnector* pickNodeConnector(const he::vec2& worldPos) = 0;
    virtual NodeGraphNodeConnector* getNodeConnector() = 0;

    // State
    virtual void move(const he::vec2& worldDelta) = 0;
    virtual NodeGraphNode* getParent() const { return m_Parent; }
    
    // Draw
    virtual void draw(const NodeGraphDrawContext& context) = 0;

    // Serialization
    virtual void visit(he::io::StructuredVisitor* const /*visitor*/) {}

private:
    NodeGraphNode* m_Parent;
};

}

#endif
