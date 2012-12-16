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
#include "BezierShape2D.h"

namespace he {
namespace tools {

//////////////////////////////////////////////////////////////////////////
// MaterialGeneratorNode::Connecter
//////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4355) // use of this in init list
MaterialGeneratorNode::Connecter::Connecter( MaterialGeneratorNode* const parent, const bool isInput, const uint8 index, const ConnecterDesc& desc ):
    m_IsInput(isInput), m_Index(index), m_Desc(desc), 
    m_IsSelected(false), m_IsHooverd(false), m_Size(10, 10),
    m_IsConnected(false), m_Bezier(nullptr), m_ConnectedConnecter(nullptr),
    m_ConnectionMovedCallback([this](){ m_Bezier->setPositionEnd(m_ConnectedConnecter->getPosition()); }),
    m_Parent(parent)
{
    gui::SpriteCreator* const cr(GUI->Sprites);
    m_Sprites[0] = cr->createSprite(m_Size);
    m_Sprites[1] = cr->createSprite(m_Size);
    m_Sprites[2] = cr->createSprite(m_Size);

    if (isInput) // only inputs have connections
    {
        m_Bezier = NEW gui::BezierShape2D;
        m_Bezier->setBeginTangent(vec2(1, 0));
        m_Bezier->setEndTangent(vec2(-1, 0));
    }

    renderSprites();
}
#pragma warning(default:4355) // use of this in init list
MaterialGeneratorNode::Connecter::~Connecter()
{
    gui::SpriteCreator* const cr(GUI->Sprites);
    cr->removeSprite(m_Sprites[0]);
    cr->removeSprite(m_Sprites[1]);
    cr->removeSprite(m_Sprites[2]);
    delete m_Bezier;
}
void MaterialGeneratorNode::Connecter::renderSprites()
{
    gui::SpriteCreator* const cr(GUI->Sprites);

    // Normal
    cr->setActiveSprite(m_Sprites[0]);
    cr->circle(m_Size / 2, 4.0f);
    cr->setColor(m_Desc.m_Color);
    cr->fill();
    cr->setLineWidth(1.0f);
    cr->setColor(Color(0.1f,0.1f,0.1f));
    cr->stroke();
    cr->renderSpriteAsync();

    // Selected
    cr->setActiveSprite(m_Sprites[1]);
    cr->circle(m_Size / 2, 4.0f);
    cr->setColor(m_Desc.m_Color);
    cr->fill();
    cr->setLineWidth(1.0f);
    cr->setColor(Color(1.0f,1.0f,1.0f));
    cr->stroke();
    cr->renderSpriteAsync();

    // Hoover
    cr->setActiveSprite(m_Sprites[2]);
    cr->circle(m_Size / 2, 4.0f);
    cr->setColor(m_Desc.m_Color);
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
        m_Bezier->draw2D(canvas, transform);
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

void MaterialGeneratorNode::Connecter::setPosition( const vec2& position )
{
    if (position != m_Position)
    {
        m_Position = position;
        Moved();
        if (m_IsConnected)
        {
            m_Bezier->setPositionStart(m_Position);
        }
    }
}

void MaterialGeneratorNode::Connecter::setConnected( const bool connected )
{
    setConnectedConnecter(nullptr);
    m_IsConnected = connected;
    if (m_IsConnected)
    {
        m_Bezier->setPositionStart(m_Position);
    }
}

void MaterialGeneratorNode::Connecter::setConnectedConnecter( Connecter* connecter )
{
    if (m_ConnectedConnecter != nullptr)
        m_ConnectedConnecter->Moved -= m_ConnectionMovedCallback;
    m_ConnectedConnecter = connecter;
    if (m_ConnectedConnecter != nullptr)
    {
        m_ConnectedConnecter->Moved += m_ConnectionMovedCallback;
        m_Bezier->setPositionEnd(m_ConnectedConnecter->getPosition());
    }
}

//////////////////////////////////////////////////////////////////////////
// MaterialGeneratorNode
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_OBJECT(MaterialGeneratorNode)

MaterialGeneratorNode::MaterialGeneratorNode(): 
    m_SelectedOverload(0), m_Overloads(1), m_Position(0, 0), m_Size(128, 96),
    m_IsSelected(false), m_IsHoovering(false), m_Parent(nullptr)
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
    gui::SpriteCreator* const cr(GUI->Sprites);
    m_Sprites.forEach([cr](gui::Sprite* sp)
    {
        cr->removeSprite(sp);
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
        m_Connecters.add(NEW Connecter(this, false, i, desc));
    }
    for (uint8 i(0); i < inputCount; ++i)
    {
        const ConnecterDesc& desc(va_arg(argList, ConnecterDesc));
        m_Connecters.add(NEW Connecter(this, true, i, desc));
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

bool MaterialGeneratorNode::isInView( const mat33& transform, const RectF& clipRect )
{
    const vec2 transformedPosition(transform * m_Position);
    const vec2 size((transform * vec3(m_Size.x, m_Size.y, 0)).xy());

     return transformedPosition.x + size.x / 2.0f > clipRect.x && transformedPosition.x - size.x / 2.0f < clipRect.x + clipRect.width && 
            transformedPosition.y + size.y / 2.0f > clipRect.y && transformedPosition.y - size.y / 2.0f < clipRect.y + clipRect.height;
}

void MaterialGeneratorNode::draw2D(gfx::Canvas2D* const canvas, const mat33& transform, const RectF& clipRect )
{
    gui::Canvas2Dnew* cvs(canvas->getRenderer2D()->getNewCanvas());

    const vec2 transformedPosition(transform * m_Position);
    const vec2 size((transform * vec3(m_Size.x, m_Size.y, 0)).xy());

    size_t index(0);
    if (isInView(transform, clipRect) || m_Connecters.find_if([&transform, &clipRect](Connecter* connecter) -> bool
        { return connecter->isConnected() && connecter->getConnection()->getParent()->isInView(transform, clipRect); }, index))
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
