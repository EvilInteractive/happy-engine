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
//Created: 14/04/2012

#include "HappyPCH.h" 

#include "WebView.h"
#include "Awesomium/BitmapSurface.h"
#include "Awesomium/WebView.h"
#include "Canvas2D.h"
#include "Renderer2D.h"
#include "GraphicsEngine.h"
#include "Texture2D.h"
#include "View.h"
#include "Window.h"

#include "ControlsManager.h"
#include "IKeyboard.h"
#include "IMouse.h"
#include "WebListener.h"

#define COMMON_ASCII_CHAR 128

namespace he {
namespace gui {

WebView::WebView(gfx::View* view, const RectI& viewport, bool enableUserInput) :   
m_WebView(nullptr),
    m_InputEnabled(enableUserInput),
    m_RenderTexture(nullptr),
    m_Position(static_cast<float>(viewport.x), static_cast<float>(viewport.y)),
    m_Size(-1, -1),
    m_View(view),
    m_ViewportPercent(0, 0, 1, 1),
    m_Buffer(nullptr),
    m_WebListener(nullptr),
    m_HasFocus(false)
{    
    init();
    resize(vec2(static_cast<float>(viewport.width), static_cast<float>(viewport.height)));
}
#pragma warning(disable:4355) // this is initializer list
WebView::WebView(gfx::View* view, const RectF& viewportPercent, bool enableUserInput) :   
    m_WebView(nullptr),
    m_InputEnabled(enableUserInput),
    m_RenderTexture(nullptr),
    m_Position(0, 0),
    m_Size(-1, -1),
    m_View(view),
    m_ViewportPercent(viewportPercent),
    m_ViewResizedHandler(boost::bind(&WebView::onViewResized, this)),
    m_Buffer(nullptr),
    m_WebListener(nullptr),
    m_HasFocus(false)
{    
    init();
    m_View->ViewportSizeChanged += m_ViewResizedHandler;
    onViewResized();
}
#pragma warning(default:4355)
void WebView::init()
{
    // create buffer
    m_Buffer = static_cast<uint8*>(he_malloc(4));

    // create webview
    m_WebView = GRAPHICS->getWebCore()->CreateWebView(1,1);
    m_WebListener = NEW WebListener(m_WebView);

    ObjectHandle handle = ResourceFactory<gfx::Texture2D>::getInstance()->create();
    m_RenderTexture = ResourceFactory<gfx::Texture2D>::getInstance()->get(handle);
    m_RenderTexture->init(gfx::TextureWrapType_Clamp, gfx::TextureFilterType_Nearest,
        gfx::TextureFormat_RGBA8, false);

    if (m_InputEnabled)
    {
        Awesomium::WebView* w(m_WebView);

        m_TextEnteredHandler = eventCallback1<void, uint32>([w,this](uint32 chr)
        {
            Awesomium::WebKeyboardEvent keyEvent;
            keyEvent.type = Awesomium::WebKeyboardEvent::kTypeChar;
            keyEvent.text[0] = (wchar16)chr;
            keyEvent.unmodified_text[0] = (wchar16)chr;
            keyEvent.native_key_code = chr;

            w->InjectKeyboardEvent(keyEvent);
        });

        m_KeyPressedHandler = eventCallback1<void, io::Key>([w,this](io::Key key)
        {
            // check if we cant get focus
            /*if (CONTROLS->getFocus(this) == false)
                return;*/

            Awesomium::WebKeyboardEvent keyEvent;

            uint32 chr = io::getWebKeyFromKey(key);

            keyEvent.virtual_key_code = chr;

            char buf[20];
            char* buffPtr(buf);
            Awesomium::GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buffPtr);
            strcpy(keyEvent.key_identifier, buf);

            keyEvent.modifiers = 0;
            keyEvent.native_key_code = 0;
            keyEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyDown;

            w->InjectKeyboardEvent(keyEvent);
            //CONTROLS->returnFocus(this);
        });

        m_KeyReleasedHandler = eventCallback1<void, io::Key>([w,this](io::Key key)
        {
            // check if we cant get focus
            /*if (CONTROLS->getFocus(this) == false)
                return;*/

            Awesomium::WebKeyboardEvent keyEvent;

            char buf[20];
            char* buffPtr(buf);
            keyEvent.virtual_key_code = io::getWebKeyFromKey(key);

            Awesomium::GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buffPtr);
            strcpy(keyEvent.key_identifier, buf);

            keyEvent.modifiers = 0;
            keyEvent.native_key_code = 0;
            keyEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyUp;

            w->InjectKeyboardEvent(keyEvent);

            CONTROLS->returnFocus(this);
        });

