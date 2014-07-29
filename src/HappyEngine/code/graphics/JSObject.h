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

#ifndef _HE_JSOBJECT_H_
#define _HE_JSOBJECT_H_
#pragma once
#ifdef USE_WEB
#include "Awesomium/JSObject.h"
#include "Awesomium/JSValue.h"
#include "event.h"

namespace he {
namespace gui {
    class WebView;

class JSObject
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    JSObject(Awesomium::JSObject& aweObject, const he::String& name);
    ~JSObject();

    /* STATIC */
    static JSObject* create(Awesomium::WebView* const webView, const he::String& name);

    /* GENERAL */
    void addCallback(const Awesomium::WebString& method, eventCallback1<void, const Awesomium::JSArray&>& callback);
    void removeCallback(const Awesomium::WebString& method, eventCallback1<void, const Awesomium::JSArray&>& callback);
    void executeCallback(const Awesomium::WebString& method, const Awesomium::JSArray& args);
    void executeFunction(const Awesomium::WebString& method, const Awesomium::JSArray& args);

    /* GETTERS */
    inline const he::String& getObjectName() const { return m_ObjectName; }
    inline Awesomium::JSObject& getAweObject() { return m_AweObject; }

private:

    /* MEMBERS */
    he::String m_ObjectName;
    Awesomium::JSObject m_AweObject;

    typedef std::map<Awesomium::WebString, event1<void, const Awesomium::JSArray&>*> Container;
    Container m_MethodCallBacks;

    /* DEFAULT COPY & ASSIGNMENT */
    JSObject(const JSObject&);
    JSObject& operator=(const JSObject&);
};

} } //end namespace

#endif

#endif
