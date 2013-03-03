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
//Created: 02/01/2013
#include "HappyPCH.h" 

#include "MessageBox.h"
#include "WebWindow.h"

#include "ContentManager.h"
#include "GraphicsEngine.h"
#include "WebListener.h"

#include "Awesomium/STLHelpers.h"

namespace he {
    
MessageBox::MessageBox()
{
}


MessageBox::~MessageBox()
{
}

MessageBoxButton MessageBox::showExt(const std::string& caption, const std::string& message, MessageBoxIcon /*icon*/,
                    const std::string& button1, const std::string& button2, const std::string& button3)
{
    MessageBoxButton result(MessageBoxButton_None);
    if (GRAPHICS != nullptr)
    {
        gfx::WebWindow window;
        window.init(512, 256);
        window.setTitle(caption);

        he::eventCallback1<void, const Awesomium::JSArray&> onButtonClicked([&](const Awesomium::JSArray& args)
        {
            std::string button(Awesomium::ToString(args[0].ToString()));
            if (strcmp(button1.c_str(), button.c_str()) == 0)
                result = MessageBoxButton_Button1;
            else if (strcmp(button2.c_str(), button.c_str()) == 0)
                result = MessageBoxButton_Button2;
            else if (strcmp(button3.c_str(), button.c_str()) == 0)
                result = MessageBoxButton_Button3;
            window.close();
        });
        he::eventCallback0<void> onloaded([&]()
        {
            gfx::WebListener* const listener(window.getWebListener());
            listener->addObjectCallback("HE", "onButtonClicked", onButtonClicked);

            Awesomium::JSArray array;
            array.Push(Awesomium::JSValue(Awesomium::WSLit(button1.c_str())));
            listener->executeFunction("", "addButton", array);
            if (button2.empty() == false)
            {
                array.Clear();
                array.Push(Awesomium::JSValue(Awesomium::WSLit(button2.c_str())));
                listener->executeFunction("", "addButton", array);
            }
            if (button3.empty() == false)
            {
                array.Clear();
                array.Push(Awesomium::JSValue(Awesomium::WSLit(button3.c_str())));
                listener->executeFunction("", "addButton", array);
            }
            array.Clear();
            array.Push(Awesomium::JSValue(Awesomium::WSLit(message.c_str())));
            listener->executeFunction("", "setMessageText", array);
        });
        window.OnUrlLoaded += onloaded;
        window.loadUrl(Path::getWorkingDir().append(CONTENT->getContentDir().append("gui/messageBox.html").str()).str());
        
        gfx::GraphicsEngine* const gfxEngine(GRAPHICS);
        while (window.isOpen())
        {
            window.tick();
            gfxEngine->tick(1 / 30.0f);
        }
    }
    return result;
}



} //end namespace
