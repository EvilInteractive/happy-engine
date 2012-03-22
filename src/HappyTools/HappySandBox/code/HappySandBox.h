//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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


#ifndef HAPPYSANDBOX_H
#define HAPPYSANDBOX_H

#include "HappyQtWidget.h"
#pragma warning(disable:4127)
#include <QtGui/QMainWindow>
#include "ui_HappySandBox.h"
#pragma warning(default:4127)

#include "ContentBrowser.h"
#include "EntityManager.h"

class HappySandBox : public QMainWindow
{
    Q_OBJECT

public:
    HappySandBox(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~HappySandBox();

    he::gfx::HappyQtWidget* getGameWidget();

private slots:

    void openContentBrowser();

private:

    Ui::HappySandBoxClass ui;

    happysandbox::ContentBrowser* m_pContentBrowser;

    happysandbox::EntityManager* m_pEntityManager;

    he::gfx::HappyQtWidget* m_pGame;
};

#endif // HAPPYSANDBOX_H
