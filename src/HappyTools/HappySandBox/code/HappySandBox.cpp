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


#include "HappySandBox.h"
#include "Qt/qsizegrip.h"

HappySandBox::HappySandBox(QWidget *parent, Qt::WFlags flags)
    :   QMainWindow(parent, flags),
        m_pEntityManager(NEW happysandbox::EntityManager()),
        m_pGame(nullptr)
{
    ui.setupUi(this);

    m_pContentBrowser = NEW happysandbox::ContentBrowser(parent, flags);
    m_pContentBrowser->resize(800,600);
    m_pContentBrowser->setWindowTitle(QString("HappySandBox - Content Browser"));
    m_pContentBrowser->setWorld(m_pEntityManager);

    ui.menuAdd_component->setDisabled(true);

    m_pGame = ui.gameWidget;
}

HappySandBox::~HappySandBox()
{
    delete m_pContentBrowser;
    delete m_pEntityManager;
}

he::gfx::HappyQtWidget* HappySandBox::getGameWidget()
{
    return ui.gameWidget; //->findChild<QGridLayout*>("gridLayout")->findChild<he::gfx::HappyQtWidget*>("gameWidget");
}

void HappySandBox::openContentBrowser()
{
    m_pContentBrowser->show();
}