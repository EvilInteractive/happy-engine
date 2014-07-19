#include "HappySandBoxPCH.h"
#include "EntityPropertiesForm.h"
#include "ui_EntityPropertiesForm.h"

#include "EntityPropertyList.h"

#include "Sandbox.h"
#include "system/GameStateMachine.h"
#include "system/SelectionManager.h"
#include "system/EntityManager.h"

#include <EntityManager.h>
#include <Entity.h>
#include <EntityComponent.h>
#include <EntityComponentDesc.h>
#include <Property.h>
#include <PropertyFeel.h>
#include <PropertyConverter.h>

EntityPropertiesForm::EntityPropertiesForm(QWidget *parent) :
    QWidget(parent),
    m_UI(NEW Ui::EntityPropertiesForm),
    m_GameStateChangedCallback(std::bind(&EntityPropertiesForm::onGameStateChanged, this, std::placeholders::_1, std::placeholders::_2)),
    m_SelectionChangedCallback(std::bind(&EntityPropertiesForm::onSelectionChanged, this))
{
    m_UI->setupUi(this);

    hs::GameStateMachine::getInstance()->GameStateChanged += m_GameStateChangedCallback;
}

EntityPropertiesForm::~EntityPropertiesForm()
{
    delete m_UI;
    hs::GameStateMachine::getInstance()->GameStateChanged -= m_GameStateChangedCallback;
}

// Panel methods

namespace
{
    typedef he::ObjectList<he::FixedString> TComponentList;
    void fillPropertyList(const hs::TSelectionSet& selection, TComponentList& outList)
    {
        // Does not work recursive!
        // Entities in entities are not shown
        he::ge::EntityManager* const entityManager(he::ge::EntityManager::getInstance());
        selection.forEach([&outList, entityManager](const he::ObjectHandle handle)
        {
            he::ge::Entity* entity(entityManager->getEntity(handle));
            if (entity)
            {
                const size_t compCount(entity->getComponentCount());
                for (size_t i(0); i < compCount; ++i)
                {
                    const he::FixedString& type(entity->getComponentAt(i)->getComponentID());
                    if (!outList.contains(type))
                        outList.add(type);
                }
            }
        });
    }

    hs::EntityPropertyList* openComponent(Ui::EntityPropertiesForm* container, const he::FixedString& id, const he::String& name)
    {
        hs::EntityPropertyList* list(NEW hs::EntityPropertyList(id, container->m_ComponentPanel));
        container->m_ComponentPanel->addItem(list, QString(name.c_str()));
        return list;
    }

    void closeComponent(hs::EntityPropertyList* /*list*/)
    {

    }

    void addProperty(hs::EntityPropertyList* list, const he::ge::PropertyDesc& prop)
    {
        list->addProperty(prop);
    }

    void setProperty(hs::EntityPropertyList* list, const he::ge::PropertyDesc& prop, const bool mixed)
    {
        if (mixed)
        {
            list->setValueMixed(prop.m_Property->getName());
        }
        else
        {
            list->setValue(prop.m_Property->getName(), prop.m_Converter->toString(prop.m_Property));
        }
    }

}

void EntityPropertiesForm::addComponent(const he::FixedString& id, const hs::TSelectionSet& selection)
{
    hs::EntityManager* entityMan(hs::Sandbox::getInstance()->getEntityManager());
    he::ge::EntityComponentDesc* desc(entityMan->getComponentDescriptor(id));
    if (desc)
    {
        hs::EntityPropertyList* list(openComponent(m_UI, id, desc->m_DisplayName));
        desc->m_Properties.forEach([list, &selection, &id](const he::FixedString& /*propID*/, const he::ge::PropertyDesc& propDesc)
        {
            addProperty(list, propDesc);

            bool hasResult(false);
            bool isMixed(false);
            he::ge::EntityManager* const entityManager(he::ge::EntityManager::getInstance());
            const size_t selectionSize(selection.size());

            he::ge::Property* prop(propDesc.m_Property);
            he::ge::Property* tempProp(prop->Clone());

            for (size_t i(0); i < selectionSize && !isMixed; ++i)
            {
                he::ge::Entity* entity(entityManager->getEntity(selection[i]));
                if (entity)
                {
                    he::ge::EntityComponent* const comp(entity->getComponent(id));
                    if (comp != nullptr && comp->getProperty(prop))
                    {
                        if (hasResult)
                        {
                            isMixed = !tempProp->Equals(prop);
                        }
                        else
                        {
                            hasResult = true;
                        }
                        std::swap(tempProp, prop);
                    }
                }
            }

            propDesc.m_Property != tempProp? delete tempProp : delete prop;

            ::setProperty(list, propDesc, isMixed);
        });

        he::eventCallback2<void, const he::FixedString&, he::ge::Property*> valueChangedCallback(
        [this](const he::FixedString& compId, he::ge::Property* prop)
        { 
            he::ge::EntityManager* const entityManager(he::ge::EntityManager::getInstance());
            const hs::TSelectionSet& selection(hs::SelectionManger::getInstance()->getSelection());
            selection.forEach([&compId, prop, entityManager](const he::ObjectHandle handle)
            {
                he::ge::Entity* entity(entityManager->getEntity(handle));
                if (entity)
                {
                    he::ge::EntityComponent* const comp(entity->getComponent(compId));
                    if (comp)
                    {
                        comp->setProperty(prop);
                    }
                }
            });
        });
        list->ValueChanged += valueChangedCallback;

        closeComponent(list);
    }
}

void EntityPropertiesForm::clearPanel()
{
    while (m_UI->m_ComponentPanel->count() > 0)
    {
        QWidget* comp(m_UI->m_ComponentPanel->widget(0));
        m_UI->m_ComponentPanel->removeItem(0);
        delete comp;
    }
}

// event handling
void EntityPropertiesForm::onGameStateChanged(const hs::EGameState from, const hs::EGameState to)
{
    if (from == hs::eGameState_Edit)
    {
        hs::SelectionManger::getInstance()->SelectionChanged -= m_SelectionChangedCallback;
    }
    else if (to == hs::eGameState_Edit)
    {
        hs::SelectionManger::getInstance()->SelectionChanged += m_SelectionChangedCallback;
    }
}

void EntityPropertiesForm::onSelectionChanged()
{
    clearPanel();
    const hs::TSelectionSet& selection(hs::SelectionManger::getInstance()->getSelection());
    if (selection.size() > 0)
    {
        TComponentList list;
        fillPropertyList(selection, list);
        list.forEach([this, &selection](const he::FixedString& id)
        {
            addComponent(id, selection);
        });

        he::String name;
        he::ge::EntityManager* const entityManager(he::ge::EntityManager::getInstance());
        selection.forEach([entityManager, &name](const he::ObjectHandle handle)
        {
            he::ge::Entity* entity(entityManager->getEntity(handle));
            if (entity)
            {
                if (name.empty() == false)
                {
                    name += ", ";
                }
                name += entity->getName();
            }
        });
        m_UI->m_Name->setText(name.c_str());
    }
    else
    {
        m_UI->m_Name->setText("");
    }
}
