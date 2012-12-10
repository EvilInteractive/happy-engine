//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 14/11/2012
#include "HappyPCH.h" 

#include "MaterialGeneratorNode.h"
#include "NodeGraphError.h"
#include "Canvas2D.h"
#include "Sprite.h"
#include "Gui.h"
#include "Canvas2Dnew.h"
#include "Renderer2D.h"
#include "MathFunctions.h"

namespace he {
namespace tools {

const vec2 connectionResolution(256, 256);

//////////////////////////////////////////////////////////////////////////
// MaterialGeneratorNode::Connecter
//////////////////////////////////////////////////////////////////////////
MaterialGeneratorNode::Connecter::Connecter( const bool isInput, const uint8 index, const ConnecterDesc& desc ):
    m_IsInput(isInput), m_Index(index), m_Desc(desc), 
    m_IsSelected(false), m_IsHooverd(false), m_Size(10, 10),
    m_IsConnected(false), m_ConnectionSprite(nullptr)
{
    gui::SpriteCreator* const cr(GUI->Sprites);
    m_Sprites[0] = cr->createSprite(m_Size);
    m_Sprites[1] = cr->createSprite(m_Size);
    m_Sprites[2] = cr->createSprite(m_Size);

    if (isInput) // only inputs have connections
    {
        m_ConnectionSprite = cr->createSprite(connectionResolution, gui::Sprite::DYNAMIC_DRAW | gui::Sprite::UNIFORM_SCALE);
    }

    renderSprites();
}
MaterialGeneratorNode::Connecter::~Connecter()
{
    m_Sprites[0]->release();
    m_Sprites[1]->release();
    m_Sprites[2]->release();

    if (m_ConnectionSprite != nullptr)
        m_ConnectionSprite->release();
}
void MaterialGeneratorNode::Connecter::renderSprites()
{
    gui::SpriteCreator* const cr(GUI->Sprites);

    // Normal
    cr->setActiveSprite(m_Sprites[0]);
    cr->circle(m_Size / 2, 4.0f);
    cr->setColor(Color(1.0f,0.9f,0.2f));
    cr->fill();
    cr->setLineWidth(1.0f);
    cr->setColor(Color(0.1f,0.1f,0.1f));
    cr->stroke();
    cr->renderSpriteAsync();

    // Selected
    cr->setActiveSprite(m_Sprites[1]);
    cr->circle(m_Size / 2, 4.0f);
    cr->setColor(Color(1.0f,0.9f,0.2f));
    cr->fill();
    cr->setLineWidth(1.0f);
    cr->setColor(Color(1.0f,1.0f,1.0f));
    cr->stroke();
    cr->renderSpriteAsync();

    // Hoover
    cr->setActiveSprite(m_Sprites[2]);
    cr->circle(m_Size / 2, 4.0f);
    cr->setColor(Color(1.0f,0.9f,0.2f));
    cr->fill();
    cr->setLineWidth(1.0f);
    cr->setColor(Color(1.0f,1.0f,1.0f));
    cr->stroke();
    cr->renderSpriteAsync();
}

void MaterialGeneratorNode::Connecter::draw2D( gfx::Canvas2D* const canvas, const mat33& transform ) const
{
    gui::Canvas2Dnew* cvs(canvas->getRenderer2D()->getNewCanvas());

    const vec2 transformedPosition(transform * m_Position);
    const vec2 size((transform * vec3(m_Size.x, m_Size.y, 0)).xy());

    if (m_IsSelected)
        cvs->drawSprite(m_Sprites[1], transformedPosition - size / 2.0f, size);
    else if (m_IsHooverd)
        cvs->drawSprite(m_Sprites[2], transformedPosition - size / 2.0f, size);
    else
        cvs->drawSprite(m_Sprites[0], transformedPosition - size / 2.0f, size);
    if (m_IsConnected)
    {
        vec2 diff(m_ConnectionPos - m_Position);
        const vec2 myNormal(diff.x > 0? 1.0f : -1.0f, 0.0f);
        const vec2 myUp(0.0f, diff.y > 0? 1.0f : -1.0f);

        cvs->drawSprite(m_ConnectionSprite, 
            transformedPosition - vec2(myNormal.x > 0 ? 0 : m_ConnectionSprite->getSize().x,
            myUp.y > 0 ? 0 : m_ConnectionSprite->getSize().y));
    }
}

bool MaterialGeneratorNode::Connecter::pick( const vec2& worldPos ) const
{
    bool result(false);
    const vec2 halfSize(m_Size / 2.0f);
    if (worldPos.x > m_Position.x - halfSize.x && worldPos.y > m_Position.y - halfSize.y &&
        worldPos.x < m_Position.x + halfSize.x && worldPos.y < m_Position.y + halfSize.y)
    {
        result = true;
    }
    return result;
}

bool MaterialGeneratorNode::Connecter::doHoover( const vec2& worldPos, const bool undoHoover )
{
    m_IsHooverd = false;
    if (undoHoover == false)
    {
        m_IsHooverd = pick(worldPos);
    }
    return m_IsHooverd;
}

void MaterialGeneratorNode::Connecter::updateSprite()
{
    HE_IF_ASSERT(m_IsInput == true && m_ConnectionSprite != nullptr && m_ConnectedConnecter != nullptr, "Set connection position on an output \nor connectionSprite == nullptr!\nor m_ConnectedConnecter == nulltpr")
    {
        m_ConnectionPos = connectionPos;
        const float lineWidth(3.0f);

        vec2 diff(m_ConnectionPos - m_Position);
        const vec2 size(abs(diff.x) + lineWidth, abs(diff.y) + lineWidth);
        const vec2 myNormal(diff.x > 0? 1.0f : -1.0f, 0.0f);
        const vec2 myUp(0.0f, diff.y > 0? 1.0f : -1.0f);
        diff.x *= myNormal.x; // abs
        diff.y *= myUp.y;

        const vec2 scaledDiff((diff / std::max(diff.x, diff.y)) * connectionResolution.x);
        const vec2 beginPoint(size.x / 2.0f - myNormal.x * size.x / 2.0f, size.y / 2.0f - myUp.y * size.y / 2.0f + (lineWidth * myUp.y) / 2.0f);
        const vec2 endPoint(size.x / 2.0f + myNormal.x * size.x / 2.0f, size.y / 2.0f + myUp.y * size.y / 2.0f - (lineWidth * myUp.y) / 2.0f);

        m_ConnectionSprite->invalidate(size);

        gui::SpriteCreator* const cr(GUI->Sprites);
        cr->setActiveSprite(m_ConnectionSprite);
        cr->newPath();
        cr->moveTo(beginPoint);
        cr->curveTo(
            beginPoint + vec2(diff.x * myNormal.x * 0.5f, 0),
            endPoint - vec2(diff.x * myNormal.x * 0.5f, 0),
            endPoint);
        cr->setColor(Color(1.0f, 1.0f, 1.0f));
        cr->setLineWidth(3);
        cr->stroke();
        cr->renderSpriteAsync();
    }
}

void MaterialGeneratorNode::Connecter::setPosition( const vec2& position )
{
    m_Position = position;
    if (m_ConnectedConnecter != nullptr && m_IsInput == false)
    {
        m_ConnectedConnecter->updateSprite();
    }
    else if (m_IsConnected)
    {
        updateSprite();
    }
}

void MaterialGeneratorNode::Connecter::setConnected( const bool connected )
{
    m_ConnectedConnecter = nullptr;
    m_IsConnected = connected;
}

//////////////////////////////////////////////////////////////////////////
// MaterialGeneratorNode
//////////////////////////////////////////////////////////////////////////
MaterialGeneratorNode::MaterialGeneratorNode(const vec2& pos): 
    m_SelectedOverload(0), m_Overloads(1), m_Position(pos), m_Size(128, 96),
    m_IsSelected(false), m_Guid(Guid::generateGuid()), m_IsHoovering(false)
{
    gui::SpriteCreator* cr(GUI->Sprites);

    gui::Sprite* sp1(cr->createSprite(vec2(100,100)));
    cr->roundedRectangle(vec2(5,5), vec2(90,90), 10.0f);
    cr->setColor(Color(0.35f,0.35f,0.35f,0.7f));
    cr->fill();
    cr->newPath();
    cr->roundedRectangle(vec2(4,4), vec2(92,92), 10.0f);
    cr->setLineWidth(2.0f);
    cr->setColor(Color(0.1f,0.1f,0.1f));
    cr->stroke();
    cr->renderSpriteAsync();

    gui::Sprite* sp2(cr->createSprite(vec2(100,100)));
    cr->roundedRectangle(vec2(5,5), vec2(90,90), 10.0f);
    cr->setColor(Color(0.35f,0.35f,0.35f,0.7f));
    cr->fill();
    cr->newPath();
    cr->roundedRectangle(vec2(4,4), vec2(92,92), 10.0f);
    cr->setLineWidth(2.0f);
    cr->setColor(Color(0.78f,0.53f,0.23f));
    cr->stroke();
    cr->renderSpriteAsync();

    gui::Sprite* sp3(cr->createSprite(vec2(100,100)));
    cr->roundedRectangle(vec2(5,5), vec2(90,90), 10.0f);
    cr->setColor(Color(0.55f,0.55f,0.55f,0.7f));
    cr->fill();
    cr->newPath();
    cr->roundedRectangle(vec2(4,4), vec2(92,92), 10.0f);
    cr->setLineWidth(2.0f);
    cr->setColor(Color(0.1f,0.1f,0.1f));
    cr->stroke();
    cr->renderSpriteAsync();

    m_Sprites.add(sp1);
    m_Sprites.add(sp2);
    m_Sprites.add(sp3);

    he::eventCallback2<void, bool, uint8> onConnect([this](const bool isInput, const uint8 index)
    {
        if (isInput)
        {
            size_t connectionIndex(0);
            if (m_Connecters.find_if([isInput, index](Connecter* connecter) -> bool
            {
                return connecter->isInput() && connecter->getIndex() == index;
            }, connectionIndex))
            {
                Connecter* connecter(m_Connecters[connectionIndex]);
                const MaterialGeneratorConnection& connection(getInputConnection(index));
                MaterialGeneratorNode* other(static_cast<MaterialGeneratorNode*>(connection.node));
                HE_ASSERT(other != nullptr, "Connecting to an unknown node");

                size_t otherConnectionIndex(0);
                if (other->m_Connecters.find_if([&connection](Connecter* connecter) -> bool
                {
                    return connecter->isInput() == false && connecter->getIndex() == connection.connecter;
                }, otherConnectionIndex))
                {
                    Connecter* otherConnecter(other->m_Connecters[otherConnectionIndex]);
                    connecter->setConnected(true);
                    connecter->setConnectedConnecter(otherConnecter);
                }
            }
        }
    });
    he::eventCallback2<void, bool, uint8> onDisconnect([this](const bool isInput, const uint8 index)
    {
        if (isInput)
        {
            size_t connectionIndex(0);
            if (m_Connecters.find_if([isInput, index](Connecter* connecter) -> bool
            {
                return connecter->isInput() && connecter->getIndex() == index;
            }, connectionIndex))
            {
                Connecter* connecter(m_Connecters[connectionIndex]);
                connecter->setConnected(false);
            }
        }
    });
    NodeConnected += onConnect;
    NodeDisconnected += onDisconnect;
}

MaterialGeneratorNode::~MaterialGeneratorNode()
{
    m_Sprites.forEach([](gui::Sprite* sp)
    {
        sp->release();
    });

    m_Connecters.forEach([](Connecter* c)
    {
        delete c;
    });
}

bool MaterialGeneratorNode::canConnect( const MaterialGeneratorNodeOutput& fromOutput, 
                                        const MaterialGeneratorNodeInput& toInput, MaterialGeneratorError& error ) const
{
    bool result(true);

    // Overloads are impossible to type check, so they are checked @shader compile time
    if (fromOutput.getType() != toInput.getType() && m_Overloads.size() == 1) // types don't match and we have no overloads
    {
        error.setMessage("Type %s is not compatible with %s", 
            materialGeneratorVariableTypeToString(fromOutput.getType()), 
            materialGeneratorVariableTypeToString(toInput.getType()));
        result = false;
    }
    return result;
}

void MaterialGeneratorNode::addOverload( uint8 outputCount, uint8 inputCount, ... )
{
    HE_ASSERT(m_Overloads.size() == 0 || (m_Overloads[0].outputs.size() == outputCount && m_Overloads[0].inputs.size() == inputCount), 
        "Incompatible amount of inputs or outputs supplied with overload:\n Outputs: %d/%d\n Inputs: %d/%d", 
        outputCount, m_Overloads[0].outputs.size(), inputCount, m_Overloads[0].inputs.size());

    Overload overload;
    overload.outputs.reserve(outputCount);
    overload.inputs.reserve(inputCount);

    va_list argList;
    va_start(argList, inputCount);
    for (uint8 i(0); i < outputCount; ++i)
    {
        MaterialGeneratorVariableType type(va_arg(argList, MaterialGeneratorVariableType));
        overload.outputs.add(type);
    }
    for (uint8 i(0); i < inputCount; ++i)
    {
        MaterialGeneratorVariableType type(va_arg(argList, MaterialGeneratorVariableType));
        overload.inputs.add(type);
    }
    va_end(argList);

    m_Overloads.add(overload);

    if (m_Overloads.size() == 1)
    {
        for (uint8 i(0); i < outputCount; ++i)
        {
            addOutput(MaterialGeneratorNodeOutput());
        }
        for (uint8 i(0); i < inputCount; ++i)
        {
            addInput(MaterialGeneratorNodeInput());
        }
        activateOverload(0);
    }
}

void MaterialGeneratorNode::addConnecters( uint8 outputCount, uint8 inputCount, ... )
{
    HE_ASSERT(m_Overloads[0].outputs.size() == outputCount && m_Overloads[0].inputs.size() == inputCount, 
        "Incompatible amount of inputs or outputs supplied with connecters:\n Outputs: %d/%d\n Inputs: %d/%d", 
        outputCount, m_Overloads[0].outputs.size(), inputCount, m_Overloads[0].inputs.size());
    HE_ASSERT(m_Connecters.empty(), "Connecters already set!");

    va_list argList;
    va_start(argList, inputCount);
    for (uint8 i(0); i < outputCount; ++i)
    {
        const ConnecterDesc& desc(va_arg(argList, ConnecterDesc));
        m_Connecters.add(NEW Connecter(false, i, desc));
    }
    for (uint8 i(0); i < inputCount; ++i)
    {
        const ConnecterDesc& desc(va_arg(argList, ConnecterDesc));
        m_Connecters.add(NEW Connecter(true, i, desc));
    }
    va_end(argList);
    updateConnecterPositions();
}


bool MaterialGeneratorNode::evaluate( MaterialGeneratorError& error )
{
    bool result(false);
    uint8 overload(UINT8_MAX);
    if (findOverload(overload))
    {
        result = true;
        activateOverload(overload);
    }
    else
    {
        error.setMessage("Error! Incorrect input types, could not find compatible overload!");
        result = false;
    }
    return result;
}

void MaterialGeneratorNode::activateOverload( uint8 overloadIndex )
{
    HE_IF_ASSERT(overloadIndex < m_Overloads.size(), "Overload out of range!")
    {
        uint8 outputCount(getOutputCount());
        uint8 inputCount(getInputCount());
        Overload& overload(m_Overloads[overloadIndex]);

        for (uint8 i(0); i < outputCount; ++i)
        {
            MaterialGeneratorNodeOutput& output(getOutput(i));
            output.setType(overload.outputs[i]);
        }
        for (uint8 i(0); i < inputCount; ++i)
        {
            HE_ASSERT(getInputConnection(i).isConnected() == false || getInputConnection(i).getConnection().getType() == overload.inputs[i], "Incompatible overload assigned");
            MaterialGeneratorNodeInput& input(getInput(i));
            input.setType(overload.inputs[i]);
        }
    }
}

bool MaterialGeneratorNode::findOverload( uint8& outOverload ) const
{
    bool result(false);
    const uint8 inputCount(getInputCount());

    const size_t overloadCount(m_Overloads.size());
    for (uint8 i(0); i < overloadCount && result == false; ++i)
    {
        const Overload& overload(m_Overloads[i]);
        result = true;
        outOverload = i;
        for (uint8 inputIndex(0); inputIndex < inputCount; ++inputIndex)
        {
            const MaterialGeneratorConnection& connection(getInputConnection(inputIndex));
            if (connection.isConnected() == true && // if input is connected
                getInputConnection(inputIndex).getConnection().getType() != overload.inputs[inputIndex]) // if inputs type is not compatible with overload
            {
                result = false; // result = false -> try other overload
                break;
            }
        }
    }
    return result;
}

bool MaterialGeneratorNode::pick( const vec2& worldPos ) const
{
    bool result(false);
    const vec2 halfSize(m_Size / 2.0f);
    if (worldPos.x > m_Position.x - halfSize.x && worldPos.y > m_Position.y - halfSize.y &&
        worldPos.x < m_Position.x + halfSize.x && worldPos.y < m_Position.y + halfSize.y)
    {
        result = true;
    }
    return result;
}
bool MaterialGeneratorNode::doHoover( const vec2& worldPos, const bool undoHoover )
{
    m_IsHoovering = false;
    bool result(false);
    m_Connecters.rForEach([&worldPos, &result, undoHoover](Connecter* const connecter)
    {
        result |= connecter->doHoover(worldPos, result || undoHoover);
    });
    if (result == false && undoHoover == false)
    {
        result = pick(worldPos);
        m_IsHoovering = result;
    }
    return result;
}


void MaterialGeneratorNode::setPosition( const vec2& position )
{
    m_Position = position;
    updateConnecterPositions();
}

void MaterialGeneratorNode::updateConnecterPositions()
{
    m_Connecters.forEach([this](Connecter* const connecter)
    {
        vec2 pos(m_Position);
        uint8 count(0);
        if (connecter->isInput())
        {
            pos.x += m_Size.x / 2.0f;
            count = getInputCount();
        }
        else
        {
            pos.x -= m_Size.x / 2.0f;
            count = getOutputCount();
        }
        pos.y += (m_Size.y / (count + 1)) * (connecter->getIndex() + 1) - m_Size.y / 2.0f;
        connecter->setPosition(pos);
    });
}

void MaterialGeneratorNode::draw2D(gfx::Canvas2D* const canvas, const mat33& transform, const RectF& clipRect )
{
    gui::Canvas2Dnew* cvs(canvas->getRenderer2D()->getNewCanvas());

    const vec2 transformedPosition(transform * m_Position);
    const vec2 size((transform * vec3(m_Size.x, m_Size.y, 0)).xy());

    if (transformedPosition.x + size.x / 2.0f > clipRect.x && transformedPosition.x - size.x / 2.0f < clipRect.x + clipRect.width && 
        transformedPosition.y + size.y / 2.0f > clipRect.y && transformedPosition.y - size.y / 2.0f < clipRect.y + clipRect.height)
    {
        if (m_IsSelected)
            cvs->drawSprite(m_Sprites[1], transformedPosition - size / 2.0f, size);
        else if (m_IsHoovering)
            cvs->drawSprite(m_Sprites[2], transformedPosition - size / 2.0f, size);
        else
            cvs->drawSprite(m_Sprites[0], transformedPosition - size / 2.0f, size);
        m_Connecters.forEach([canvas, &transform](Connecter* connecter)
        {
            connecter->draw2D(canvas, transform);
        });
    }
}

} } //end namespace
