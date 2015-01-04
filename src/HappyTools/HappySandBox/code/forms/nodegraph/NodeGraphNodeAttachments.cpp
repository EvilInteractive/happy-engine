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
#include "NodeGraphNodeAttachments.h"

#include "NodeGraphEnums.h"

#include <Canvas2D.h>
#include <ContentManager.h>
#include <Font.h>
#include <Texture2D.h>
#include <StructuredVisitor.h>

namespace hs {

IMPLEMENT_LAYOUT_FROM(hs::NodeGraphNodeTextConnector, m_Layout)

NodeGraphNodeTextConnector::NodeGraphNodeTextConnector(NodeGraphNode* parent)
: NodeGraphNodeAttachment(parent)
{

}

NodeGraphNodeTextConnector::~NodeGraphNodeTextConnector()
{

}

void NodeGraphNodeTextConnector::init( const ENodeGraphNodeConnectorType connectorType, const he::FixedString& id, const char* text )
{
    m_Layout.suspendLayout();
    he::gui::Font* font(CONTENT->loadFont("Ubuntu-Regular.ttf", 72, he::gui::Font::NO_CACHE));
    m_Text.create(font, 16, text);
    font->release();

    m_Text.setLayoutVAlignment(he::gui::eLayoutVAlignment_Center);
    NodeGraphNodeConnector& connector(*getNodeConnector());
    connector.setId(id);
    connector.setLayoutVAlignment(he::gui::eLayoutVAlignment_Center);
    connector.setType(connectorType);
    switch (connectorType)
    {
    case eNodeGraphNodeConnectorType_Input:
        {
            m_Text.setLayoutPadding(he::vec4(4, 0, 0, 0));
            m_Layout.add(&connector);
            m_Layout.add(&m_Text);
            m_Layout.setLayoutHAlignment(he::gui::eLayoutHAlignment_Left);
        } break;
    case eNodeGraphNodeConnectorType_Output:
        {
            m_Text.setLayoutPadding(he::vec4(0, 0, 4, 0));
            m_Layout.add(&m_Text);
            m_Layout.add(&connector);
            m_Layout.setLayoutHAlignment(he::gui::eLayoutHAlignment_Right);
        } break;
    }
    he::vec2 minSize(m_Text.getLayoutMinSize());
    minSize.x += connector.getLayoutMinSize().x + 8; // +8 padding
    m_Layout.setLayoutMinSize(minSize);
    m_Layout.resumeLayout();
}

NodeGraphNodeConnector* NodeGraphNodeTextConnector::pickNodeConnector( const he::vec2& worldPos )
{
    NodeGraphNodeConnector* connector(getNodeConnector());
    if (connector->pick(worldPos))
        return connector;
    else
        return nullptr;
}

void NodeGraphNodeTextConnector::draw( const NodeGraphDrawContext& context )
{
    getNodeConnector()->draw(context);
    m_Text.draw2D(context.canvas, context.transform);
}

void NodeGraphNodeTextConnector::visit( he::io::StructuredVisitor* const visitor )
{
    getNodeConnector()->visit(visitor);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_LAYOUT_FROM(hs::NodeGraphNodeTextureAttachment, m_Layout)

NodeGraphNodeTextureAttachment::NodeGraphNodeTextureAttachment(NodeGraphNode* parent)
    : NodeGraphNodeAttachment(parent)
    , m_Texture(nullptr)
{
    m_Layout.setLayoutHAlignment(he::gui::eLayoutHAlignment_Center);
}

NodeGraphNodeTextureAttachment::~NodeGraphNodeTextureAttachment()
{
    if (m_Texture)
        m_Texture->release();
}

void NodeGraphNodeTextureAttachment::init( const he::gfx::Texture2D* const tex, const he::vec2& size )
{
    m_Layout.setLayoutMaxSize(size);
    m_Layout.setLayoutMinSize(size);
    tex->instantiate();
    m_Texture = tex;
}

void NodeGraphNodeTextureAttachment::draw( const NodeGraphDrawContext& context )
{
    const he::RectF bound(m_Layout.getLayoutBound().transform(context.transform));
    context.canvas->drawImage(m_Texture, he::vec2(bound.x, bound.y), he::vec2(bound.width, bound.height));
}

} //end namespace

