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
//
//Author:  Sebastiaan Sprengers
//Created: 11/01/2012

#ifndef _CONTENT_BROWSER_H_
#define _CONTENT_BROWSER_H_
#pragma once

#pragma warning(disable:4127)
#include <QtGui/QDialog>
#include <QtGui/QKeyEvent>
#include "ui_ContentBrowser.h"
#pragma warning(default:4127)

#include <map>
#include "Texture2D.h"
#include "Model.h"
#include "Material.h"
#include "EntityManager.h"

namespace happysandbox {

class ContentBrowser : public QWidget
{
    Q_OBJECT

public:

    /* CONSTRUCTOR - DESTRUCTOR */
    ContentBrowser(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~ContentBrowser();

    /* GENERAL */
    void keyPressEvent(QKeyEvent* event);

    /* TEMP */
    void setWorld(EntityManager* pWorld);

private slots:

    void importFile();
    void itemClicked(QTreeWidgetItem* item, int c);
    void itemDoubleClicked(QTreeWidgetItem* item, int c);
    void deleteItem();
    void addToScene();

private:

    uint m_NrItems;

    Ui::ContentBrowser ui;

    std::vector<QTreeWidgetItem*> m_ContentItems;

    QTreeWidgetItem* m_SelectedItem;

    bool m_bEdit;
    bool m_bOnce;

    std::map<uint, he::gfx::Texture2D::pointer> m_Textures;
    std::map<uint, he::gfx::Model::pointer> m_Models;

    he::gfx::Material m_DefaultMaterial;

    EntityManager* m_pWorld;
};

} //end namespace

#endif
