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
//Created: 2013/04/01

#include "HappySandBoxPCH.h" 

#include "UIMain.h"
#include "Sandbox.h"
#include "system/SandboxRenderPipeline.h"
#include "system/EntityManager.h"

#include <WebView.h>
#include <WebListener.h>

namespace hs {

UIMain::UIMain()
    : m_WebView(nullptr)
{
}

UIMain::~UIMain()
{
}

void UIMain::load()
{
    HE_IF_ASSERT(m_WebView == nullptr, "UIMain already loaded!")
    {
        Sandbox* const sandbox(Sandbox::getInstance());
        UIRenderer* const renderer(sandbox->getRenderPipeline()->getUIRenderer());
        m_WebView = renderer->createWebViewRelative(he::RectF(0, 0, 1, 1), true);
        he::eventCallback0<void> onUrlLoaded([this, renderer]()
        {
            he::gui::WebListener* const listener(m_WebView->getWebListener());

            // Test
            he::eventCallback1<void, const Awesomium::JSArray&> callbackTest([](const Awesomium::JSArray& /*args*/){ CONSOLE->addMessage("testing gui", he::CMSG_TYPE_INFO);});
            listener->addObjectCallback("HE", "test", callbackTest);

            // Create Entity
            he::eventCallback1<void, const Awesomium::JSArray&> menuCreateEntityCallback(boost::bind(&UIMain::createEntityHandler, this, _1));
            listener->addObjectCallback("HE", "menuCreateEntity", menuCreateEntityCallback);
        });
        m_WebView->OnUrlLoaded += onUrlLoaded;
        he::Path url(he::Path::getWorkingDir().append("../../data/gui/main.html"));
        m_WebView->loadUrl(url);
    }
}

void UIMain::unload()
{
    HE_IF_ASSERT(m_WebView != nullptr, "UIMain already unloaded!")
    {
        if (isVisible() == true)
        {
            hide();
        }
        Sandbox* const sandbox(Sandbox::getInstance());
        UIRenderer* const renderer(sandbox->getRenderPipeline()->getUIRenderer());
        renderer->removeWebView(m_WebView);
        m_WebView = nullptr;
    }
}

void UIMain::show()
{
    UI::show();
    Sandbox* const sandbox(Sandbox::getInstance());
    UIRenderer* const renderer(sandbox->getRenderPipeline()->getUIRenderer());
    renderer->attachToRender(m_WebView);
}

void UIMain::hide()
{
    UI::hide();
    Sandbox* const sandbox(Sandbox::getInstance());
    UIRenderer* const renderer(sandbox->getRenderPipeline()->getUIRenderer());
    renderer->detachFromRender(m_WebView);
}

void UIMain::createEntityHandler(const Awesomium::JSArray& /*args*/)
{
    Sandbox* const sandbox(Sandbox::getInstance());
    sandbox->getEntityManager()->createEntity();
}

} //end namespace