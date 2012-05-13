//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Author:  Sebastiaan Sprengers
//Created: 19/03/2012

#include "HappyPCH.h" 

#include "Renderer2D.h"
#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "Happy2DRenderer.h"
#include "ControlsManager.h"

namespace he {
namespace gfx {

Renderer2D::Renderer2D() :  m_WebCore(nullptr),
                            m_TextureEffect(NEW Simple2DTextureEffect()),
                            m_TextureQuad(nullptr)
{
}

Renderer2D::~Renderer2D()
{
    if (m_WebCore != nullptr)
    {
        Awesomium::WebCore::Shutdown();
    }

    delete m_TextureEffect;
    m_TextureQuad->release();
}

/* GENERAL */
Canvas2D* Renderer2D::createCanvas(const vec2& size)
{
    vec2 dim = size;;

    if (size.x <= 0 || size.y <= 0)
    {
        dim.x = (float)GRAPHICS->getScreenWidth();
        dim.y = (float)GRAPHICS->getScreenHeight();
    }

    Canvas2D::Data* data = Canvas2D::create(dim);

    if (data == nullptr)
    {
        HE_ERROR("Failed to create Canvas2D!");
        return nullptr;
    }

    return NEW Canvas2D(data, dim);
}

WebView* Renderer2D::createWebView(bool enableUserInput, const vec2& size)
{
    if (m_WebCore == nullptr)
    {
        m_WebCore = Awesomium::WebCore::Initialize(Awesomium::Config());
    }

    vec2 dim = size;
    bool f = false;

    if (size == vec2(0,0))
    {
        dim.x = (float)GRAPHICS->getScreenWidth();
        dim.y = (float)GRAPHICS->getScreenHeight();

        f = true;
    }

    Awesomium::WebView* pView = m_WebCore->CreateWebView(dim.x, dim.y);

    WebView* web = NEW WebView(pView, enableUserInput, f);
    m_WebViews.push_back(web);

    if (enableUserInput)
    {
        Awesomium::WebView* w = web->getAWEView();

        CONTROLS->getKeyboard()->addOnKeyPressedListener([&, w](io::Key key)
        {
            if (w != nullptr)
            {
                Awesomium::WebKeyboardEvent keyEvent;

                uint chr = io::getWebKeyFromKey(key);

                keyEvent.virtual_key_code = chr;
                char* buf = new char[20];

                Awesomium::GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buf);
                strcpy(keyEvent.key_identifier, buf);

                delete[] buf;

                keyEvent.modifiers = 0;
                keyEvent.native_key_code = 0;
                keyEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyDown;

                w->InjectKeyboardEvent(keyEvent);

                // if it is an ASCII char
                if (chr < 128)
                {
                    // if letter
                    if (chr >= 65 && chr <= 90)
                    {
                        if (!(CONTROLS->getKeyboard()->isKeyDown(io::Key_Lshift) ||
                            CONTROLS->getKeyboard()->isKeyDown(io::Key_Rshift)))
                        {
                            chr += 32; // to lowercase ASCII
                        }
                    }

                    keyEvent.type = Awesomium::WebKeyboardEvent::kTypeChar;
                    keyEvent.text[0] = chr;
                    keyEvent.unmodified_text[0] = chr;
                    keyEvent.native_key_code = chr;

                    w->InjectKeyboardEvent(keyEvent);
                }
            }
        });

        CONTROLS->getKeyboard()->addOnKeyReleasedListener([&, w](io::Key key)
        {
            if (w != nullptr)
            {
                Awesomium::WebKeyboardEvent keyEvent;

                char* buf = new char[20];
                keyEvent.virtual_key_code = io::getWebKeyFromKey(key);

                Awesomium::GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buf);
                strcpy(keyEvent.key_identifier, buf);

                delete[] buf;

                keyEvent.modifiers = 0;
                keyEvent.native_key_code = 0;
                keyEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyUp;

                w->InjectKeyboardEvent(keyEvent);
            }
        });

        CONTROLS->getMouse()->addOnButtonPressedListener([&, w](io::MouseButton but)
        {
            if (w != nullptr)
            {
                if (but == io::MouseButton_Left)
                    w->InjectMouseDown(Awesomium::kLeftMouseButton);
                else if (but == io::MouseButton_Right)
                    w->InjectMouseDown(Awesomium::kRightMouseButton);
                else if (but == io::MouseButton_Middle)
                    w->InjectMouseDown(Awesomium::kMiddleMouseButton);
            }
        });

        CONTROLS->getMouse()->addOnButtonReleasedListener([&, w](io::MouseButton but)
        {
            if (w != nullptr)
            {
                if (but == io::MouseButton_Left)
                    w->InjectMouseUp(Awesomium::kLeftMouseButton);
                else if (but == io::MouseButton_Right)
                    w->InjectMouseUp(Awesomium::kRightMouseButton);
                else if (but == io::MouseButton_Middle)
                    w->InjectMouseUp(Awesomium::kMiddleMouseButton);
            }
        });

