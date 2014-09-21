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
public:
    struct ConnectorStyle
    {
        ConnectorStyle();
        he::vec2 m_ConnectorSize;
        he::Color m_ConnectorBackgroundColor[INodeGraphNodeAttachment::eConnectorState_MAX];
        he::Color m_ConnectorBorderColor[INodeGraphNodeAttachment::eConnectorState_MAX];
    };

    NodeGraphNodeConnector();
    virtual ~NodeGraphNodeConnector();

    // Style
    void setConnectorStyle(const ConnectorStyle& style);
    const ConnectorStyle& getConnectorStyle() const { return m_ConnectorStyle; }

    // INodeGraphNodeAttachment
    virtual void setParent(NodeGraphNode* const parent) override { m_Parent = parent; }

    virtual ELayoutAlignment getLayoutAlignment() const override = 0;
    virtual const he::vec4& getLayoutMargin() const override { return m_Margin; }

    virtual void setBound(const he::RectF& bound) override;
    virtual const he::RectF& getBound() const override { return m_Bound; }

    virtual bool hasConnector() const override { return true; }
    virtual bool isInsideConnector(const he::vec2& worldPos) const override;
    virtual void setConnectorState(const EConnectorState state) override { m_State = state; }
    virtual EConnectorState getConnectorState() const override { return m_State; }

    virtual void draw(const NodeGraphDrawContext& context) override = 0;

protected:
    void setContentSize(const he::vec2& contentSize);
    const he::vec2& getContentSize() const { return m_ContentSize; }

    void setContentMargin(const he::vec4& contentMargin);
    const he::vec4& getContentMargin() const { return m_ContentMargin; }

    const he::RectF& getContentBound() const { return m_ContentBound; }

    void drawConnector(const NodeGraphDrawContext& context);

    NodeGraphNode* m_Parent;

private:
    void calculateBound();

    he::RectF m_Bound;
    he::vec4 m_Margin;

    he::RectF m_ContentBound;
    he::vec2 m_ContentSize;
    he::vec4 m_ContentMargin;

    ConnectorStyle m_ConnectorStyle;

    EConnectorState m_State;
};

}

#endif
