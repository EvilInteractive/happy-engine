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

#ifndef _HS_NODEGRAPHNODEATTACHMENTS_H_
#define _HS_NODEGRAPHNODEATTACHMENTS_H_
#pragma once

#include "NodeGraphNodeAttachment.h"
#include "NodeGraphNodeConnector.h"

#include <HorizontalLayout.h>
#include <VectorText.h>

namespace he {
namespace gfx {
    class Texture2D;
} }

namespace hs {
    class NodeGraphNodeTextConnector : public NodeGraphNodeAttachment
    {
    public:
        NodeGraphNodeTextConnector();
        ~NodeGraphNodeTextConnector();

        void init(const ENodeGraphNodeConnectorType connectorType, const char* text);

        // Connector
        NodeGraphNodeConnector* pickNodeConnector(const he::vec2& worldPos) override;
        NodeGraphNodeConnector& getNodeConnector()  { return m_Connector; }

        // Draw
        void draw(const NodeGraphDrawContext& context) override;

        DECLARE_LAYOUT

    private:
        he::gui::HorizontalLayout m_Layout;
        he::gui::LayoutableVectorText m_Text;
        NodeGraphNodeConnector m_Connector;
    };

    class NodeGraphNodeTextureAttachment : public NodeGraphNodeAttachment
    {
    public:
        NodeGraphNodeTextureAttachment();
        ~NodeGraphNodeTextureAttachment();

        void init(const he::gfx::Texture2D* const tex, const he::vec2& size);

        // Connector
        NodeGraphNodeConnector* pickNodeConnector(const he::vec2& /*worldPos*/) override { return nullptr; }

        // Draw
        void draw(const NodeGraphDrawContext& context) override;

        DECLARE_LAYOUT

    private:
        he::gui::DefaultLayoutable m_Layout;
        const he::gfx::Texture2D* m_Texture;
    };
}

#endif
