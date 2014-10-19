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
#include "NodeGraphNode.h"

#include "NodeGraphEnums.h"
#include "NodeGraphNodeAttachment.h"

#include <Canvas2D.h>

namespace hs {

NodeGraphNode::Style::Style() 
    : m_TitleSize(12)
    , m_BackgroundMargin(4, 0, 4, 0)
{
    m_Border[eState_Normal] = he::Color(79, 133, 149, static_cast<he::uint8>(255));
    m_Border[eState_Hover] = he::Color(143, 185, 197, static_cast<he::uint8>(255));
    m_Border[eState_Selected] = he::Color(208, 225, 230, static_cast<he::uint8>(255));

    m_Background[eState_Normal] = he::Color(95, 100, 126, static_cast<he::uint8>(255));
    m_Background[eState_Hover] = m_Background[eState_Normal];
    m_Background[eState_Selected] = he::Color(118, 123, 152, static_cast<he::uint8>(255));

    m_Title[eState_Normal] = he::Color(210, 227, 232, static_cast<he::uint8>(255));
    m_Title[eState_Hover] =  m_Title[eState_Normal];
    m_Title[eState_Selected] =  m_Title[eState_Normal];
}

NodeGraphNode::NodeGraphNode()
    : m_State(eState_Normal)
{
}

NodeGraphNode::~NodeGraphNode()
{

}

void NodeGraphNode::create()
{
    m_Layout.initGrid(5, 3);
    m_Layout.setLayoutBound(he::RectF(0, 0, 128, 64));
}

void NodeGraphNode::destroy()
{

}

bool NodeGraphNode::isInside( const he::vec2& worldPos )
{
    return m_Layout.getLayoutBound().isInside(worldPos);
}

void NodeGraphNode::setPosition(const he::vec2& position)
{
    he::RectF bound(m_Layout.getLayoutBound());
    bound.x = position.x;
    bound.y = position.y;
    m_Layout.setLayoutBound(bound);
}

he::vec2 NodeGraphNode::getPosition() const
{
    const he::RectF& bound(m_Layout.getLayoutBound());
    return he::vec2(bound.x, bound.y);
}

void NodeGraphNode::addOutput( NodeGraphNodeAttachment* output )
{
    m_Layout.setAt(2, he::checked_numcast<he::uint8>(m_Attachments.size()), 0, output);
    addAttachment(output);
}

void NodeGraphNode::addInput( NodeGraphNodeAttachment* input )
{
    m_Layout.setAt(0, he::checked_numcast<he::uint8>(m_Attachments.size()), 0, input);
    addAttachment(input);
}

void NodeGraphNode::addDecoration(NodeGraphNodeAttachment* deco)
{
    m_Layout.setAt(1, he::checked_numcast<he::uint8>(m_Attachments.size()), 0, deco);
    addAttachment(deco);
}

void NodeGraphNode::addAttachment(NodeGraphNodeAttachment* att)
{
    m_Attachments.add(att);
}

namespace {
    bool BoundCheck(const he::RectF& bound, const he::RectF& region)
    {
        return bound.x + bound.width > region.x &&
            bound.x < region.x + region.width && 
            bound.y + bound.height > region.y &&
            bound.y < region.y + region.height;
    }
}

void NodeGraphNode::draw( const NodeGraphDrawContext& context )
{
    const he::RectF& bound(m_Layout.getLayoutBound());
    if (BoundCheck(bound, context.worldRect))
    {
        drawNodeBackground(context);

        NodeGraphDrawContext localContext(context);
        localContext.transform = context.transform * he::mat33::createTranslation2D(he::vec2(bound.x, bound.y));
        m_Attachments.forEach([this, &localContext](NodeGraphNodeAttachment* att)
        {
            drawAttachment(localContext, att);
        });
    }
}

void NodeGraphNode::drawNodeBackground( const NodeGraphDrawContext& context )
{
    he::RectF bound(m_Layout.getLayoutBound());
    const he::vec4& margin(m_Style.m_BackgroundMargin);
    bound.x += margin.x;
    bound.y += margin.y;
    bound.width -= margin.x + margin.z;
    bound.height -= margin.y + margin.w;
    const he::RectI transformedBound(bound.transform(context.transform));

    context.canvas->setColor(m_Style.m_Background[m_State]);
    context.canvas->fillRect(transformedBound);    

    context.canvas->setColor(m_Style.m_Border[m_State]);
    context.canvas->strokeRect(transformedBound);    
}

void NodeGraphNode::drawAttachment( const NodeGraphDrawContext& context, NodeGraphNodeAttachment* attachment )
{
    attachment->draw(context);
}

} //end namespace

