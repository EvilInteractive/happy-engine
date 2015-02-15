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
//Author:  Bastian Damman
//Created: 2015/02/10
#include "HappySandBoxPCH.h"
#include "EntityInteractionSet.h"

#include "Sandbox.h"
#include "InteractionManager.h"
#include "forms/MainWindow.h"

#include "EntityMoveInteractionMode.h"
#include "EntityRotateInteractionMode.h"
#include "EntityScaleInteractionMode.h"

#include <QToolBar>
#include <QAction>
#include <QApplication>

namespace hs {

EntityInteractionSet::EntityInteractionSet()
    : QObject()
    , m_ActiveInteraction(eInteraction_None)
{
    m_TranslateAction = HENew(QAction)(this);
    m_TranslateAction->setObjectName(QStringLiteral("actionActivateTranslateEntity"));
    QIcon translateIcon;
    translateIcon.addFile(QStringLiteral(":/icons/interactions/resources/translate.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_TranslateAction->setIcon(translateIcon);
    m_TranslateAction->setCheckable(true);
    m_TranslateAction->setText(QApplication::translate("MainWindow", "Translate", 0));
    m_TranslateAction->setShortcut(Qt::Key_1);
    connect(m_TranslateAction, SIGNAL(triggered(bool)), this, SLOT(translateTriggered()));

    m_RotateAction = HENew(QAction)(this);
    m_RotateAction->setObjectName(QStringLiteral("actionActivateRotateEntity"));
    QIcon rotateIcon;
    rotateIcon.addFile(QStringLiteral(":/icons/interactions/resources/rotate.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_RotateAction->setIcon(rotateIcon);
    m_RotateAction->setCheckable(true);
    m_RotateAction->setText(QApplication::translate("MainWindow", "Rotate", 0));
    m_RotateAction->setShortcut(Qt::Key_2);
    connect(m_RotateAction, SIGNAL(triggered(bool)), this, SLOT(rotateTriggered()));

    m_ScaleAction = HENew(QAction)(this);
    m_ScaleAction->setObjectName(QStringLiteral("actionActivateScaleEntity"));
    QIcon scaleIcon;
    scaleIcon.addFile(QStringLiteral(":/icons/interactions/resources/scale.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_ScaleAction->setIcon(scaleIcon);
    m_ScaleAction->setCheckable(true);
    m_ScaleAction->setText(QApplication::translate("MainWindow", "Scale", 0));
    m_ScaleAction->setShortcut(Qt::Key_3);
    connect(m_ScaleAction, SIGNAL(triggered(bool)), this, SLOT(scaleTriggered()));

    m_ToolBar = HENew(QToolBar)();
    m_ToolBar->setObjectName(QStringLiteral("entityInteractionSet"));
    m_ToolBar->addAction(m_TranslateAction);
    m_ToolBar->addAction(m_RotateAction);
    m_ToolBar->addAction(m_ScaleAction);

    doStateChange(eInteraction_None);

    QMainWindow* mainWindow(Sandbox::getInstance()->getEditorWindow());
    mainWindow->addToolBar(Qt::TopToolBarArea, m_ToolBar);

    m_InteractionModes[eInteraction_Translate] = HENew(EntityMoveInteractionMode)();
    m_InteractionModes[eInteraction_Rotate] = HENew(EntityRotateInteractionMode)();
    m_InteractionModes[eInteraction_Scale] = HENew(EntityScaleInteractionMode)();
}

EntityInteractionSet::~EntityInteractionSet()
{
    deactivate();

    QMainWindow* mainWindow(Sandbox::getInstance()->getEditorWindow());
    mainWindow->removeToolBar(m_ToolBar);

    HEDelete(m_ToolBar);
    HEDelete(m_TranslateAction);
    HEDelete(m_RotateAction);
    HEDelete(m_ScaleAction);

    for (size_t i(0); i < eInteraction_MAX; ++i)
    {
        HEDelete(m_InteractionModes[i]);
    }
}

void EntityInteractionSet::init( InteractionManager* const interactionManager )
{
    m_InteractionManager = interactionManager;
}

void EntityInteractionSet::activate()
{
}

void EntityInteractionSet::deactivate()
{
    doStateChange(eInteraction_None);
}

void EntityInteractionSet::translateTriggered()
{
    doStateChange(eInteraction_Translate);
}

void EntityInteractionSet::rotateTriggered()
{
    doStateChange(eInteraction_Rotate);
}

void EntityInteractionSet::scaleTriggered()
{
    doStateChange(eInteraction_Scale);
}

void EntityInteractionSet::doStateChange( const EInteraction newState )
{
    m_TranslateAction->setChecked(newState == eInteraction_Translate);
    m_RotateAction->setChecked(newState == eInteraction_Rotate);
    m_ScaleAction->setChecked(newState == eInteraction_Scale);
    m_TranslateAction->setEnabled(newState != eInteraction_Translate);
    m_RotateAction->setEnabled(newState != eInteraction_Rotate);
    m_ScaleAction->setEnabled(newState != eInteraction_Scale);
    if (m_ActiveInteraction == eInteraction_None && newState != eInteraction_None)
    {
        m_InteractionManager->requestActivate(this);
    }
    if (m_ActiveInteraction != eInteraction_None)
        m_InteractionModes[m_ActiveInteraction]->deactivate();
    if (newState != eInteraction_None)
        m_InteractionModes[newState]->activate();
    m_ActiveInteraction = newState;
}

} //end namespace