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
//Author:  
//Created: //
#include "HappyPCH.h" 

#include "MessageBox.h"
#include "GraphicsEngine.h"

#include "Window.h"
#include "View.h"
#include "RenderSettings.h"

#include "Text.h"
#include "Button.h"
#include "Font.h"
#include "ContentManager.h"

#include "ControlsManager.h"
#include "IMouse.h"

#include "Renderer2D.h"
#include "Canvas2D.h"

#include "IDrawable2D.h"
#include "Texture2D.h"

namespace he {

#define BORDER 12.0f
#define SMALL_MARGE 6.0f
#define MEDIUM_MARGE 8.0f
#define FONT_SIZE 12
#define TEXT_MARGIN 3
#define BUTTON_MARGE 45.0f
const vec2 iconSize(96, 96);
const vec2 iconPos(BORDER, BORDER);
const vec2 windowSize(512, 256);
const vec2 buttonSize(128, 30);
const vec2 textboxPos(iconPos.x + iconSize.x + SMALL_MARGE, BORDER);
const vec2 textboxSize(windowSize - textboxPos - vec2(BORDER, MEDIUM_MARGE*2 + buttonSize.y));
const vec2 buttonBoxPos(BORDER, iconPos.y + iconSize.y + SMALL_MARGE);
const vec2 buttonBoxSize(iconSize.x, windowSize.y - buttonBoxPos.y - MEDIUM_MARGE*2 - buttonSize.y);

namespace details {
class MessageboxDrawer : public gfx::IDrawable2D
{
public:
    gui::Button m_Buttons[3];
    gui::Text   m_TextBlock;
    uint8       m_ButtonCount;
    const gfx::Texture2D* m_Icon;

    MessageboxDrawer(const std::string& message, MessageBoxIcon icon,
        const std::string& button1, const std::string& button2, const std::string& button3):
            m_TextBlock()
    {
        switch (icon)
        {
        case MessageBoxIcon_Info:
            m_Icon = CONTENT->loadTexture2D("engine/messagebox/info.png");
            break;
        case MessageBoxIcon_Warning:
            m_Icon = CONTENT->loadTexture2D("engine/messagebox/warning.png");
            break;
        case MessageBoxIcon_Error:
            m_Icon = CONTENT->loadTexture2D("engine/messagebox/error.png");
            break;
        case MessageBoxIcon_Success:
            m_Icon = CONTENT->loadTexture2D("engine/messagebox/success.png");
            break;
        case MessageBoxIcon_ProgrammerAssert:
            m_Icon = CONTENT->loadTexture2D("engine/messagebox/assert_programmer.png");
            break;
        case MessageBoxIcon_ArtAssert:
            m_Icon = CONTENT->loadTexture2D("engine/messagebox/assert_artist.png");
            break;
        }

        m_ButtonCount = 1;
        if (button3 != "")
            m_ButtonCount = 3;
        else if (button2 != "")
            m_ButtonCount = 2;

        gfx::Font* font(CONTENT->loadFont("Ubuntu-Regular.ttf", FONT_SIZE));
        m_TextBlock.addText(message);
        m_TextBlock.setFont(font);
        m_TextBlock.setOverFlowType(gui::Text::OverFlowType_Wrap);
        m_TextBlock.setBounds(textboxSize - vec2(TEXT_MARGIN*2, TEXT_MARGIN*2));
        font->release();

        for (uint8 i(0); i < m_ButtonCount; ++i)
        {
            if (i == 0)
                m_Buttons[i].setText(button1, FONT_SIZE);
            else if (i == 1)
                m_Buttons[i].setText(button2, FONT_SIZE);
            else
                m_Buttons[i].setText(button3, FONT_SIZE);
            m_Buttons[i].setSize(buttonSize);

            vec2 pos(BORDER, windowSize.y - MEDIUM_MARGE - buttonSize.y);
            pos += buttonSize / 2;
            if (m_ButtonCount == 3)
            {
                if (i == 1)
                    pos.x = windowSize.x / 2;
                else if (i == 2)
                    pos.x = windowSize.x - BORDER - buttonSize.x / 2;              
            }
            else if (m_ButtonCount == 2)
            {
                if (i == 0)
                    pos.x = windowSize.x / 3;
                else
                    pos.x = windowSize.x / 3 * 2;
            }
            else if (m_ButtonCount == 1)
            {
                pos.x = windowSize.x / 2;
            }
            m_Buttons[i].setPosition(pos);
            m_Buttons[i].setType(gui::Button::TYPE_NORMAL);
            m_Buttons[i].setActivationMode(gui::Button::ACTIVATION_MOUSE);
            
        }
    }
    virtual ~MessageboxDrawer()
    {
        m_Icon->release();
    }

