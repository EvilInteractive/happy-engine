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
//Author:  Bastian Damman
//Created: 2014/10/19
#include "HappyPlugin2DTestPCH.h" 
#include "ColouredRect.h"

#include <ContentManager.h>
#include <Font.h>
#include <Canvas2D.h>

namespace ht {

ColouredRect::ColouredRect()
{
}

ColouredRect::~ColouredRect()
{
}

void ColouredRect::draw2D( he::gui::Canvas2D* canvas )
{
    he::RectI clipRect(m_LayoutClipBound);
    canvas->setClipRect(clipRect);
    canvas->setColor(he::Color(0.4f, 0.0f, 0.0f));
    canvas->drawLine(
        he::vec2(m_LayoutClipBound.x, m_LayoutClipBound.y + m_LayoutClipBound.height - 1), 
        he::vec2(m_LayoutClipBound.x + m_LayoutClipBound.width, m_LayoutClipBound.y + m_LayoutClipBound.height - 1));
    canvas->drawLine(
        he::vec2(m_LayoutClipBound.x + m_LayoutClipBound.width - 1, m_LayoutClipBound.y), 
        he::vec2(m_LayoutClipBound.x + m_LayoutClipBound.width - 1, m_LayoutClipBound.y + m_LayoutClipBound.height));
    canvas->setColor(m_Color);
    canvas->fillRect(he::RectI(m_LayoutBound));
    canvas->setColor(he::Color(1.0f-m_Color.r(), 1.0f-m_Color.g(), 1.0f-m_Color.b(), 1.0f));
    canvas->fillText(m_DebugText, he::vec2(m_LayoutBound.x + 4, m_LayoutBound.y + 4));
    canvas->resetClipRect();
}

void ColouredRect::createDebugText()
{
    if (m_DebugText.getFont() == nullptr)
    {
        he::gui::Font* const debugFont(CONTENT->getDefaultFont(12));
        m_DebugText.setFont(debugFont);
        debugFont->release();
    }
    m_DebugText.clear();
    m_DebugText.addTextExt("Padding: %.2f, %.2f, %.2f, %.2f", m_Padding.x, m_Padding.y, m_Padding.z, m_Padding.z);
    m_DebugText.addTextExt("\nMinSize: %.2f, %.2f", m_MinSize.x, m_MinSize.y);
    m_DebugText.addTextExt("\nMaxSize: %.2f, %.2f", m_MaxSize.x, m_MaxSize.y);
    m_DebugText.addTextExt("\nHAlign: %d", m_HAlignment);
    m_DebugText.addTextExt("\nVAlign: %d", m_VAlignment);
    m_DebugText.addTextExt("\nSize: %.2f %.2f", m_LayoutBound.width, m_LayoutBound.height);
}

} //end namespace