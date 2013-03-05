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

#include "HappySandBoxPCH.h" 

#include "UIBind.h"
#include "WebListener.h"
#include "UIController.h"
#include "WebView.h"
#include "EntityManager.h"

namespace hs {

/* CONSTRUCTOR - DESTRUCTOR */
UIBind::UIBind(UIController* uiController, EntityManager* entityManager) :
                                            m_UIController(uiController),
                                            m_WebListener(m_UIController->getWebView()->getWebListener()),
                                            m_EntityManager(entityManager)
{
}

UIBind::~UIBind()
{
}

/* GENERAL */
void UIBind::bindObjectMethodToCallback(const std::string& object,
                                        const std::string& method,
                                        he::eventCallback1<void, const Awesomium::JSArray&>& callBack)
{
    m_WebListener->addObjectCallback(object, method, callBack);
}

void UIBind::unbindObjectMethodToCallback(const std::string& object,
                                          const std::string& method,
                                          he::eventCallback1<void, const Awesomium::JSArray&>& callBack)
{
    m_WebListener->removeObjectCallback(object, method, callBack);
}

void UIBind::setup()
{
    he::eventCallback1<void, const Awesomium::JSArray&> menuCreateEntityCallback(boost::bind(&UIBind::menuCreateEntityHandler, this, _1));
    bindObjectMethodToCallback("HE", "menuCreateEntity", menuCreateEntityCallback);
}

/* HANDLERS */
void UIBind::menuCreateEntityHandler(const Awesomium::JSArray& /*args*/)
{
    m_EntityManager->createEntity();
}

} //end namespace