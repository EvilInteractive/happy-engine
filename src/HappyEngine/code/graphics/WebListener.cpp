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

#include "WebListener.h"
#include "WebView.h"
#include "Awesomium/JSValue.h"
#include "Awesomium/WebView.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
WebListener::WebListener() : m_WebView(nullptr)
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
void WebListener::attachToView(WebView* view)
{
    view->getAWEView()->set_js_method_handler(this);
    m_WebView = view;
}
void WebListener::addObjectCallback(const std::string& object,
                                    const std::string& method,
                                    eventCallback0<void>& callBack)
{
    // check if jsobject already exists
    bool objectExists(std::find_if(m_Objects.cbegin(), m_Objects.cend(), [&object](JSObject* obj)
    {
        return obj->getObjectName() == object;
    }) != m_Objects.cend());

    // create new js object if it doesn't already exists
    if (objectExists == false)
    {
        Awesomium::JSValue val = m_WebView->getAWEView()->CreateGlobalJavascriptObject(
            Awesomium::WebString::CreateFromUTF8(object.c_str(), strlen(object.c_str())));

        Awesomium::WebString aweMethod = Awesomium::WebString::CreateFromUTF8(method.c_str(), strlen(method.c_str()));

        Awesomium::JSObject& obj = val.ToObject();

        JSObject* jsObject(NEW JSObject(obj, object));
        jsObject->addCallback(aweMethod, callBack);

        m_Objects.push_back(jsObject);
    }

    // TODO: seeb i don't think this line is correct, why always the last in the list? what if objectExists == true
    // set method on object
    m_Objects.back()->getAweObject().SetCustomMethod(
        Awesomium::WebString::CreateFromUTF8(method.c_str(), strlen(method.c_str())), false);
}
void WebListener::removeObjectCallback(const std::string& object,
    const std::string& method,
    const eventCallback0<void>& callBack)
{
    // check if jsobject already exists
    std::vector<JSObject*>::iterator it(std::find_if(m_Objects.begin(), m_Objects.end(), [&object](JSObject* obj)
    {
        return obj->getObjectName() == object;
    }));

    HE_IF_ASSERT(it != m_Objects.end(), "object ('%s') not found to remove callback from", object.c_str())
    {
        Awesomium::WebString aweMethod = Awesomium::WebString::CreateFromUTF8(method.c_str(), strlen(method.c_str()));
        (*it)->removeCallback(aweMethod, callBack);
    }
}

void WebListener::OnMethodCall(Awesomium::WebView* caller,
                               unsigned int remote_object_id,
                               const Awesomium::WebString& method_name,
                               const Awesomium::JSArray& /*args*/)
{
    if (caller == m_WebView->getAWEView())
    {
        // call correct callback on method of jsobject
        std::for_each(m_Objects.cbegin(), m_Objects.cend(), [&](JSObject* jsObject)
        {
            if (jsObject->getAweObject().remote_id() == remote_object_id)
            {
                jsObject->executeCallback(method_name);
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
