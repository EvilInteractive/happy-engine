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

#ifndef _HT_EntityInteractionSet_H_
#define _HT_EntityInteractionSet_H_
#pragma once

#include "IInteractionSet.h"

#include <QObject>

class QToolBar;
class QAction;

namespace hs {

class IInteractionMode;

class EntityInteractionSet : public QObject, public IInteractionSet
{
    Q_OBJECT

    enum EInteraction
    {
        eInteraction_None = -1,
        eInteraction_Translate,
        eInteraction_Rotate,
        eInteraction_Scale,

        eInteraction_MAX
    };

public:
    EntityInteractionSet();
    ~EntityInteractionSet();

    void init(InteractionManager* const interactionManager) final;

    void activate() final;
    void deactivate() final;

private slots:
    void translateTriggered();
    void rotateTriggered();
    void scaleTriggered();

private:
    void doStateChange(const EInteraction newState);

    QToolBar* m_ToolBar;
    QAction* m_TranslateAction;
    QAction* m_RotateAction;
    QAction* m_ScaleAction;
    
    InteractionManager* m_InteractionManager;
    EInteraction m_ActiveInteraction;

    IInteractionMode* m_InteractionModes[eInteraction_MAX];

    /* DEFAULT COPY & ASSIGNMENT */
    EntityInteractionSet(const EntityInteractionSet&);
    EntityInteractionSet& operator=(const EntityInteractionSet&);
};

} //end namespace

#endif
