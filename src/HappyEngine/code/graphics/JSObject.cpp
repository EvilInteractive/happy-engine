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

#include "JSObject.h"

namespace he {
namespace gui {

JSObject::JSObject(Awesomium::JSObject& aweObject, const std::string& name) : m_AweObject(aweObject),
                                                                              m_ObjectName(name)
{
}

JSObject::~JSObject()
{
    std::for_each(m_MethodCallBacks.cbegin(), m_MethodCallBacks.cend(), [](const std::pair<Awesomium::WebString,
                                                                           event1<void, const Awesomium::JSArray&>*>& callback)
    {
        delete callback.second;
    });
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