        CONTROLS->getMouse()->addOnMouseMovedListener([&, w](const vec2& pos)
        {
            if (w != nullptr)
            {
                w->InjectMouseMove(static_cast<int>(pos.x), static_cast<int>(pos.y));
            }
        });

        CONTROLS->getMouse()->addOnMouseWheelMovedListener([&, w](int move)
        {
            if (w != nullptr)
            {
                w->InjectMouseWheel(move * 30, 0);
            }
        });
    }

    return web;
}

void Renderer2D::tick()
{
    if (m_WebCore != nullptr)
    {
        m_WebCore->Update();
    }
}

void Renderer2D::draw()
{
}

void Renderer2D::init()
{
    m_TextureEffect->load();

    BufferLayout vLayout;
    vLayout.addElement(BufferElement(0, BufferElement::Type_Vec2, BufferElement::Usage_Position, 8, 0));
    vLayout.addElement(BufferElement(1, BufferElement::Type_Vec2, BufferElement::Usage_TextureCoordinate, 8, 8));

    // model texturequad
    m_TextureQuad = ResourceFactory<ModelMesh>::getInstance()->get(ResourceFactory<ModelMesh>::getInstance()->create());
    m_TextureQuad->init();

    std::vector<VertexPosTex2D> vertices;
    vertices.push_back(
        VertexPosTex2D(vec2(-0.5f, 0.5f),
        vec2(0, 0)));

    vertices.push_back(
        VertexPosTex2D(vec2(0.5f, 0.5f),
        vec2(1, 0)));

    vertices.push_back(
        VertexPosTex2D(vec2(-0.5f, -0.5f),
        vec2(0, 1)));

    vertices.push_back(
        VertexPosTex2D(vec2(0.5f, -0.5f),
        vec2(1, 1)));

    std::vector<byte> indices;
    indices.push_back(2); indices.push_back(1); indices.push_back(0);
    indices.push_back(1); indices.push_back(2); indices.push_back(3);

    m_TextureQuad->setVertices(&vertices[0], 4, vLayout);
    m_TextureQuad->setIndices(&indices[0], 6, IndexStride_Byte);
}

void Renderer2D::drawTexture2DToScreen( const Texture2D* tex2D, const vec2& pos,
                                        bool useBlending, const vec2& newDimensions,
                                        const RectF& regionToDraw)
{
    vec2 tcOffset(0.0f,0.0f);
    vec2 tcScale(1.0f,1.0f);
    vec2 size;

    if (regionToDraw != RectF(vec2(0.0f,0.0f), vec2(0.0f,0.0f)))
    {
        tcScale.x = regionToDraw.width / tex2D->getWidth();
        tcScale.y = regionToDraw.height / tex2D->getHeight();

        tcOffset.x = regionToDraw.x / tex2D->getWidth();
        tcOffset.y = 1 - (regionToDraw.y / tex2D->getHeight()) - tcScale.y;
    }

    if (newDimensions != vec2(0.0f,0.0f))
    {
        size = vec2(abs(newDimensions.x), abs(newDimensions.y));

        if (newDimensions.x < 0) tcScale.x *= -1.0f;
        if (newDimensions.y < 0) tcScale.y *= -1.0f;
    }
    else
    {
        size = vec2(static_cast<float>(tex2D->getWidth()), static_cast<float>(tex2D->getHeight()));
    }

    mat44 world(mat44::createTranslation(vec3(pos.x + size.x/2, pos.y + size.y/2, 0.0f)) * mat44::createScale(size.x, size.y, 1.0f));
    mat44 orthographicMatrix = mat44::createOrthoLH(0.0f, (float)GRAPHICS->getScreenWidth(), 0.0f, (float)GRAPHICS->getScreenHeight(), 0.0f, 100.0f);

    m_TextureEffect->begin();
    m_TextureEffect->setWorldMatrix(orthographicMatrix * world);
    m_TextureEffect->setDiffuseMap(tex2D);
    m_TextureEffect->setAlpha(1.0f);
    m_TextureEffect->setTCOffset(tcOffset);
    m_TextureEffect->setTCScale(tcScale);
    m_TextureEffect->setDepth(0.5f);

    if (useBlending == true)
    {
        GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
        GL::heBlendEnabled(true);
    }
    else
    {
        GL::heBlendEnabled(false);
    }

    GL::heBindFbo(0);

    GL::heBindVao(m_TextureQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_TextureQuad->getNumIndices(), m_TextureQuad->getIndexType(), 0);
}

} } //end namespace