        m_MouseButtonPressedHandler = eventCallback1<void, io::MouseButton>([w,this](io::MouseButton but)
        {
            // check if we cant get focus
            /*if (CONTROLS->getFocus(this) == false)
                return;*/

            if (but == io::MouseButton_Left)
                w->InjectMouseDown(Awesomium::kMouseButton_Left);
            else if (but == io::MouseButton_Right)
                w->InjectMouseDown(Awesomium::kMouseButton_Right);
            else if (but == io::MouseButton_Middle)
                w->InjectMouseDown(Awesomium::kMouseButton_Middle);

            //CONTROLS->returnFocus(this);
        });

        m_MouseButtonReleasedHandler = eventCallback1<void, io::MouseButton>([w,this](io::MouseButton but)
        {
            // check if we cant get focus
            /*if (CONTROLS->getFocus(this) == false)
                return;*/

            if (but == io::MouseButton_Left)
                w->InjectMouseUp(Awesomium::kMouseButton_Left);
            else if (but == io::MouseButton_Right)
                w->InjectMouseUp(Awesomium::kMouseButton_Right);
            else if (but == io::MouseButton_Middle)
                w->InjectMouseUp(Awesomium::kMouseButton_Middle);

            //CONTROLS->returnFocus(this);
        });

        m_MouseMoveHandler = eventCallback1<void, const vec2&>([w,this](const vec2& pos)
        {
            // check if we cant get focus
            /*if (CONTROLS->getFocus(this) == false)
                return;*/

            w->InjectMouseMove(static_cast<int>(pos.x), static_cast<int>(pos.y));

            //CONTROLS->returnFocus(this);
        });

        m_MouseScrollHandler = eventCallback1<void, int>([w,this](int move)
        {
            // check if we cant get focus
            /*if (CONTROLS->getFocus(this) == false)
                return;*/

            w->InjectMouseWheel(move * 30, 0);

            //CONTROLS->returnFocus(this);
        });

        focus();
    }
}

WebView::~WebView()
{
    if (m_View != nullptr)
        m_View->ViewportSizeChanged -= m_ViewResizedHandler;
    if (m_InputEnabled)
    {
        io::IKeyboard* keyboard(CONTROLS->getKeyboard());
        io::IMouse* mouse(CONTROLS->getMouse());
        keyboard->TextCharEntered -= m_TextEnteredHandler;
        keyboard->KeyPressed -= m_KeyPressedHandler;
        keyboard->KeyReleased -= m_KeyReleasedHandler;
        mouse->MouseButtonPressed -= m_MouseButtonPressedHandler;
        mouse->MouseButtonReleased -= m_MouseButtonReleasedHandler;
        mouse->MouseMoved -= m_MouseMoveHandler;
        mouse->MouseWheelMoved -= m_MouseScrollHandler;
    }

    he_free(m_Buffer);
    m_WebView->Destroy();
    m_RenderTexture->release();
    delete m_WebListener;
}

/* GENERAL */
void WebView::draw2D(gui::Canvas2D* canvas)
{
    if (m_WebView->surface())
    {
        Awesomium::BitmapSurface* pSurface = static_cast<Awesomium::BitmapSurface*>(m_WebView->surface());

        if (pSurface->is_dirty())
        {
            pSurface->CopyTo(m_Buffer, pSurface->width() * 4, 4, false, true);

            m_RenderTexture->setData(
                pSurface->width(), pSurface->height(), 
                m_Buffer, 
                gfx::TextureBufferLayout_BGRA, gfx::TextureBufferType_Byte, 0);
        }
    }

    canvas->drawImage(m_RenderTexture, m_Position);
}

void WebView::loadUrl(const std::string& url)
{
    Awesomium::WebURL webUrl(Awesomium::WebString::CreateFromUTF8(url.c_str(), static_cast<unsigned int>(strlen(url.c_str()))));
    m_WebView->LoadURL(webUrl);

    m_WebView->set_load_listener(this);
    m_WebView->set_view_listener(this);
    m_WebView->set_js_method_handler(m_WebListener);
}

void WebView::loadFile(const he::Path& /*path*/)
{
    //m_WebView->LoadURL()
}

void WebView::executeJavaScript(const std::string& /*script*/)
{
    //Awesomium::WebString string(script.c_str());
    //m_WebView->ExecuteJavascriptWithResult(string);
}

void WebView::focus()
{
    HE_IF_ASSERT(m_HasFocus == false, "Webview already has focus")
    {
        m_HasFocus = true;
        m_WebView->Focus();
        if (m_InputEnabled)
        {
            io::IKeyboard* keyboard(CONTROLS->getKeyboard());
            io::IMouse* mouse(CONTROLS->getMouse());
            keyboard->TextCharEntered += m_TextEnteredHandler;
            keyboard->KeyPressed += m_KeyPressedHandler;
            keyboard->KeyReleased += m_KeyReleasedHandler;
            mouse->MouseButtonPressed += m_MouseButtonPressedHandler;
            mouse->MouseButtonReleased += m_MouseButtonReleasedHandler;
            mouse->MouseMoved += m_MouseMoveHandler;
            mouse->MouseWheelMoved += m_MouseScrollHandler;
        }
    }
}

