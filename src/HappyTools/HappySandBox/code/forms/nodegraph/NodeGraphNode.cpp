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
#include <ContentManager.h>
#include <Texture2D.h>
#include <Font.h>
#include <StructuredVisitor.h>

#define HEADER_HEIGHT 24.0f

namespace hs {

NodeGraphNode::Style::Style() 
    : m_TitleSize(12)
{
    m_NinePatchTextureBackground[eState_Normal] = nullptr;
    m_NinePatchTextureBackground[eState_Hover] = nullptr;
    m_NinePatchTextureBackground[eState_Selected] = nullptr;

    m_Title[eState_Normal] = he::Color(210, 227, 232, static_cast<he::uint8>(255));
    m_Title[eState_Hover] =  m_Title[eState_Normal];
    m_Title[eState_Selected] =  m_Title[eState_Normal];
}

void NodeGraphNode::Style::sdmInit()
{
    Style::s_DefaultStyle.m_NinePatchTextureBackground[eState_Normal] = CONTENT->asyncLoadTexture2D("sandbox/ui/nodeninepatch.png");
    Style::s_DefaultStyle.m_NinePatchTextureBackground[eState_Hover] = CONTENT->asyncLoadTexture2D("sandbox/ui/nodeninepatch_hover.png");
    Style::s_DefaultStyle.m_NinePatchTextureBackground[eState_Selected] = CONTENT->asyncLoadTexture2D("sandbox/ui/nodeninepatch_selected.png");

    Style::s_ErrorStyle.m_NinePatchTextureBackground[eState_Normal] = CONTENT->asyncLoadTexture2D("sandbox/ui/nodeninepatch_error.png");
    Style::s_ErrorStyle.m_NinePatchTextureBackground[eState_Hover] = CONTENT->asyncLoadTexture2D("sandbox/ui/nodeninepatch_hover_error.png");
    Style::s_ErrorStyle.m_NinePatchTextureBackground[eState_Selected] = CONTENT->asyncLoadTexture2D("sandbox/ui/nodeninepatch_selected_error.png");
}

void NodeGraphNode::Style::sdmDestroy()
{
    Style::s_DefaultStyle.m_NinePatchTextureBackground[eState_Normal]->release();
    Style::s_DefaultStyle.m_NinePatchTextureBackground[eState_Hover]->release();
    Style::s_DefaultStyle.m_NinePatchTextureBackground[eState_Selected]->release();

    Style::s_ErrorStyle.m_NinePatchTextureBackground[eState_Normal]->release();
    Style::s_ErrorStyle.m_NinePatchTextureBackground[eState_Hover]->release();
    Style::s_ErrorStyle.m_NinePatchTextureBackground[eState_Selected]->release();
}

NodeGraphNode::Style NodeGraphNode::Style::s_DefaultStyle;
NodeGraphNode::Style NodeGraphNode::Style::s_ErrorStyle;

NodeGraphNode::NodeGraphNode(NodeGraph* parent)
    : m_State(eState_Normal)
    , m_Style(&Style::s_DefaultStyle)
    , m_Parent(parent)
{
    m_Layout.setLayoutBound(he::RectF(0, 0, 160, 128));
    m_Layout.setLayoutMargin(he::vec4(4, 16, 4, 16));

    m_Title.setLayoutHAlignment(he::gui::eLayoutHAlignment_Center);
    m_Title.setLayoutVAlignment(he::gui::eLayoutVAlignment_Top);
    m_Title.setLayoutPadding(he::vec4(0.0f, 0.0f, 0.0f, 16.0f));

    m_Layout.add(&m_Title);
}

NodeGraphNode::~NodeGraphNode()
{
    m_Attachments.forEach([](NodeGraphNodeAttachment* att)
    {
        delete att;
    });
}

void NodeGraphNode::setStyle( const Style* style )
{
    m_Style = style;
}

bool NodeGraphNode::isInside( const he::vec2& worldPos )
{
    return m_Layout.getLayoutBound().isInside(worldPos);
}

NodeGraphNodeConnector* NodeGraphNode::pickConnector( const he::vec2& worldPos )
{
    NodeGraphNodeConnector* result(nullptr);
    size_t index(0);
    m_Attachments.find_if([&worldPos, &result](NodeGraphNodeAttachment* att)
    {
        result = att->pickNodeConnector(worldPos);
        return result != nullptr;
    }, index);
    return result;
}

void NodeGraphNode::move( const he::vec2& worldDelta )
{
    m_Layout.move(worldDelta);
}

void NodeGraphNode::addAttachment(NodeGraphNodeAttachment* att)
{
    m_Attachments.add(att);
    HE_ASSERT(m_Layout.isLayoutSuspended(), "Please call startEdit before adding or removing NodeGraphNodeAttachment's");
    const float height(att->getLayoutMinSize().y + 8);
    he::RectF layoutBound(m_Layout.getLayoutBound());
    layoutBound.height += height;
    m_Layout.setLayoutBound(layoutBound);
    m_Layout.add(att);
}

void NodeGraphNode::startEdit()
{
    m_Layout.suspendLayout();
}

void NodeGraphNode::endEdit()
{
    m_Layout.resumeLayout();
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

        m_Title.draw2D(context.canvas, context.transform);

        m_Attachments.forEach([this, &context](NodeGraphNodeAttachment* att)
        {
            drawAttachment(context, att);
        });
    }
}

void NodeGraphNode::drawNodeBackground( const NodeGraphDrawContext& context )
{
    he::RectF bound(m_Layout.getLayoutBound());
    const he::RectI transformedBound(bound.transform(context.transform));

    const he::gfx::Texture2D* tex(m_Style->m_NinePatchTextureBackground[m_State]);
    context.canvas->setColor(he::Color(1.0f, 1.0f, 1.0f, 0.75f));
    context.canvas->drawNinePatch(tex, he::vec2(static_cast<float>(transformedBound.x), static_cast<float>(transformedBound.y)), 
        he::RectF(24.0f, 24.0f, transformedBound.width - 48.0f, transformedBound.height - 48.0f),
        he::vec2(static_cast<float>(transformedBound.width), static_cast<float>(transformedBound.height)));
}

void NodeGraphNode::drawAttachment( const NodeGraphDrawContext& context, NodeGraphNodeAttachment* attachment )
{
    attachment->draw(context);
}

void NodeGraphNode::setTitle( const char* title )
{
    he::gui::Font* font(CONTENT->loadFont("Ubuntu-Regular.ttf", 72, he::gui::Font::NO_CACHE));
    m_Title.create(font, 16, title);
    font->release();
}

void NodeGraphNode::visit( he::io::StructuredVisitor* const visitor )
{
    visitor->visit(he::HEFS::strPosition, m_Position);
    m_Attachments.forEach([visitor](NodeGraphNodeAttachment* att)
    {
        att->visit(visitor);
    });
}

} //end namespace

