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
//Author:  Sebastiaan Sprengers
//Created: 10/07/2012

#include "HappyPCH.h" 
#ifdef USE_WEB
#include "JSObject.h"
#include "Awesomium/WebView.h"
#pragma warning(disable:4267) // conversion warning
#include "Awesomium/STLHelpers.h"
#pragma warning(default:4267)

namespace he {
namespace gui {

JSObject::JSObject(Awesomium::JSObject& aweObject, const std::string& name) : m_AweObject(aweObject),
                                                                              m_ObjectName(name)
{
}

JSObject::~JSObject()
{
    std::for_each(m_MethodCallBacks.cbegin(), m_MethodCallBacks.cend(),
        [](const std::pair<Awesomium::WebString,
        event1<void, const Awesomium::JSArray&>*>& callback)
    {
        delete callback.second;
    });
}

/* STATIC */
JSObject* JSObject::create(Awesomium::WebView* const webView, const he::String& name)
{
    // prevent crashing by retrying
    // wait some time for the awesomium process to
    // finish with the object
    // needed for slow/heavy debug start-up
    #ifdef HE_DEBUG
    uint8 tries(10);
    const size_t waitTime(1000);

    Awesomium::JSObject obj;

    while (tries > 0)
    {
        Awesomium::JSValue val(webView->CreateGlobalJavascriptObject(Awesomium::WSLit(name.c_str())));

        if (val.IsObject())
        {
            obj = val.ToObject();

            Awesomium::Error error(obj.last_error());
            if (error != Awesomium::kError_None)
            {
                const char* errorString("Unknown");
                switch (error)
                {
                case Awesomium::kError_BadParameters: errorString = "BadParameters"; break; 
                case Awesomium::kError_ObjectGone: errorString = "ObjectGone"; break;     
                case Awesomium::kError_ConnectionGone: errorString = "ConnectionGone"; break; 
                case Awesomium::kError_TimedOut: errorString = "TimedOut"; break;       
                case Awesomium::kError_WebViewGone: errorString = "WebViewGone"; break;    
                case Awesomium::kError_Generic: errorString = "Generic"; break; 
                }

                HE_ERROR("Awesomium error: %s, waiting 1s", errorString);
                he::Thread::sleep(waitTime);
                --tries;
            }
            else
                break;
        }
        else
        {
            HE_WARNING("WebListener: Waiting 1s for JS Object creation");
            he::Thread::sleep(waitTime);
            --tries;
        }
    }

    HE_ASSERT(tries > 0, "JSObject creation timed out!");

    #else
    Awesomium::JSValue val(webView->CreateGlobalJavascriptObject(Awesomium::WSLit(name.c_str())));

    HE_ASSERT(val.IsObject(), "object: %s, is not a javascript object!", object.c_str());

    Awesomium::JSObject& obj(val.ToObject());
    #endif

    return NEW JSObject(obj, name);
}

/* GENERAL */
void JSObject::addCallback(const Awesomium::WebString& method, eventCallback1<void, const Awesomium::JSArray&>& callback)
{
    Container::const_iterator it(m_MethodCallBacks.find(method));
    event1<void, const Awesomium::JSArray&>* event(nullptr);

    if (it == m_MethodCallBacks.cend())
    {
        event = NEW event1<void, const Awesomium::JSArray&>();
        m_MethodCallBacks[method] = event;
    }
    else
    {
        event = it->second;
    }

    event->add(callback);

    m_AweObject.SetCustomMethod(method, false);
}

void JSObject::removeCallback(const Awesomium::WebString& method, eventCallback1<void, const Awesomium::JSArray&>& callback)
{
    Container::const_iterator it(m_MethodCallBacks.find(method));
    HE_IF_ASSERT(it != m_MethodCallBacks.cend(), "Removing callback from non existing method")
        m_MethodCallBacks[method]->remove(callback);
}

void JSObject::executeCallback(const Awesomium::WebString& method, const Awesomium::JSArray& args)
{
    HE_IF_ASSERT(m_MethodCallBacks.find(method) != m_MethodCallBacks.cend(), "Executing non existing method")
    {
        m_MethodCallBacks[method]->execute(args);
    }
}

void JSObject::executeFunction(const Awesomium::WebString& method, const Awesomium::JSArray& args)
{
    m_AweObject.Invoke(method, args);
}

} } //end namespace

#endif