void WebView::unfocus()
{
    HE_IF_ASSERT(m_HasFocus == true, "Webview does not have focus when unfocussing")
    {
        m_HasFocus = false;
        m_WebView->Unfocus();
        if (m_InputEnabled)
        {
            io::IKeyboard* keyboard(CONTROLS->getKeyboard());
            io::IMouse* mouse(CONTROLS->getMouse());
            keyboard->TextCharEntered -= m_TextEnteredHandler;
            keyboard->KeyPressed -= m_KeyPressedHandler;
            keyboard->KeyReleased -= m_KeyReleasedHandler;
            mouse->MouseButtonPressed -= m_MouseButtonPressedHandler;
            mouse->MouseButtonReleased -= m_MouseButtonReleasedHandler;
            mouse->MouseMoved -= m_MouseMoveHandler;
            mouse->MouseWheelMoved -= m_MouseScrollHandler;
        }
    }
}

void WebView::setTransparent(bool transparent)
{
    m_WebView->SetTransparent(transparent);
}

/* GETTERS */
Awesomium::WebView* WebView::getAWEView() const
{
    return m_WebView;
}

bool WebView::inputEnabled() const
{
    return m_InputEnabled;
}

/* EXTRA */
void WebView::onViewResized()
{
    const RectI& viewport(m_View->getViewport());
    m_Position.x = viewport.x * m_ViewportPercent.x;
    m_Position.y = viewport.y * m_ViewportPercent.y;

    resize(vec2(viewport.width * m_ViewportPercent.width, viewport.height * m_ViewportPercent.height));
}

void WebView::resize( const vec2& newSize )
{
    if (m_Size != newSize)
    {
        if (m_WebView != nullptr)
            m_WebView->Resize((int)newSize.x, (int)newSize.y);
        else
            m_WebView = GRAPHICS->getWebCore()->CreateWebView((int)newSize.x, (int)newSize.y);

        m_Buffer = static_cast<uint8*>(he_realloc(m_Buffer, (int)newSize.x * 4 * (int)newSize.y));
        m_Size = newSize;
    }
}

void WebView::OnFailLoadingFrame(
        Awesomium::WebView *  		/*caller*/,
        ::int64  						/*frame_id*/,
        bool  						/*is_main_frame*/,
        const Awesomium::WebURL&  	url,
        int  						/*error_code*/,
        const Awesomium::WebString& error_desc 
    )
{
    char* buff0 = NEW char[url.path().length()];
    url.path().ToUTF8(buff0, url.path().length());

    char* buff2 = NEW char[error_desc.length()];
    error_desc.ToUTF8(buff2, error_desc.length());

    HE_WARNING("Failed to load url: '%s', '%s'", buff0, buff2);

    delete[] buff0;
    delete[] buff2;
}

void WebView::OnFinishLoadingFrame(
        Awesomium::WebView *  		/*caller*/,
        ::int64  						/*frame_id*/,
        bool  						/*is_main_frame*/,
        const Awesomium::WebURL&  	url 
    )
{
    char buff0[200];
    HE_ASSERT(200 > url.path().length(), "Output buffer to small!");
    url.path().ToUTF8(buff0, std::max<uint32>(url.path().length(), 200));

    HE_INFO("Finished loading url: '%s'", buff0);

    OnUrlLoaded();
}

void WebView::OnDocumentReady(
        Awesomium::WebView *  		/*caller*/,
        const Awesomium::WebURL &  	/*url */
    )
{
}

void WebView::OnBeginLoadingFrame(
        Awesomium::WebView*			/*caller*/,
        ::int64						/*frame_id*/,
        bool						/*is_main_frame*/,
        const Awesomium::WebURL&	/*url*/,
        bool						/*is_error_page*/
    )
{
}

