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
#include "HappySandBoxPCH.h"
#include "NodeGraphNodeConnector.h"

#include "NodeGraphEnums.h"
#include "NodeGraphNode.h"

#include <Canvas2D.h>

namespace hs {

NodeGraphNodeConnector::ConnectorStyle::ConnectorStyle()
    : m_ConnectorSize(8, 8)
{
    m_ConnectorBackgroundColor[eConnectorState_Normal] = he::Color(126, 109, 95, static_cast<he::uint8>(255));
    m_ConnectorBackgroundColor[eConnectorState_Hover] = m_ConnectorBackgroundColor[eConnectorState_Normal];
    m_ConnectorBackgroundColor[eConnectorState_Selected] = he::Color(171, 148, 129, static_cast<he::uint8>(255));

    m_ConnectorBorderColor[eConnectorState_Normal] = he::Color(149, 106, 79, static_cast<he::uint8>(255));
    m_ConnectorBorderColor[eConnectorState_Hover] = he::Color(197, 139, 104, static_cast<he::uint8>(255));
    m_ConnectorBorderColor[eConnectorState_Selected] = he::Color(255, 180, 135, static_cast<he::uint8>(255));
}

NodeGraphNodeConnector::NodeGraphNodeConnector()
    : m_Bound(0, 0, 0, 0)
    , m_Margin(0, 4, 0, 4)
    , m_ContentBound(0, 0, 0, 0)
    , m_ContentSize(0, 0)
    , m_ContentMargin(4, 4, 4, 4)
    , m_State(eConnectorState_Normal)
    , m_Parent(nullptr)
{

}

NodeGraphNodeConnector::~NodeGraphNodeConnector()
{

}

void NodeGraphNodeConnector::setConnectorStyle(const NodeGraphNodeConnector::ConnectorStyle& style)
{
    m_ConnectorStyle = style;
    calculateBound();
}

bool NodeGraphNodeConnector::isInsideConnector( const he::vec2& worldPos ) const 
{
    return m_Bound.isInside(worldPos);
}

void NodeGraphNodeConnector::setBound(const he::RectF& bound)
{
    m_Bound = bound;
    m_ContentBound = bound;

    m_ContentBound.y += m_ContentMargin.y;
    m_ContentBound.height -= m_ContentMargin.y + m_ContentMargin.w;
    switch (getLayoutAlignment())
    {
    case eLayoutAlignment_Left:
        {
            m_ContentBound.x += m_ConnectorStyle.m_ConnectorSize.x + m_ContentMargin.x;
            m_ContentBound.width -= m_ConnectorStyle.m_ConnectorSize.x + m_ContentMargin.x + m_ContentMargin.z;
        } break;
    case eLayoutAlignment_Right:
        {
            m_ContentBound.x += m_ContentMargin.x;
            m_ContentBound.width -= m_ContentMargin.x + m_ContentMargin.z + m_ConnectorStyle.m_ConnectorSize.x;
        } break;
    default: LOG(he::LogType_ProgrammerAssert, "Invalid alignment type for NodeConnector");
    }
}

void NodeGraphNodeConnector::setContentSize(const he::vec2& contentSize)
{
    m_ContentSize = contentSize;
    calculateBound();
}

void NodeGraphNodeConnector::setContentMargin(const he::vec4& contentMargin)
{
    m_ContentMargin = contentMargin;
    calculateBound();
}

void NodeGraphNodeConnector::calculateBound()
{
    m_Bound.width = m_ConnectorStyle.m_ConnectorSize.x + m_ContentMargin.x + m_ContentMargin.z + m_ContentSize.x;
    m_Bound.height = std::max(m_ConnectorStyle.m_ConnectorSize.y, m_ContentMargin.y + m_ContentMargin.w + m_ContentSize.y);

    if (m_Parent)
        m_Parent->setLayoutDirty();
}

void NodeGraphNodeConnector::drawConnector(const NodeGraphDrawContext& context)
{
    const he::RectF& bound(getBound());
    he::vec2 pos(0, bound.y + bound.height / 2.0f - m_ConnectorStyle.m_ConnectorSize.y / 2.0f);
    switch (getLayoutAlignment())
    {
    case eLayoutAlignment_Left:
        {
            pos.x = bound.x;
        } break;
    case eLayoutAlignment_Right:
        {
            pos.x = bound.x + bound.width - m_ConnectorStyle.m_ConnectorSize.x;
        } break;
    default: LOG(he::LogType_ProgrammerAssert, "Invalid alignment type for NodeConnector");
    }

    he::RectI rect(he::RectF(pos.x, pos.y, m_ConnectorStyle.m_ConnectorSize.x, m_ConnectorStyle.m_ConnectorSize.y ).transform(context.transform));

    context.canvas->setColor(m_ConnectorStyle.m_ConnectorBackgroundColor[m_State]);
    context.canvas->fillRect(rect);

    context.canvas->setColor(m_ConnectorStyle.m_ConnectorBorderColor[m_State]);
    context.canvas->fillRect(rect);
}

} //end namespace

