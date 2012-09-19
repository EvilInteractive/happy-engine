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

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
WebListener::WebListener() : m_WebView(nullptr)
{
}

WebListener::~WebListener()
{
    m_WebView = nullptr;
}

/* GENERAL */
void WebListener::attachToView(WebView* view)
{
    view->getAWEView()->set_js_method_handler(this);
    m_WebView = view;
}
void WebListener::setObjectCallback(const std::string& object,
                                    const std::string& method,
                                    boost::function<void()> callBack)
{
    bool objectExists(false);

    // check if jsobject already exists
    std::for_each(m_Objects.cbegin(), m_Objects.cend(), [&](JSObject jsObject)
    {
        if (jsObject.objectName == object)
        {
            objectExists = true;
        }
    });

    // create new js object if it doesnt already exists
    if (objectExists == false)
    {
        Awesomium::JSValue val = m_WebView->getAWEView()->CreateGlobalJavascriptObject(
            Awesomium::WebString::CreateFromUTF8(object.c_str(), strlen(object.c_str())));

        Awesomium::WebString aweMethod = Awesomium::WebString::CreateFromUTF8(method.c_str(), strlen(method.c_str()));

        Awesomium::JSObject& obj = val.ToObject();

        JSObject jsObject(obj);
        jsObject.objectName = object;
        jsObject.methodCallBacks[aweMethod] += callBack;

        m_Objects.push_back(jsObject);
    }

    // set method on object
    m_Objects[m_Objects.size() - 1].aweObject.SetCustomMethod(
        Awesomium::WebString::CreateFromUTF8(method.c_str(), strlen(method.c_str())), false);
}

void WebListener::OnMethodCall(Awesomium::WebView* caller,
                               unsigned int remote_object_id,
                               const Awesomium::WebString& method_name,
                               const Awesomium::JSArray& /*args*/)
{
    if (caller == m_WebView->getAWEView())
    {
        // call correct callback on method of jsobject
        std::for_each(m_Objects.cbegin(), m_Objects.cend(), [&](JSObject jsObject)
        {
            if (jsObject.aweObject.remote_id() == remote_object_id)
            {
                jsObject.methodCallBacks[method_name]();
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
