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
#include "NodeGraphNodeOutput.h"

#include "NodeGraphEnums.h"

#include <Canvas2D.h>

namespace hs {

NodeGraphNodeOutput::NodeGraphNodeOutput()
{
    setContentMargin(he::vec4(4, 0, 4, 0));
    setContentSize(he::vec2(rand() % 64 + 64.0f, rand() % 64 + 16.0f));
}

NodeGraphNodeOutput::~NodeGraphNodeOutput()
{

}

void NodeGraphNodeOutput::draw(const NodeGraphDrawContext& context)
{
    he::RectF contentBound(getContentBound().transform(context.transform));
    context.canvas->setColor(he::Color(50, 200, 50, static_cast<he::uint8>(255)));
    context.canvas->fillRect(he::RectI(contentBound));

    drawConnector(context);
}

} //end namespace