    virtual void draw2D(gfx::Canvas2D* renderer) 
    {
        renderer->setFillColor(Color(243ui8, 249, 253));
        renderer->fillRect(vec2(0, 0), windowSize);

        renderer->setFillColor(Color(197ui8, 209, 217));
        renderer->fillRect(textboxPos, textboxSize);
        renderer->fillRect(buttonBoxPos, buttonBoxSize);
        renderer->fillRect(iconPos, iconSize);

        renderer->setStrokeColor(Color(26ui8, 68, 95));
        renderer->strokeRect(textboxPos, textboxSize);
        renderer->strokeRect(buttonBoxPos, buttonBoxSize);
        renderer->strokeRect(iconPos, iconSize);

        renderer->drawImage(m_Icon, iconPos, iconSize);

        renderer->setFillColor(Color(0ui8, 0, 0));
        renderer->fillText(m_TextBlock, textboxPos + vec2(TEXT_MARGIN, TEXT_MARGIN));
        for (uint8 i(0); i < m_ButtonCount; ++i)
        {
            m_Buttons[i].draw2D(renderer);
        }
    }
};
}


boost::mutex MessageBox::s_Mutex;
MessageBoxButton MessageBox::s_Result(MessageBoxButton_None);


MessageBox::MessageBox()
{
}


MessageBox::~MessageBox()
{
}

MessageBoxButton MessageBox::showExt(const std::string& caption, const std::string& message, MessageBoxIcon icon,
                    const std::string& button1, const std::string& button2, const std::string& button3)
{
    MessageBoxButton result(MessageBoxButton_None);
    if (GRAPHICS == nullptr)
        return result;
    if (s_Mutex.try_lock())
    {
        //////////////////////////////////////////////////////////////////////////
        // Init
        //////////////////////////////////////////////////////////////////////////
        gfx::Window* window(GRAPHICS->createWindow());
        gfx::View* view(GRAPHICS->createView());
        gfx::Renderer2D* renderer(NEW gfx::Renderer2D());
        view->addRenderPlugin(renderer);

        window->setResizable(false);
        window->setWindowDimension(static_cast<uint32>(windowSize.x), static_cast<uint32>(windowSize.y));
        vec2 mousePos(CONTROLS->getMouse()->getPosition());
        window->setWindowPosition(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));
        window->setVSync(true);
        window->setCursorVisible(true);
        window->setWindowTitle(caption);
        he::eventCallback0<void> closeCallback([]()
        { 
            s_Result = MessageBoxButton_Escape; 
        });
        window->Closed += closeCallback;
        window->create();

        view->setWindow(window);
        view->setRelativeViewport(RectF(0, 0, 1, 1));
        view->init(gfx::RenderSettings());

        details::MessageboxDrawer drawer(message, icon, button1, button2, button3);
        renderer->attachToRender(&drawer);

        for (uint8 i(0); i < drawer.m_ButtonCount; ++i)
        {
            he::eventCallback0<void> clickCallback([i]()
            { 
                s_Result = (MessageBoxButton)(MessageBoxButton_Button1 + i); 
            });
            drawer.m_Buttons[i].OnClick += clickCallback;
        }

        //////////////////////////////////////////////////////////////////////////
        // Loop
        //////////////////////////////////////////////////////////////////////////
        window->prepareForRendering();
        while (s_Result == MessageBoxButton_None)
        {
            CONTROLS->getMouse()->tick();

            window->doEvents(1/30.0f);

            for (uint8 i(0); i < drawer.m_ButtonCount; ++i)
            {
                drawer.m_Buttons[i].tick();
            }
            view->draw();
        }

        //////////////////////////////////////////////////////////////////////////
        // Cleanup
        //////////////////////////////////////////////////////////////////////////
        renderer->detachFromRender(&drawer);

        window->Closed -= closeCallback;
        window->close();

        delete renderer;
        GRAPHICS->removeView(view);
        GRAPHICS->removeWindow(window);

        result = s_Result;
        s_Result = MessageBoxButton_None;

        s_Mutex.unlock();
    }
    return result;
}



} //end namespace