io::MouseCursor getCursorTypeFromAwesomium(const Awesomium::Cursor cursor)
{
    io::MouseCursor result(io::MouseCursor_Pointer);
    switch (cursor)
    {
        case Awesomium::kCursor_Pointer: result = io::MouseCursor_Pointer; break;
        case Awesomium::kCursor_Cross: result = io::MouseCursor_Cross; break;
        case Awesomium::kCursor_Hand: result = io::MouseCursor_Hand; break;
        case Awesomium::kCursor_IBeam: result = io::MouseCursor_IBeam; break;
        case Awesomium::kCursor_Wait: result = io::MouseCursor_Wait; break;
        case Awesomium::kCursor_Help: result = io::MouseCursor_Help; break;
        case Awesomium::kCursor_EastResize: result = io::MouseCursor_EastResize; break;
        case Awesomium::kCursor_NorthResize: result = io::MouseCursor_NorthResize; break;
        case Awesomium::kCursor_NorthEastResize: result = io::MouseCursor_NorthEastResize; break;
        case Awesomium::kCursor_NorthWestResize: result = io::MouseCursor_NorthWestResize; break;
        case Awesomium::kCursor_SouthResize: result = io::MouseCursor_SouthResize; break;
        case Awesomium::kCursor_SouthEastResize: result = io::MouseCursor_SouthEastResize; break;
        case Awesomium::kCursor_SouthWestResize: result = io::MouseCursor_SouthWestResize; break;
        case Awesomium::kCursor_WestResize: result = io::MouseCursor_WestResize; break;
        case Awesomium::kCursor_NorthSouthResize: result = io::MouseCursor_NorthSouthResize; break;
        case Awesomium::kCursor_EastWestResize: result = io::MouseCursor_EastWestResize; break;
        case Awesomium::kCursor_NorthEastSouthWestResize: result = io::MouseCursor_NorthEastSouthWestResize; break;
        case Awesomium::kCursor_NorthWestSouthEastResize: result = io::MouseCursor_NorthWestSouthEastResize; break;
        case Awesomium::kCursor_ColumnResize: result = io::MouseCursor_ColumnResize; break;
        case Awesomium::kCursor_RowResize: result = io::MouseCursor_RowResize; break;
        case Awesomium::kCursor_MiddlePanning: result = io::MouseCursor_MiddlePanning; break;
        case Awesomium::kCursor_EastPanning: result = io::MouseCursor_EastPanning; break;
        case Awesomium::kCursor_NorthPanning: result = io::MouseCursor_NorthPanning; break;
        case Awesomium::kCursor_NorthEastPanning: result = io::MouseCursor_NorthEastPanning; break;
        case Awesomium::kCursor_NorthWestPanning: result = io::MouseCursor_NorthWestPanning; break;
        case Awesomium::kCursor_SouthPanning: result = io::MouseCursor_SouthPanning; break;
        case Awesomium::kCursor_SouthEastPanning: result = io::MouseCursor_SouthEastPanning; break;
        case Awesomium::kCursor_SouthWestPanning: result = io::MouseCursor_SouthWestPanning; break;
        case Awesomium::kCursor_WestPanning: result = io::MouseCursor_WestPanning; break;
        case Awesomium::kCursor_Move: result = io::MouseCursor_Move; break;
        case Awesomium::kCursor_VerticalText: result = io::MouseCursor_VerticalText; break;
        case Awesomium::kCursor_Cell: result = io::MouseCursor_Cell; break;
        case Awesomium::kCursor_ContextMenu: result = io::MouseCursor_ContextMenu; break;
        case Awesomium::kCursor_Alias: result = io::MouseCursor_Alias; break;
        case Awesomium::kCursor_Progress: result = io::MouseCursor_Progress; break;
        case Awesomium::kCursor_NoDrop: result = io::MouseCursor_NoDrop; break;
        case Awesomium::kCursor_Copy: result = io::MouseCursor_Copy; break;
        case Awesomium::kCursor_None: result = io::MouseCursor_None; break;
        case Awesomium::kCursor_NotAllowed: result = io::MouseCursor_NotAllowed; break;
        case Awesomium::kCursor_ZoomIn: result = io::MouseCursor_ZoomIn; break;
        case Awesomium::kCursor_ZoomOut: result = io::MouseCursor_ZoomOut; break;
        case Awesomium::kCursor_Grab: result = io::MouseCursor_Grab; break;
        case Awesomium::kCursor_Grabbing: result = io::MouseCursor_Grabbing; break;
        case Awesomium::kCursor_Custom: result = io::MouseCursor_Custom; break;
        default:
            LOG(LogType_ProgrammerAssert, "Unknown Awesomium mouse cursor"); break;
    }
    return result;
}
void WebView::OnChangeCursor( Awesomium::WebView* /*caller*/, Awesomium::Cursor cursor )
{
    m_View->getWindow()->setCursor(getCursorTypeFromAwesomium(cursor));
}

void WebView::OnAddConsoleMessage(
        Awesomium::WebView* /*caller*/,
        const Awesomium::WebString& msg,int lineNr,
        const Awesomium::WebString& source)
{
    char* buff0 = NEW char[msg.length()];
    msg.ToUTF8(buff0, msg.length());

    char* buff1 = NEW char[source.length()];
    source.ToUTF8(buff1, source.length());

    HE_INFO("JS Console: msg:'%s', lineNr:'%d', source:'%s'", buff0, lineNr, buff1);

    delete[] buff0;
    delete[] buff1;
}

}} //end namespace