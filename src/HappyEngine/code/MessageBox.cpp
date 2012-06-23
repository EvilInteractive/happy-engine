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

namespace he {

MessageBox::MessageBox()
{
}


MessageBox::~MessageBox()
{
}

MessageBoxButton MessageBox::show(const std::string& caption, const std::string& message,
                    const std::string& button1, const std::string& button2, const std::string& button3)
{
    #define MARGE 8 
    #define MESSAGE_BUTTON_MARGE 16
    #define BUTTON_TEXT_MARGE 8
    #define BUTTON_SPACING 32
    #define BUTTON_COLOR sf::Color(128, 128, 128, 255)
    #define BUTTON_HIGHLIGHT_COLOR sf::Color(200, 200, 200, 255)

    byte buttonCount(1);
    if (button3 != "")
        buttonCount = 3;
    else if (button2 != "")
        buttonCount = 2;

    sf::Text buttonText[3];
    sf::RectangleShape buttonRect[3];
    float buttonWidth(0), buttonHeight(0);
    for (byte i(0); i < buttonCount; ++i)
    {
        if (i == 0)
            buttonText[i].setString(button1);
        else if (i == 1)
            buttonText[i].setString(button2);
        else
            buttonText[i].setString(button3);
        buttonText[i].setCharacterSize(14);
        buttonText[i].setColor(sf::Color(0, 0, 0, 255));

        sf::FloatRect buttonTextSize(buttonText[i].getGlobalBounds());
        buttonWidth = he::max(buttonWidth, buttonTextSize.width);
        buttonHeight = he::max(buttonHeight, buttonTextSize.height);
    }
    buttonWidth += BUTTON_TEXT_MARGE * 2;
    buttonHeight += BUTTON_TEXT_MARGE * 2;

    sf::Text text(message);
    text.setCharacterSize(14);
    text.setColor(sf::Color(0, 0, 0, 255));
    text.setPosition(MARGE, MARGE);
    sf::FloatRect textSize(text.getGlobalBounds());

    float windowWidth(max(textSize.width, buttonWidth + (buttonCount - 1) * BUTTON_SPACING) + 2 * MARGE);
    float windowHeight(textSize.height + MARGE*2 + MESSAGE_BUTTON_MARGE + buttonHeight);

    sf::Vector2f buttonSize(buttonWidth, buttonHeight);
    for (byte i(0); i < buttonCount; ++i)
    {
        buttonRect[i].setSize(buttonSize);
        buttonRect[i].setOutlineThickness(1.0f);
        buttonRect[i].setOutlineColor(sf::Color(0, 0, 0, 255));
        buttonRect[i].setFillColor(BUTTON_COLOR);
        sf::FloatRect buttonTextSize(buttonText[i].getGlobalBounds());
        if (buttonCount == 1 || (buttonCount == 3 && i == 1))
        { //center align
            buttonText[i].setPosition(
                windowWidth / 2.0f - buttonTextSize.width / 2.0f, 
                windowHeight - MARGE - buttonHeight / 2.0f - buttonTextSize.height / 2.0f);
            buttonRect[i].setPosition(
                windowWidth / 2.0f - buttonWidth / 2.0f, 
                windowHeight - MARGE - buttonHeight);
        }
        else if (i == 0)
        { //left align
            buttonText[i].setPosition(
                MARGE + buttonWidth / 2.0f - buttonTextSize.width / 2.0f, 
                windowHeight - MARGE - buttonHeight / 2.0f - buttonTextSize.height / 2.0f);
            buttonRect[i].setPosition(
                MARGE, 
                windowHeight - MARGE - buttonHeight);
        }
        else
        { //right align
            buttonText[i].setPosition(
                windowWidth - MARGE - buttonWidth / 2.0f - buttonTextSize.width / 2.0f, 
                windowHeight - MARGE - buttonHeight / 2.0f - buttonTextSize.height / 2.0f);
            buttonRect[i].setPosition(
                windowWidth - MARGE - buttonWidth, 
                windowHeight - MARGE - buttonHeight);
        }
    }

    sf::RenderWindow window(sf::VideoMode(static_cast<uint>(windowWidth), static_cast<uint>(windowHeight)), 
        caption, sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    
    MessageBoxButton result(MessageBoxButton_Escape);
    
    window.setActive();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                result = MessageBoxButton_Escape;
                break;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                    result = MessageBoxButton_Escape;
                    break;
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.key.code == sf::Mouse::Left)
                {
                    for (byte i(0); i < buttonCount; ++i)
                    {
                        int x(event.mouseButton.x);
                        int y(event.mouseButton.y);
                        sf::FloatRect rect(buttonRect[i].getGlobalBounds());
                        if (x > rect.left && x < rect.left + rect.width &&
                            y > rect.top && y < rect.top + rect.height)
                        {
                            window.close();
                            if (i == 0)
                                result = MessageBoxButton_Button1;
                            else if (i == 1)
                                result = MessageBoxButton_Button2;
                            else
                                result = MessageBoxButton_Button3;
                            break;
                        }
                    }
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                for (byte i(0); i < buttonCount; ++i)
                {
                    int x(event.mouseMove.x);
                    int y(event.mouseMove.y);
                    sf::FloatRect rect(buttonRect[i].getGlobalBounds());
                    if (x > rect.left && x < rect.left + rect.width &&
                        y > rect.top && y < rect.top + rect.height)
                    {
                        buttonRect[i].setFillColor(BUTTON_HIGHLIGHT_COLOR);
                    }
                    else
                    {
                        buttonRect[i].setFillColor(BUTTON_COLOR);
                    }
                }
            }
        }
        window.clear(sf::Color(220, 218, 220, 255));
        window.draw(text);
        for (byte i(0); i < buttonCount; ++i)
        {
            window.draw(buttonRect[i]);
            window.draw(buttonText[i]);
        }
        window.display();
    }
    window.setActive(false);
    GRAPHICS->getWindow()->setActive(true);

    return result;
}


} //end namespace
