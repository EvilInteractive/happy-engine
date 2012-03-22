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

#include "ContentBrowser.h"
#include "HappyPCH.h"

#pragma warning(disable:4127)
#include <QtGui/QFileDialog>
#include <QtGui/QStandardItem>
#include <QtGui/QMessageBox>
#pragma warning(default:4127)

#include <algorithm>
#include "ContentManager.h"
#include "Material.h"
#include "ModelComponent.h"
#include "Entity.h"

namespace happysandbox {

ContentBrowser::ContentBrowser(QWidget* parent, Qt::WFlags flags) : QWidget (parent, flags),
                                                                    m_SelectedItem(nullptr),
                                                                    m_bEdit(false),
                                                                    m_NrItems(0),
                                                                    m_bOnce(false),
                                                                    m_pWorld(nullptr)
{
    ui.setupUi(this);
}

ContentBrowser::~ContentBrowser()
{
}

/* GENERAL */

void ContentBrowser::keyPressEvent(QKeyEvent* event)
{
    if (m_bEdit && event->key() == Qt::Key_Return)
    {
        ui.contentList->closePersistentEditor(m_SelectedItem, 1);
        m_bEdit = false;
    }
}

void ContentBrowser::importFile()
{
    if (!m_bOnce) // SUCKY - QUICK FIX
    {
        CONTENT->setRootDir("../data/");
        CONTENT->setTextureFolder("textures/");
        m_DefaultMaterial = CONTENT->loadMaterial("editor/default.material");

        m_bOnce = true;
    }

    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"),"",tr("Files (*.*)"));
    
    if (!fileNames.isEmpty())
    {
        CONTENT->setRootDir("");
        CONTENT->setTextureFolder("");
        CONTENT->setModelFolder("");

        for (int i(0); i < fileNames.size(); ++i)
        {
            QString type("Unknown");
            QString name("new");

            he::gfx::Texture2D::pointer tex;
            he::gfx::Model::pointer mod;

            if (fileNames.at(i).contains(".png") || fileNames.at(i).contains(".tga"))
            {
                type = "Texture";
                name += "Texture";
                
                tex = CONTENT->asyncLoadTexture(fileNames.at(i).toStdString());
            }
            else if (fileNames.at(i).contains(".obj") || fileNames.at(i).contains(".binobj"))
            {
                type = "Model";
                name += "Model";

                mod = CONTENT->asyncLoadModel(fileNames.at(i).toStdString(), m_DefaultMaterial.getCompatibleVertexLayout());
            }
            else
            {
                name += "Unknown";
            }

            name += QVariant(m_NrItems).toString();

            QStringList list;
            list.push_back(QVariant(m_NrItems).toString());
            list.push_back(name);
            list.push_back(type);
            list.push_back(fileNames.at(i));
            list.push_back("Loading...");

            m_ContentItems.push_back(NEW QTreeWidgetItem(list));

            ui.contentList->insertTopLevelItem(0, m_ContentItems[m_ContentItems.size() - 1]);

            if (type == "Texture")
            {
                tex->callbackIfLoaded([&](){m_ContentItems[m_ContentItems.size() - 1]->setData(4,0, QVariant("Loaded"));});
                m_Textures[m_NrItems] = tex;
            }
            else if (type == "Model")
            {
                //mod->getMesh(0)->callbackIfLoaded([&](){m_ContentItems[m_ContentItems.size() - 1]->setData(3,0, QVariant("Loaded"));});
                m_ContentItems[m_ContentItems.size() - 1]->setData(4,0, QVariant("-PROBABLY LOADED? !CHECK CONSOLE! NEEDs TO BE FIXED-"));
                m_Models[m_NrItems] = mod;
            }
            else
                m_ContentItems[m_ContentItems.size() - 1]->setData(4,0, QVariant("Can't load unknown format"));

            ++m_NrItems;
        }
    }
}

void ContentBrowser::itemClicked(QTreeWidgetItem* item, int c)
{
    if (item != m_SelectedItem || c != 1)
    {
        if (m_SelectedItem != nullptr && m_bEdit == true)
        {
            ui.contentList->closePersistentEditor(m_SelectedItem,0);
            m_bEdit = false;
        }
    }

    m_SelectedItem = item;
}

void ContentBrowser::itemDoubleClicked(QTreeWidgetItem* item, int c)
{
    if (c == 1)
    {
        ui.contentList->openPersistentEditor(item,1);
        m_bEdit = true;
    }
}

void ContentBrowser::deleteItem()
{
    if (m_SelectedItem != nullptr)
    {
        QString itemName(m_SelectedItem->data(1,0).toString());

        QMessageBox confirm;
        confirm.setWindowTitle("Delete");
        confirm.setText("Are you sure you want to delete the item: '" + itemName + "' ?");
        confirm.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        int pressed(confirm.exec());

        switch (pressed)
        {
            case QMessageBox::Ok:
            {
                m_ContentItems.erase(std::remove(m_ContentItems.begin(), m_ContentItems.end(), m_SelectedItem), m_ContentItems.end());

                delete m_SelectedItem;

                break;
            }
        }

        m_SelectedItem = nullptr;
    }
}

void ContentBrowser::addToScene()
{
    if (m_SelectedItem->data(2,0).toString() == "Model")
    {
        uint itemID(m_SelectedItem->data(0,0).toUInt());
        he::game::Entity* pEntity(NEW he::game::Entity());
        he::game::ModelComponent* modComp(NEW he::game::ModelComponent());
        modComp->setModelMesh(m_Models[itemID]->getMesh(0));
        modComp->setMaterial(m_DefaultMaterial);
        pEntity->addComponent(modComp);

        m_pWorld->addEntity(pEntity);
    }
}

/* TEMP */
void ContentBrowser::setWorld(EntityManager* pWorld)
{
    m_pWorld = pWorld;
}

} //end namespace