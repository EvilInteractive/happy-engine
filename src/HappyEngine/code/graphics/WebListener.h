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
    void addObjectCallback(const std::string& object,
                           const std::string& method,
                           eventCallback0<void>& callBack);
    void removeObjectCallback(const std::string& object,
                              const std::string& method,
                              eventCallback0<void>& callBack);

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
    class JSObject
    {
    public:
        /* CONSTRUCTOR */
        JSObject(Awesomium::JSObject& aweObject, const std::string& name) : m_AweObject(aweObject), m_ObjectName(name) {}
        ~JSObject()
        {
            std::for_each(m_MethodCallBacks.cbegin(), m_MethodCallBacks.cend(), [](const std::pair<Awesomium::WebString, event0<void>*>& callback)
            {
                delete callback.second;
            });
        }

        inline const std::string& getObjectName() const { return m_ObjectName; }
        inline Awesomium::JSObject& getAweObject() { return m_AweObject; }

        void addCallback(const Awesomium::WebString& method, eventCallback0<void>& callback)
        {
            Container::const_iterator it(m_MethodCallBacks.find(method));
            event0<void>* event(nullptr);
            if (it == m_MethodCallBacks.cend())
            {
                event = NEW event0<void>();
                m_MethodCallBacks[method] = event;
            }
            else
            {
                event = it->second;
            }
            event->add(callback);
        }
        void removeCallback(const Awesomium::WebString& method, eventCallback0<void>& callback)
        {
            Container::const_iterator it(m_MethodCallBacks.find(method));
            HE_IF_ASSERT(it != m_MethodCallBacks.cend(), "Removing callback from non existing method")
                m_MethodCallBacks[method]->remove(callback);
        }
        void executeCallback(const Awesomium::WebString& method)
        {
            HE_IF_ASSERT(m_MethodCallBacks.find(method) != m_MethodCallBacks.cend(), "Executing non existing method")
                m_MethodCallBacks[method]->execute();
        }

    private:
        std::string m_ObjectName;
        Awesomium::JSObject m_AweObject; // TODO: seeb: I removed the reference here because it looks very dangerous

        typedef std::map<Awesomium::WebString, event0<void>*> Container;
        Container m_MethodCallBacks;

        JSObject(const JSObject&);
        JSObject& operator=(const JSObject&);
    };

    /* DATAMEMBERS */
    WebView* m_WebView;
    std::vector<JSObject*> m_Objects;

    /* DEFAULT COPY & ASSIGNMENT */
    WebListener(const WebListener&);
    WebListener& operator=(const WebListener&);
};

} } //end namespace

#endif
