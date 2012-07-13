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

#ifndef _HE_WEB_LISTENER_H_
#define _HE_WEB_LISTENER_H_
#pragma once

#include "Awesomium/JSObject.h"
#include "Awesomium/JSValue.h"
#include "event.h"

namespace he {
namespace gfx {
    class WebView;

class WebListener : public Awesomium::JSMethodHandler
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    WebListener();
    virtual ~WebListener();

    /* GENERAL */
    void attachToView(WebView* view);
    void setObjectCallback(const std::string& object,
                           const std::string& method,
                           boost::function<void()> callBack);

    virtual void OnMethodCall(Awesomium::WebView* caller,
                              unsigned int remote_object_id,
                              const Awesomium::WebString& method_name,
                              const Awesomium::JSArray& args);
    virtual Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView* caller,
                                                           unsigned int remote_object_id,
                                                           const Awesomium::WebString& method_name,
                                                           const Awesomium::JSArray& args);

private:

    /* EXTRA */
    struct JSObject
    {
        /* CONSTRUCTOR */
        JSObject(Awesomium::JSObject& aweObject) : aweObject(aweObject){}

        std::string objectName;
        Awesomium::JSObject& aweObject;
        std::map<Awesomium::WebString, event0<void> > methodCallBacks;

        /* COPY */
        JSObject& operator=(const JSObject& jsObject)
        {
            objectName = jsObject.objectName;
            aweObject = jsObject.aweObject;
            methodCallBacks = jsObject.methodCallBacks;
        }
    };

    /* DATAMEMBERS */
    WebView* m_WebView;
    std::vector<JSObject> m_Objects;

    /* DEFAULT COPY & ASSIGNMENT */
    WebListener(const WebListener&);
    WebListener& operator=(const WebListener&);
};

} } //end namespace

#endif
