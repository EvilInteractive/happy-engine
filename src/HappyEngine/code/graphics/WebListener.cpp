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
//Author:  Sebastiaan Sprengers
//Created: 10/07/2012

#include "HappyPCH.h" 

#ifdef USE_WEB
#include "WebListener.h"
#include "WebView.h"
#include "Awesomium/JSValue.h"
#include "Awesomium/WebView.h"
#pragma warning(disable:4267) // conversion warning
#include "Awesomium/STLHelpers.h"
#pragma warning(default:4267)

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
WebListener::WebListener(Awesomium::WebView* const webView)
    : m_WebView(webView)
{
}

WebListener::~WebListener()
{
    m_WebView = nullptr;

    std::for_each(m_Objects.cbegin(), m_Objects.cend(), [](JSObject* obj)
    {
        delete obj;
    });
}

/* GENERAL */
void WebListener::addObjectCallback(const he::String& object,
                                    const he::String& method,
                                    eventCallback1<void, const Awesomium::JSArray&>& callBack)
{
    // check if jsobject already exists
    auto it(std::find_if(m_Objects.cbegin(), m_Objects.cend(), [&object](JSObject* obj)
    {
        return obj->getObjectName() == object;
    }));
        
    bool objectExists(it != m_Objects.cend());

    // create new js object if it doesn't already exists
    if (objectExists == false)
    {
        JSObject* jsObject(JSObject::create(m_WebView, object));
        Awesomium::WebString aweMethod(Awesomium::WSLit(method.c_str()));

        jsObject->addCallback(aweMethod, callBack);

        m_Objects.add(jsObject);
    }
    else
    {
        Awesomium::WebString aweMethod(Awesomium::WSLit(method.c_str()));

        (*it)->addCallback(aweMethod, callBack);
    };
}
void WebListener::removeObjectCallback(const he::String& object,
                            const he::String& method,
                            eventCallback1<void, const Awesomium::JSArray&>& callBack)
{
    // check if jsobject already exists
    auto it(std::find_if(m_Objects.begin(), m_Objects.end(), [&object](JSObject* obj)
    {
        return obj->getObjectName() == object;
    }));

    HE_IF_ASSERT(it != m_Objects.end(), "object ('%s') not found to remove callback from", object.c_str())
    {
        Awesomium::WebString aweMethod = Awesomium::WSLit(method.c_str());
        (*it)->removeCallback(aweMethod, callBack);
    }
}

void WebListener::executeFunction(const he::String& object,
                                    const he::String& method,
                                    const Awesomium::JSArray& args)
{
    // global js object for global functions
    he::String objName("window");
    Awesomium::WebString aweMethod = Awesomium::WSLit(method.c_str());

    if (object != "window" && object != "")
    {
        objName = object;

        // check if jsobject already exists
        auto it(std::find_if(m_Objects.cbegin(), m_Objects.cend(), [&objName](JSObject* obj)
        {
            return obj->getObjectName() == objName;
        }));
        
        bool objectExists(it != m_Objects.cend());

        // create new js object if it doesn't already exists
        if (objectExists == false)
        {
            Awesomium::JSValue val = m_WebView->CreateGlobalJavascriptObject(
                Awesomium::WSLit(objName.c_str()));

            Awesomium::JSObject& obj = val.ToObject();

            JSObject* jsObject(NEW JSObject(obj, objName));

            m_Objects.add(jsObject);

            jsObject->executeFunction(aweMethod, args);
        }
        else
        {
            (*it)->executeFunction(aweMethod, args);
        };
    }
    else
    {
        Awesomium::JSValue window(
            m_WebView->ExecuteJavascriptWithResult(
            Awesomium::WSLit("window"), Awesomium::WSLit("")));

        Awesomium::JSObject& obj = window.ToObject();

        obj.Invoke(aweMethod, args);
    }
}

void WebListener::OnMethodCall(Awesomium::WebView* caller,
                               unsigned int remote_object_id,
                               const Awesomium::WebString& method_name,
                               const Awesomium::JSArray& args)
{
    if (caller == m_WebView)
    {
        // call correct callback on method of jsobject
        m_Objects.forEach([&](JSObject* jsObject)
        {
            if (jsObject->getAweObject().remote_id() == remote_object_id)
            {
                jsObject->executeCallback(method_name, args);
            }            
        });
    }
}

Awesomium::JSValue WebListener::OnMethodCallWithReturnValue(Awesomium::WebView* /*caller*/,
                                                            unsigned int /*remote_object_id*/,
                                                            const Awesomium::WebString& /*method_name*/,
                                                            const Awesomium::JSArray& /*args*/)
{
    return Awesomium::JSValue(false);
}

} } //end namespace

#endif
