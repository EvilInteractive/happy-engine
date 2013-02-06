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
//Created: 23/11/2012
#include "HappyPCH.h" 

#include "MaterialGeneratorGraph.h"
#include "ShaderGenerator.h"

#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "Game.h"
#include "View.h"
#include "Window.h"

#include "Renderer2D.h"
#include "Canvas2D.h"

#include "NodeGraph.h"
#include "MaterialGeneratorNode.h"

#include "ControlsManager.h"
#include "IKeyboard.h"
#include "IMouse.h"

#include "MaterialGeneratorRootNodes.h"
#include "Canvas2Dnew.h"
#include "Font.h"
#include "Sprite.h"
#include "Gui.h"
#include "BezierShape2D.h"

#include "Command.h"
#include "BinaryFileVisitor.h"
#include "MaterialGeneratorNodeFactory.h"
#include "WebView.h"
#include "WebListener.h"


#define ZOOM_STEP 0.1f
#define ZOOM_MIN 0.1f
#define ZOOM_MAX 3.0f

#define ERROR_TIME 2.0f
#define ERROR_FADE_TIME 0.5f
const he::vec2 errorTextMarge(4.0f, 4.0f);

namespace he {
namespace tools {

#pragma warning(disable:4355) // use of this in init list
MaterialGeneratorGraph::MaterialGeneratorGraph()
    : m_Generator(NEW ct::ShaderGenerator)
    , m_Renderer(nullptr)
    , m_Window(nullptr)
    , m_View(nullptr)
    , m_IsActive(false)
    , m_Offset(0, 0)
    , m_Scale(1.0f)
    , m_GrabWorldPos(0.0f, 0.0f)
    , m_State(State_Idle)
    , m_MoveCommand(this)
    , m_EditSelectionCommand(this)
    , m_ConnectNodeCommand(this)
    , m_CreateCommand(this)
    , m_DeleteCommand(this)
    , m_GhostConnection(NEW gui::BezierShape2D)
    , m_WebViewGui(nullptr)
    , m_WebListener(nullptr)
{
    he::gui::Font* font(CONTENT->loadFont("Ubuntu-Medium.ttf", 14));
    m_DebugText.setFont(font);
    m_DebugText.setHorizontalAlignment(gui::Text::HAlignment_Left);
    font->release();

    m_ErrorFont = CONTENT->loadFont("DejaVuSansMono.ttf", 12);

    increaseErrorPool(5);

    m_ShortcutList.add(Shortcut(io::Key_1, MaterialGeneratorNodeType_Float1));
    m_ShortcutList.add(Shortcut(io::Key_2, MaterialGeneratorNodeType_Float2));
    m_ShortcutList.add(Shortcut(io::Key_3, MaterialGeneratorNodeType_Float3));
    m_ShortcutList.add(Shortcut(io::Key_4, MaterialGeneratorNodeType_Float4));
    m_ShortcutList.add(Shortcut(io::Key_A, MaterialGeneratorNodeType_Add));
    m_ShortcutList.add(Shortcut(io::Key_D, MaterialGeneratorNodeType_Divide));
    m_ShortcutList.add(Shortcut(io::Key_E, MaterialGeneratorNodeType_Power));
    m_ShortcutList.add(Shortcut(io::Key_L, MaterialGeneratorNodeType_Lerp));
    m_ShortcutList.add(Shortcut(io::Key_M, MaterialGeneratorNodeType_Multiply));
    m_ShortcutList.add(Shortcut(io::Key_N, MaterialGeneratorNodeType_Normalize));
    m_ShortcutList.add(Shortcut(io::Key_O, MaterialGeneratorNodeType_OneMin));
    //m_ShortcutList.add(Shortcut(io::Key_P, MaterialGeneratorNodeType_Panner));
    //m_ShortcutList.add(Shortcut(io::Key_R, MaterialGeneratorNodeType_Reflect));
    //m_ShortcutList.add(Shortcut(io::Key_T, MaterialGeneratorNodeType_Texture2D));
    //m_ShortcutList.add(Shortcut(io::Key_U, MaterialGeneratorNodeType_Texcoord));

    MaterialGeneratorNode* const root(MaterialGeneratorNodeFactory::getInstance()->create(MaterialGeneratorNodeType_RootNormalDraw));
    addNode(root);
    m_NodeGraph.addRootNode(root);
}
#pragma warning(default:4355) // use of this in init list


MaterialGeneratorGraph::~MaterialGeneratorGraph()
{
    MaterialGeneratorNodeFactory* const factory(MaterialGeneratorNodeFactory::getInstance());
    m_NodeList.forEach([factory](MaterialGeneratorNode* const node)
    {
        factory->destroy(node);
    });
    m_NodeList.clear();
    m_Renderer->detachFromRender(this);
    delete m_Renderer;
    delete m_Generator;
    if (m_View != nullptr)
    {
        GRAPHICS->removeView(m_View);
    }
    if (m_Window != nullptr)
    {
        GRAPHICS->removeWindow(m_Window);
    }

    m_ErrorFont->release();

    m_ErrorPool.forEach([](gui::Text* const text)
    {
        delete text;
    });
    m_ErrorPool.clear();
    m_VisibleErrors.forEach([](const ErrorMessage& msg)
    {
        delete msg.m_Text;
    });
    m_VisibleErrors.clear();

    delete m_GhostConnection;
    gui::SpriteCreator* const cr(GUI->Sprites);
    cr->removeSprite(m_Background);
    cr->removeSprite(m_ErrorBackgroundSprite);
}

void MaterialGeneratorGraph::init()
{
    m_Window = GRAPHICS->createWindow();
    m_View = GRAPHICS->createView();

    m_Window->setResizable(true);
    m_Window->create();
    m_Window->setCursorVisible(true);
    m_Window->setFullscreen(false);
    m_Window->setVSync(false);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("Happy Material Editor");
    m_Window->close();

    m_View->setWindow(m_Window);
    m_View->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_Renderer = NEW gfx::Renderer2D();
    m_View->addRenderPlugin(m_Renderer);
    gfx::RenderSettings settings;
    settings.enablePost = false;
    m_View->init(settings);

    he::eventCallback0<void> viewResizedHandler(boost::bind(&MaterialGeneratorGraph::onViewResized, this));
    m_View->ViewportSizeChanged += viewResizedHandler;

    m_Renderer->attachToRender(this);

    m_WebViewGui = m_Renderer->createWebViewRelative(RectF(0, 0, 1, 1), true);
    loadGui();
    m_WebViewGui->setTransparent(true);
    m_WebListener = m_WebViewGui->getWebListener();

    he::eventCallback0<void> loadedCallback([this]()
    {
        Awesomium::JSArray args;
        for (size_t i(0); i < MaterialGeneratorNodeTypeSubdivion_MAX; ++i)
        {
            const MaterialGeneratorNodeTypeSubdivion subD(static_cast<MaterialGeneratorNodeTypeSubdivion>(i));
            const char* subDString(materialGeneratorNodeTypeSubdivionToString(subD));
            args.Clear();
            args.Push(Awesomium::JSValue(Awesomium::WebString::CreateFromUTF8(subDString, strlen(subDString))));
            m_WebListener->executeFunction("", "addNodeSubdivision", args);
        }
        for (size_t i(0); i < MaterialGeneratorNodeType_MAX; ++i)
        {
            const MaterialGeneratorNodeType type(static_cast<MaterialGeneratorNodeType>(i));
            const MaterialGeneratorNodeTypeSubdivion subD(getMaterialGeneratorNodeTypeSubdivision(type));
            if (subD != MaterialGeneratorNodeTypeSubdivion_None)
            {
                const char* subDString(materialGeneratorNodeTypeSubdivionToString(subD));
                const char* typeString(materialGeneratorNodeTypeToString(type));
                args.Clear();
                args.Push(Awesomium::JSValue(Awesomium::WebString::CreateFromUTF8(subDString, strlen(subDString))));
                args.Push(Awesomium::JSValue(Awesomium::WebString::CreateFromUTF8(typeString, strlen(typeString))));
                m_WebListener->executeFunction("", "addNode", args);
            }
        }
        args.Clear();
        m_WebListener->executeFunction("", "init", args);

        MaterialGeneratorGraph* _this(this);
        he::eventCallback1<void, const Awesomium::JSArray&> nodeDroppedCallback([_this](const Awesomium::JSArray& args)
        {
            char buff[100];
            he_memset(buff, 0, 100);
            HE_ASSERT(args.size() > 0, "Nor arguments supply with nodeDroppedCallback callback!");
            const Awesomium::JSValue& value(args[0]);
            HE_ASSERT(value.IsString(), "nodeDroppedCallback did not return a string!");
            HE_ASSERT(value.IsUndefined() == false, "nodeDroppedCallback returned am undefined value!");
            const Awesomium::WebString str(value.ToString());
            value.ToString().ToUTF8(buff, str.length());
            _this->m_CommandStack.beginTransaction("Create node");
            _this->m_CreateCommand.create(materialGeneratorNodeTypeFromString(buff), 
                _this->screenToWorldPos(CONTROLS->getMouse()->getPosition()));
            _this->m_CommandStack.endTransaction();
        });
        m_WebListener->addObjectCallback("HME", "nodeDropped", nodeDroppedCallback);
    });

    m_WebViewGui->OnUrlLoaded += loadedCallback;

    CONSOLE->registerCmd(boost::bind(&MaterialGeneratorGraph::loadGui, this), "reloadMaterialGeneratorGui");

    eventCallback0<void> lostfocusCallback([this]()
    {
        if (m_IsActive == true)
        {
            m_IsActive = false;
            GAME->removeFromTickList(this);
            CONTROLS->returnFocus(this);
            m_WebViewGui->unfocus();
        }
    });
    eventCallback0<void> closeCallback([this]()
    {
        if (m_IsActive == true)
        {
            m_IsActive = false;
            GAME->removeFromTickList(this);
            CONTROLS->returnFocus(this);
            m_WebViewGui->unfocus();
        }
    });
    eventCallback0<void> gainfocusCallback([this]()
    {
        if (m_IsActive == false)
        {
            m_IsActive = true;
            GAME->addToTickList(this);
            CONTROLS->getFocus(this);
            m_WebViewGui->focus();
        }
    });

    m_Window->GainedFocus += gainfocusCallback;
    m_Window->LostFocus += lostfocusCallback;
    m_Window->Closed += closeCallback;

    gui::SpriteCreator* const cr(GUI->Sprites);
    m_Background = cr->createSprite(vec2(1280, 720));
    renderBackground();

    m_ErrorBackgroundSprite = cr->createSprite(vec2(16, 16));
    cr->rectangle(vec2(0, 0), vec2(16, 16));
    cr->setColor(Color(0.35f, 0.35f, 0.35f, 0.7f));
    cr->fill();
    cr->setColor(Color(0.7f, 0.2f, 0.2f, 1));
    cr->setLineWidth(2.0f);
    cr->setLineJoin(gui::LINE_JOIN_BEVEL);
    cr->stroke();
    cr->renderSpriteAsync();
}

void MaterialGeneratorGraph::loadGui()
{
    m_WebViewGui->loadUrl((Path::getWorkingDir().append(CONTENT->getContentDir().str()).append("gui/materialEditor.html")).str());
}


void MaterialGeneratorGraph::open()
{
    m_Window->open();
}

void MaterialGeneratorGraph::close()
{
    m_Window->close();
}

void MaterialGeneratorGraph::updateStates( const float /*dTime*/ )
{
    const io::ControlsManager* const controls(CONTROLS);
    const io::IMouse* const mouse(controls->getMouse());
    const io::IKeyboard* const keyboard(controls->getKeyboard());
    const bool keepSelection(keyboard->isKeyDown(io::Key_Lctrl) || keyboard->isKeyDown(io::Key_Rctrl));
    const bool removeSelection(keyboard->isKeyDown(io::Key_Lalt) || keyboard->isKeyDown(io::Key_Ralt));

    const vec2 mouseWorld(screenToWorldPos(mouse->getPosition()));

    switch (m_State)
    {
    case State_Idle:
        {
            m_GrabWorldPos = mouseWorld;
            const bool leftDown(mouse->isButtonPressed(io::MouseButton_Left));
            if (keyboard->isKeyPressed(io::Key_F7))
            {
                he::ObjectList<MaterialGeneratorError> errors;
                m_NodeGraph.evalute(errors);
            }
            else if (keyboard->isShortcutPressed(io::Key_Ctrl, io::Key_Shift, io::Key_Z) || 
                keyboard->isShortcutPressed(io::Key_Ctrl, io::Key_Y))
            {
                m_CommandStack.redo();
            }
            else if (keyboard->isShortcutPressed(io::Key_Ctrl, io::Key_S))
            {
                save();
            }
            else if (keyboard->isShortcutPressed(io::Key_Ctrl, io::Key_O))
            {
                load();
            }
            else if (keyboard->isShortcutPressed(io::Key_Ctrl, io::Key_Z))
            {
                m_CommandStack.undo();
            }
            else if (leftDown)
            {
                size_t keyIndex(0);
                if (m_ShortcutList.find_if(
                    [keyboard](const Shortcut& shortcut) { return keyboard->isKeyDown(shortcut.m_Key); }, keyIndex))
                {
                    m_CommandStack.beginTransaction("Create node");
                    m_CreateCommand.create(m_ShortcutList[keyIndex].m_Type, mouseWorld);
                    m_CommandStack.endTransaction();
                }
                else
                {
                    if (doConnectStart(mouse->getPosition()))
                    {
                        m_CommandStack.beginTransaction("Connect node");
                        m_State = State_ConnectNode;
                    }
                    else
                    {
                        m_CommandStack.beginTransaction("Select Node");
                        if (doNodeSelect(mouse->getPosition(), keepSelection, removeSelection))        
                        {
                            if (keepSelection == false && removeSelection == false)
                                m_State = State_StartMoveNode;
                            else
                                m_CommandStack.endTransaction();
                        }
                        else
                        {
                            m_State = State_StartPan;
                        }
                    }
                }
            }
            else if (keyboard->isKeyPressed(io::Key_Delete))
            {
                m_CommandStack.beginTransaction("Delete node(s)");
                m_SelectedNodeList.forEach([this](MaterialGeneratorNode* const node)
                {
                    m_DeleteCommand.doDelete(node);
                });
                m_SelectedNodeList.clear();
                m_CommandStack.endTransaction();
            }
        } break;
    case State_StartPan:
        {
            const vec2 mousePos(mouse->getPosition());
            const vec2 grabScreenPos(worldToScreenPos(m_GrabWorldPos));
            const vec2 diff(mousePos - grabScreenPos);
            if (mouse->isButtonReleased(io::MouseButton_Left))
            {
                if (m_SelectedNodeList.empty() == false)
                {
                    m_EditSelectionCommand.beginCommand();
                    m_EditSelectionCommand.deselectAll();
                    m_EditSelectionCommand.endCommand();
                }
                m_State = State_Idle;
                m_CommandStack.endTransaction("Deselect");
            }
            else if (fabs(diff.x) > 4 || fabs(diff.y) > 4)
            {
                m_State = State_Pan;
                m_CommandStack.endTransaction();
            }
        } break;
    case State_Pan:
        {
            const vec2 mousePos(mouse->getPosition());
            const vec2 worldMouse(screenToWorldPos(mousePos));
            const vec2 diff(worldMouse - m_GrabWorldPos);
            m_Offset -= diff; 
            if (mouse->isButtonReleased(io::MouseButton_Left))
            {
                m_State = State_Idle;
            }
        } break;
    case State_StartMoveNode:
        {
            const vec2 mousePos(mouse->getPosition());
            const vec2 grabScreenPos(worldToScreenPos(m_GrabWorldPos));
            const vec2 diff(mousePos - grabScreenPos);
            if (mouse->isButtonReleased(io::MouseButton_Left))
            {
                if (keepSelection == false && removeSelection == false)
                {
                    if (m_SelectedNodeList.empty() == false)
                    {
                        m_EditSelectionCommand.beginCommand();
                        m_EditSelectionCommand.deselectAll();
                        m_EditSelectionCommand.endCommand();
                    }
                }
                doNodeSelect(mouse->getPosition(), keepSelection, removeSelection);
                m_CommandStack.endTransaction();
                m_State = State_Idle;
            }
            else if (fabs(diff.x) > 4 || fabs(diff.y) > 4)
            {
                m_MoveCommand.beginCommand();
                m_State = State_MoveNode;
            }
        } break;
    case State_MoveNode:
        {
            const vec2 mouseMove(mouse->getMove());
            const vec2 worldMove(mouseMove / m_Scale);
            m_MoveCommand.doMove(worldMove);
            if (mouse->isButtonReleased(io::MouseButton_Left))
            {
                m_MoveCommand.endCommand();
                m_CommandStack.endTransaction("Move Node(s)");
                m_State = State_Idle;
            }
        } break;
    case State_ConnectNode:
        {
            m_GhostConnection->setPositionEnd(screenToWorldPos(mouse->getPosition()));
            if (mouse->isButtonReleased(io::MouseButton_Left))
            {
                doConnectEnd(mouse->getPosition());
                m_CommandStack.endTransaction();
                m_State = State_Idle;
            }

        } break;
    }
}

void MaterialGeneratorGraph::updateErrors( const float dTime )
{
    const io::ControlsManager* const controls(CONTROLS);
    const io::IMouse* const mouse(controls->getMouse());
    const vec2 mouseWorld(screenToWorldPos(mouse->getPosition()));
    for (size_t i(0); i < m_VisibleErrors.size();)
    {
        ErrorMessage& msg(m_VisibleErrors[i]);
        msg.m_TimeLeft -= dTime;
        if (mouseWorld.x > msg.m_Position.x - msg.m_TextSize.x / 2.0f - errorTextMarge.x && 
            mouseWorld.y > msg.m_Position.y - msg.m_TextSize.y / 2.0f - errorTextMarge.y &&
            mouseWorld.x < msg.m_Position.x + msg.m_TextSize.x / 2.0f + errorTextMarge.x && 
            mouseWorld.y < msg.m_Position.y + msg.m_TextSize.y / 2.0f + errorTextMarge.y)
        {
            if (mouse->isButtonPressed(io::MouseButton_Left))
                msg.m_TimeLeft = 0.0f;
            else if (msg.m_TimeLeft < ERROR_FADE_TIME)
                msg.m_TimeLeft = ERROR_FADE_TIME;
        }
        if (msg.m_TimeLeft <= 0.0f)
        {
            m_ErrorPool.add(m_VisibleErrors[i].m_Text);
            m_VisibleErrors.removeAt(i);
        }
        else
        {
            ++i;
        }
    }
}
void MaterialGeneratorGraph::tick( float dTime )
{
    const io::ControlsManager* const controls(CONTROLS);
    const io::IMouse* const mouse(controls->getMouse());

    const vec2 mouseWorld(screenToWorldPos(mouse->getPosition()));
    bool foundHoover(false);
    m_NodeList.rForEach([&mouseWorld, &foundHoover](MaterialGeneratorNode* const node)
    {
        foundHoover |= node->doHoover(mouseWorld, foundHoover);
    });

    updateErrors(dTime);
        
    updateStates(dTime);

    const int scroll(mouse->getScroll());
    if (scroll != 0)
    {
        const vec2 mousePos(mouse->getPosition());
        const vec2 mouseWorldPos(screenToWorldPos(mouse->getPosition()));
        m_Scale += mouse->getScroll() * ZOOM_STEP;
        m_Scale = he::clamp(m_Scale, ZOOM_MIN, ZOOM_MAX);
        const vec2 wrongMouseWorldPos(screenToWorldPos(mousePos));
        const vec2 offset(wrongMouseWorldPos - mouseWorldPos);
        m_Offset -= offset;
    }
}
bool MaterialGeneratorGraph::doNodeSelect(const vec2& mousePos, const bool keepSelection, const bool removeSelection)
{
    bool result(false);

    const vec2 mouseWorld(screenToWorldPos(mousePos));
    size_t pickedNode(0);
    if (m_NodeList.rfind_if([&mouseWorld](MaterialGeneratorNode* const node)
    {
        return node->pick(mouseWorld);
    }, pickedNode))
    {
        MaterialGeneratorNode* const selectedNode(m_NodeList[pickedNode]);
        m_EditSelectionCommand.beginCommand();
        m_EditSelectionCommand.doEditSelection(keepSelection, removeSelection, selectedNode);
        m_EditSelectionCommand.endCommand();
        result = true;
    }
    return result;
}

bool MaterialGeneratorGraph::doConnectStart( const vec2& mousePos )
{
    bool result(false);

    const vec2 mouseWorld(screenToWorldPos(mousePos));
    size_t pickedNode(0);
    size_t pickedConnecter(0);
    if (m_NodeList.rfind_if([&mouseWorld, &pickedConnecter](MaterialGeneratorNode* const node) -> bool
    {
        const vec2& mouseWorldScoped(mouseWorld);
        return node->getConnecters().rfind_if([&mouseWorldScoped](MaterialGeneratorNode::Connecter* const connecter) -> bool
        {
            return connecter->pick(mouseWorldScoped);
        }, pickedConnecter);
    }, pickedNode))
    {
        const MaterialGeneratorNode* const node(m_NodeList[pickedNode]);
        const MaterialGeneratorNode::Connecter* const connecter(node->getConnecters()[pickedConnecter]);
        m_ConnectNodeCommand.startConnect(node->getGuid(), !connecter->isInput(), static_cast<uint8>(connecter->getIndex()));

        m_GhostConnection->setPositionStart(connecter->getPosition());
        m_GhostConnection->setPositionEnd(connecter->getPosition());

        int mod = -1;
        if (connecter->isInput())
            mod = 1;
        m_GhostConnection->setBeginTangent(vec2(1.0f * mod, 0));
        m_GhostConnection->setEndTangent(vec2(-1.0f * mod, 0));

        result = true;
    }
    return result;
}
bool MaterialGeneratorGraph::doConnectEnd( const vec2& mousePos )
{
    bool result(false);

    const vec2 mouseWorld(screenToWorldPos(mousePos));
    size_t pickedNode(0);
    size_t pickedConnecter(0);
    if (m_NodeList.rfind_if([&mouseWorld, &pickedConnecter](MaterialGeneratorNode* const node) -> bool
    {
        const vec2& mouseWorldScoped(mouseWorld);
        return node->getConnecters().rfind_if([&mouseWorldScoped](MaterialGeneratorNode::Connecter* const connecter) -> bool
        {
            return connecter->pick(mouseWorldScoped);
        }, pickedConnecter);
    }, pickedNode))
    {
        const MaterialGeneratorNode* const node(m_NodeList[pickedNode]);
        const MaterialGeneratorNode::Connecter* const connecter(node->getConnecters()[pickedConnecter]);
        const bool isInput(connecter->isInput());
        if (isInput == m_ConnectNodeCommand.startedAtOutput())
        {
            result = m_ConnectNodeCommand.doConnect(node->getGuid(), connecter->getIndex());
        }
        else
        {
            m_ConnectNodeCommand.cancelConnect();
        }
    }
    else
    {
        m_ConnectNodeCommand.cancelConnect();
    }
    return result;
}


bool MaterialGeneratorGraph::isOpen() const
{
    return m_Window->isOpen();
}

void MaterialGeneratorGraph::draw2D( gfx::Canvas2D* canvas )
{
    gui::Canvas2Dnew* const cvs(canvas->getRenderer2D()->getNewCanvas());

    cvs->drawSprite(m_Background, vec2(0,0));

    const mat33 transform(mat33::createScale2D(vec2(m_Scale, m_Scale)) * mat33::createTranslation2D(-m_Offset));
    const vec2 transformedSize(static_cast<float>(m_View->getViewport().width) / m_Scale, static_cast<float>(m_View->getViewport().height) / m_Scale);
    const RectF clipRect(0.0f, 0.0f, static_cast<float>(m_View->getViewport().width), static_cast<float>(m_View->getViewport().height));
    m_NodeList.forEach([canvas, &transform, &clipRect](MaterialGeneratorNode* const node)
    {
        node->draw2D(canvas, transform, clipRect);
    });

    if (m_State == State_ConnectNode)
    {
        m_GhostConnection->draw2D(canvas, transform);
    }

    m_VisibleErrors.forEach([this, cvs](const ErrorMessage& msg)
    {
        const vec2 screenPos(worldToScreenPos(msg.m_Position));
        cvs->setColor(Color(1, 1, 1, (ERROR_FADE_TIME + std::min(0.0f, msg.m_TimeLeft - ERROR_FADE_TIME)) / ERROR_FADE_TIME));
        cvs->drawSprite(m_ErrorBackgroundSprite, screenPos - msg.m_TextSize / 2.0f - errorTextMarge, msg.m_TextSize + errorTextMarge * 2);
        cvs->fillText(*msg.m_Text, screenPos - msg.m_TextSize / 2.0f);
    });

    m_WebViewGui->draw2D(canvas);

    // DEBUG
    m_DebugText.clear();
    const vec2 mouseWorld(screenToWorldPos(CONTROLS->getMouse()->getPosition()));
    m_DebugText.addTextExt("&5F5Zoom: &AFA%.2f\n&5F5Region: &AFA%.2f, %.2f, %.2f, %.2f\n&5F5Mouse: &AFA%.2f, %.2f\n\n&5F5: Undo/Redo: &5F5\n", 
        m_Scale, m_Offset.x, m_Offset.y, transformedSize.x, transformedSize.y, mouseWorld.x, mouseWorld.y);

    const he::ObjectList<CommandTransaction>& transactions(m_CommandStack.getTransactions());
    const size_t transactionCount(transactions.size());
    const size_t undoIndex(m_CommandStack.getUndoIndex());
    for (size_t i(0); i < transactionCount; ++i)
    {
        m_DebugText.addTextExt("&%s  - %s\n", i < undoIndex? "AFA" : "ABA", transactions[i].getName().c_str());
    }
    
    cvs->setColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
    cvs->fillText(m_DebugText, vec2(14.0f, 14.0f));
    cvs->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
    cvs->fillText(m_DebugText, vec2(12.0f, 12.0f));
}

he::vec2 MaterialGeneratorGraph::screenToWorldPos( const vec2& mousePos ) const
{
    return m_Offset + mousePos / m_Scale;
}

he::vec2 MaterialGeneratorGraph::worldToScreenPos( const vec2& worldPos ) const
{
    return (worldPos - m_Offset) * m_Scale;
}

void MaterialGeneratorGraph::renderBackground()
{
    int width(m_View->getViewport().width);
    int height(m_View->getViewport().height);

    m_Background->invalidate(vec2((float)width,(float)height));

    gui::SpriteCreator* const cr(GUI->Sprites);

    cr->setActiveSprite(m_Background);

    cr->rectangle(vec2(0,0),vec2((float)width,(float)height));
    cr->setColor(Color(0.22f,0.22f,0.22f));
    cr->fill();

    cr->newPath();

    for (uint8 i(0); i < ((height / 20) + 1); ++i)
    {
        cr->moveTo(vec2(0, (float)(i * 20)));
        cr->lineTo(vec2((float)width, (float)(i * 20)));
    }

    for (uint8 i(0); i < ((width / 20) + 1); ++i)
    {
        cr->moveTo(vec2((float)(i * 20), 0));
        cr->lineTo(vec2((float)(i * 20), (float)(height)));
    }

    cr->setLineWidth(1.0f);
    cr->setColor(Color(0.0f,0.0f,0.0f,0.2f));
    cr->stroke();

    cr->renderSpriteAsync();
}
void MaterialGeneratorGraph::onViewResized()
{
    renderBackground();
}

MaterialGeneratorNode* MaterialGeneratorGraph::getNode( const Guid& guid ) const
{
    MaterialGeneratorNode* node(nullptr);
    size_t index(0);
    if (m_NodeList.find_if([&guid](const MaterialGeneratorNode* const node) -> bool
                            {
                                return node->getGuid() == guid;
                            }, index))
    {
        node = m_NodeList[index];
    }
    return node;
}

MaterialGeneratorNode* MaterialGeneratorGraph::getSelectedNode( const Guid& guid ) const
{
    MaterialGeneratorNode* node(nullptr);
    size_t index(0);
    if (m_SelectedNodeList.find_if([&guid](const MaterialGeneratorNode* const node) -> bool
    {
        return node->getGuid() == guid;
    }, index))
    {
        node = m_NodeList[index];
    }
    return node;
}

void MaterialGeneratorGraph::addNode( MaterialGeneratorNode* const node )
{
    m_NodeList.add(node);
    node->setParent(this);
}

void MaterialGeneratorGraph::pushError( const MaterialGeneratorError& errorMsg )
{
    HE_ASSERT(m_ErrorPool.empty() == false, "Error pool to small resizing... %d -> %d", m_VisibleErrors.size(), m_VisibleErrors.size() + 5);
    if (m_ErrorPool.empty())
    {
        increaseErrorPool(m_VisibleErrors.size() + 5);
    }

    ErrorMessage msg;
    msg.m_Text = m_ErrorPool.back();
    m_ErrorPool.removeAt(m_ErrorPool.size() - 1);
    msg.m_Text->clear();
    msg.m_Text->addTextExt("&F55%s", errorMsg.getMessage().c_str());
    msg.m_TextSize = msg.m_Text->measureText();
    msg.m_Position = screenToWorldPos(CONTROLS->getMouse()->getPosition());
    msg.m_TimeLeft = ERROR_TIME;
    m_VisibleErrors.add(msg);
}

void MaterialGeneratorGraph::increaseErrorPool(const size_t extraSize)
{
    m_ErrorPool.reserve(m_ErrorPool.size() + extraSize);
    for (uint8 i(0); i < 5; ++i)
    {
        gui::Text* text(NEW gui::Text);
        text->setHorizontalAlignment(gui::Text::HAlignment_Left);
        text->setVerticalAlignment(gui::Text::VAlignment_Top);
        text->setFont(m_ErrorFont);
        m_ErrorPool.add(text);
    }
}

#define VERSION 1ui16

void MaterialGeneratorGraph::save()
{
    io::BinaryFileVisitor stream;
    if (stream.openWrite(CONTENT->getShaderFolderPath().append("testShader.shader").str()))
    {
        uint16 version(VERSION);
        uint32 nodeCount(static_cast<uint32>(m_NodeList.size()));
        stream.visit(version);
        stream.visit(nodeCount);
        m_NodeList.forEach([&stream](MaterialGeneratorNode* const node)
        {
            MaterialGeneratorNodeType type(node->getType());
            stream.visitEnum<MaterialGeneratorNodeType, uint16>(type);
            stream.visit(type);
            node->visit(stream);
        });
        m_NodeList.forEach([&stream](const MaterialGeneratorNode* const node)
        {
            io::BinaryFileVisitor& scopedStream(stream);
            node->getConnecters().forEach([&scopedStream](MaterialGeneratorNode::Connecter* const connecter)
            {
                if (connecter->isInput())
                {
                    bool isConnected(connecter->isConnected());
                    scopedStream.visit(isConnected);
                    if (isConnected)
                    {
                        MaterialGeneratorNode::Connecter* const connection(connecter->getConnection());
                        Guid guid(connection->getParent()->getGuid());
                        uint8 index(connection->getIndex());
                        scopedStream.visit(guid);
                        scopedStream.visit(index);
                    }
                }
            });
        });
        stream.close();
    }
    else
    {
        LOG(LogType_ArtAssert, "File open failed!\nSave failed...");
    }
}

void MaterialGeneratorGraph::load()
{
    MaterialGeneratorNodeFactory* const factory(MaterialGeneratorNodeFactory::getInstance());
    m_NodeList.forEach([factory](MaterialGeneratorNode* const node)
    {
        factory->destroy(node);
    });
    m_NodeList.clear();
    m_NodeGraph.clear();
    m_CommandStack.clear();

    io::BinaryFileVisitor stream;
    if (stream.openRead(CONTENT->getShaderFolderPath().append("testShader.shader").str()))
    {
        uint16 version(0);
        stream.visit(version);
        if (version == VERSION)
        {
            uint32 size(0);
            stream.visit(size);
            for (uint32 i(0); i < size; ++i)
            {
                MaterialGeneratorNodeType type(MaterialGeneratorNodeType_Unassigned);
                stream.visitEnum<MaterialGeneratorNodeType, uint16>(type);
                MaterialGeneratorNode* const node(factory->create(type));
                addNode(node);
                node->visit(stream);
            }
            m_NodeGraph.addRootNode(m_NodeList[0]); // first node is always root
            m_NodeList.forEach([&stream, this](MaterialGeneratorNode* const node)
            {
                io::BinaryFileVisitor& scopedStream(stream);
                MaterialGeneratorGraph* const scopedThis(this);
                node->getConnecters().forEach([&scopedStream, node, scopedThis](MaterialGeneratorNode::Connecter* const connecter)
                {
                    if (connecter->isInput())
                    {
                        bool isConnected(false);
                        scopedStream.visit(isConnected);
                        if (isConnected)
                        {
                            Guid guid;
                            scopedStream.visit(guid);
                            uint8 index(0);
                            scopedStream.visit(index);
                            MaterialGeneratorNode* const otherNode(scopedThis->getNode(guid));
                            MaterialGeneratorError error;
                            node->connectToInput(otherNode, index, connecter->getIndex(), error);
                        }
                    }
                });
            });
        }
        else
        {
            LOG(LogType_ArtAssert, "Incompatible version!\nVersion = %d, code version = %d", version, VERSION);
        }

        stream.close();
    }
}


} } //end namespace
