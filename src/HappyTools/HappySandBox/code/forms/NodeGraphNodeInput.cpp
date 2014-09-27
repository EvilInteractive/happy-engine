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
#include "NodeGraphNodeInput.h"

#include "NodeGraphEnums.h"

#include <Canvas2D.h>
#include <ContentManager.h>
#include <Font.h>

namespace hs {

NodeGraphNodeInput::NodeGraphNodeInput()
{
    const char* testText[7] = { "x", "y", "z", "w", "map", "xyz", "rgba" };
    const char* text(testText[rand()%7]);

    he::gui::Font* vecfont(CONTENT->loadFont("Ubuntu-Regular.ttf", 14, he::gui::Font::NO_CACHE));
    m_Text.create(vecfont, he::gui::VectorText::HAlignment_Left, he::gui::VectorText::VAlignment_Center, text);
    vecfont->release();

    m_Text.setScale(32.0f);

    setContentMargin(he::vec4(4, 0, 4, 0));
    setContentSize(m_Text.getSize());

    m_Text.setPostion(he::vec2(0, 0));

}

NodeGraphNodeInput::~NodeGraphNodeInput()
{

}

void NodeGraphNodeInput::draw(const NodeGraphDrawContext& context)
{
    he::RectF contentBound(getContentBound());
    he::RectF transformedContentBound(contentBound.transform(context.transform));
    context.canvas->setColor(he::Color(200, 50, 50, static_cast<he::uint8>(255)));
    context.canvas->fillRect(he::RectI(transformedContentBound));

    context.canvas->setColor(he::Color(255, 255, 255, static_cast<he::uint8>(255)));
    m_Text.setPostion(he::vec2(contentBound.x, contentBound.y + contentBound.height / 2.0f));
    m_Text.draw2D(context.canvas, context.transform);

    drawConnector(context);
}

} //end namespace

