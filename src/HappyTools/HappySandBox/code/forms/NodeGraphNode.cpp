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
#include "INodeGraphNodeAttachment.h"

#include <Canvas2D.h>

namespace hs {

NodeGraphNode::Style::Style() 
    : m_TitleSize(12)
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
{

}

NodeGraphNode::~NodeGraphNode()
{

}

bool NodeGraphNode::isInside( const he::vec2& worldPos )
{
    return m_Bound.isInside(worldPos);
}

void NodeGraphNode::addOutput( INodeGraphOutput* output )
{
    m_Outputs.add(output);
}

void NodeGraphNode::addInput( INodeGraphInput* input )
{
    m_Inputs.add(input);
}

void NodeGraphNode::updateLayout()
{
    float leftY(0.0f), centerY(0.0f), rightY(0.0f);
    m_Attachments.forEach([&](INodeGraphNodeAttachment* att)
    {
        switch (att->getLayoutAlignment())
        {
        case INodeGraphNodeAttachment::eLayoutAlignment_Left:
            {
                leftY += att->getLayoutMargin().y;
                he::RectF bound(att->getBound());
                bound.y = leftY;
                leftY += att->getLayoutMargin().w;
            } break;
        case INodeGraphNodeAttachment::eLayoutAlignment_Center:
            {
                centerY = std::max(leftY, rightY);
                centerY += att->getLayoutMargin().y;
                he::RectF bound(att->getBound());
                bound.y = centerY;
                centerY += att->getLayoutMargin().w;
                leftY = centerY;
                rightY = centerY;
            } break;
        case INodeGraphNodeAttachment::eLayoutAlignment_Right:
            {
                rightY += att->getLayoutMargin().y;
                he::RectF bound(att->getBound());
                bound.y = rightY;
                rightY += att->getLayoutMargin().w;
            } break;
        }
    });
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
    size_t notUsed(0);
    m_Attachments.find_if([this](INodeGraphNodeAttachment* att)
    {
        if (att->needsLayoutUpdate())
        {
            updateLayout();
            return true;
        }
        return false;
    }, notUsed);

    if (BoundCheck(m_Bound, context.worldRect))
    {
        drawNodeBackground(context);

        m_Attachments.forEach([this, &context](INodeGraphNodeAttachment* att)
        {
            drawAttachment(context, att);
        });
    }
}

void NodeGraphNode::drawNodeBackground( const NodeGraphDrawContext& context )
{
    const he::RectI transformedBound(m_Bound.transform(context.transform));

    context.canvas->setColor(m_Style.m_Background[m_State]);
    context.canvas->fillRect(transformedBound);    

    context.canvas->setColor(m_Style.m_Border[m_State]);
    context.canvas->strokeRect(transformedBound);    
}

void NodeGraphNode::drawAttachment( const NodeGraphDrawContext& context, INodeGraphNodeAttachment* attachment )
{
    attachment->draw(context);
}

} //end namespace

