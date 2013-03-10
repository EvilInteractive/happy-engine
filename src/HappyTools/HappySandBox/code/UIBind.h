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
//Created: 13/07/2012

#ifndef _HS_UI_BIND_H_
#define _HS_UI_BIND_H_
#pragma once

#include "Awesomium/JSObject.h"
#include "Awesomium/JSValue.h"
#include "event.h"

namespace he{
    namespace gui {
        class WebListener;
    }
}

namespace hs {
    class UIController;
    class EntityManager;

class UIBind
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    UIBind(UIController* uiController, EntityManager* entityManager);
    virtual ~UIBind();

    /* GENERAL */
    void bindObjectMethodToCallback(const he::String& object,
                                    const he::String& method,
                                    he::eventCallback1<void, const Awesomium::JSArray&>& callBack);
    void unbindObjectMethodToCallback(const he::String& object,
                                      const he::String& method,
                                      he::eventCallback1<void, const Awesomium::JSArray&>& callBack);

    void setup();

    /* HANDLERS */
    void menuCreateEntityHandler(const Awesomium::JSArray& /*args*/);

private:

    /* DATAMEMBERS */
    UIController* m_UIController;
    he::gui::WebListener* m_WebListener;
    EntityManager* m_EntityManager;

    /* DEFAULT COPY & ASSIGNMENT */
    UIBind(const UIBind&);
    UIBind& operator=(const UIBind&);
};

} //end namespace

#endif
