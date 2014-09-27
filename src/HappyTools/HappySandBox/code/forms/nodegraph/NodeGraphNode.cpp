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
#include "NodeGraphNodeInput.h"
#include "NodeGraphNodeOutput.h"
#include "NodeGraphNodeDecoration.h"

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
    , m_LayoutDirty(false)
    , m_Bound(0, 0, 0, 0)
{

}

NodeGraphNode::~NodeGraphNode()
{

}

bool NodeGraphNode::isInside( const he::vec2& worldPos )
{
    return m_Bound.isInside(worldPos);
}

void NodeGraphNode::setPosition(const he::vec2& position)
{
    m_Bound.x = position.x;
    m_Bound.y = position.y;
}

he::vec2 NodeGraphNode::getPosition() const
{
    return he::vec2(m_Bound.x, m_Bound.y);
}

void NodeGraphNode::addOutput( NodeGraphNodeOutput* output )
{
    m_Outputs.add(output);
    addAttachment(output);
}

void NodeGraphNode::addInput( NodeGraphNodeInput* input )
{
    m_Inputs.add(input);
    addAttachment(input);
}

void NodeGraphNode::addDecoration(NodeGraphNodeDecoration* deco)
{
    addAttachment(deco);
}

void NodeGraphNode::addAttachment(INodeGraphNodeAttachment* att)
{
    m_Attachments.add(att);
    m_LayoutDirty = true;
}

void NodeGraphNode::updateLayout()
{
    float sizeX(0.0f), sizeLeftX(0.0f), sizeRightX(0.0f);
    float sizeY(0.0f);
    // Vertical layout
    {
        float leftY(0.0f), centerY(0.0f), rightY(0.0f);
        m_Attachments.forEach([&](INodeGraphNodeAttachment* att)
        {
            const he::vec4& margin(att->getLayoutMargin());
            he::RectF bound(att->getBound());
            switch (att->getLayoutAlignment())
            {
            case INodeGraphNodeAttachment::eLayoutAlignment_Left:
                {
                    leftY += margin.y;
                    bound.y = leftY;
                    leftY += margin.w + bound.height;
                    sizeLeftX = std::max(sizeLeftX, bound.width + margin.x + margin.z);
                } break;
            case INodeGraphNodeAttachment::eLayoutAlignment_Center:
                {
                    centerY = std::max(leftY, rightY);
                    centerY += margin.y;
                    bound.y = centerY;
                    centerY += margin.w + bound.height;
                    leftY = centerY;
                    rightY = centerY;

                    sizeX = std::max(sizeX, sizeLeftX + sizeRightX);
                    sizeX = std::max(sizeX, bound.width + margin.x + margin.z);
                    sizeLeftX = 0.0f;
                    sizeRightX = 0.0f;
                } break;
            case INodeGraphNodeAttachment::eLayoutAlignment_Right:
                {
                    rightY += margin.y;
                    bound.y = rightY;
                    rightY += margin.w + bound.height;
                    sizeRightX = std::max(sizeRightX, bound.width + margin.x + margin.z);
                } break;
            }
            att->setBound(bound);
        });
        sizeY = std::max(leftY, rightY);
    }
    sizeX = std::max(sizeX, sizeLeftX + sizeRightX);

    // Horizontal layout
    {
        const float center(sizeX / 2.0f);
        const float left(0.0f);
        const float right(sizeX);
        m_Attachments.forEach([&](INodeGraphNodeAttachment* att)
        {
            const he::vec4& margin(att->getLayoutMargin());
            he::RectF bound(att->getBound());

            switch (att->getLayoutAlignment())
            {
            case INodeGraphNodeAttachment::eLayoutAlignment_Left:
                {
                    bound.x = left + margin.x;
                } break;
            case INodeGraphNodeAttachment::eLayoutAlignment_Center:
                {
                    bound.x = center - (bound.width + margin.x + margin.z) / 2.0f;
                } break;
            case INodeGraphNodeAttachment::eLayoutAlignment_Right:
                {
                    bound.x = right - margin.z - bound.width;
                } break;
            }

            att->setBound(bound);
        });
    }

    // Bound
    m_Bound.width = sizeX;
    m_Bound.height = sizeY;
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
    if (m_LayoutDirty)
    {
        updateLayout();
        m_LayoutDirty = false;
    }

    if (BoundCheck(m_Bound, context.worldRect))
    {
        drawNodeBackground(context);

        NodeGraphDrawContext localContext(context);
        localContext.transform = context.transform * he::mat33::createTranslation2D(he::vec2(m_Bound.x, m_Bound.y));
        m_Attachments.forEach([this, &localContext](INodeGraphNodeAttachment* att)
        {
            drawAttachment(localContext, att);
        });
    }
}

void NodeGraphNode::drawNodeBackground( const NodeGraphDrawContext& context )
{
    he::RectF bound(m_Bound);
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

void NodeGraphNode::drawAttachment( const NodeGraphDrawContext& context, INodeGraphNodeAttachment* attachment )
{
    attachment->draw(context);
}

} //end namespace

