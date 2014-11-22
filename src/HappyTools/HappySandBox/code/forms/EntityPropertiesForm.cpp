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
    m_UI->m_EntityProperties->setComponentType(he::HEFS::strEntity);

    clearPanel();

    hs::GameStateMachine::getInstance()->GameStateChanged += m_GameStateChangedCallback;
}

EntityPropertiesForm::~EntityPropertiesForm()
{
    delete m_UI;
    hs::GameStateMachine* gm(hs::GameStateMachine::getInstance());
    if (gm) // @exit it is already gone
        gm->GameStateChanged -= m_GameStateChangedCallback;
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

    hs::EntityPropertyList* createPropertyContainer(Ui::EntityPropertiesForm* container, const he::FixedString& id, const he::String& name)
    {
        hs::EntityPropertyList* list(nullptr);
        if (id == he::HEFS::strEntity)
        {
            list = container->m_EntityProperties;
        }
        else
        {
            list = NEW hs::EntityPropertyList(container->m_ComponentPanel);
            list->setComponentType(id);
            container->m_ComponentPanel->addItem(list, QString(name.c_str()));
        }
        return list;
    }
    
    void addProperty(hs::PropertyList* list, const he::ge::PropertyDesc& prop)
    {
        list->addProperty(prop);
    }

    void setProperty(hs::PropertyList* list, const he::ge::PropertyDesc& prop, const bool mixed)
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

void EntityPropertiesForm::createComponentProperties(const he::FixedString& id, const hs::TSelectionSet& selection)
{
    hs::EntityManager* entityMan(hs::Sandbox::getInstance()->getEntityManager());
    he::ge::EntityComponentDesc* desc(entityMan->getComponentDescriptor(id));
    if (desc)
    {
        hs::EntityPropertyList* list(createPropertyContainer(m_UI, id, desc->m_DisplayName));
        desc->m_Properties.forEach([list, &selection, &id](const he::FixedString& /*propID*/, const he::ge::PropertyDesc& propDesc)
        {
            // Add Property to list
            addProperty(list, propDesc);

            // Find if the property has mixed values, or get the value and fill it in
            bool hasResult(false);
            bool isMixed(false);
            he::ge::EntityManager* const entityManager(he::ge::EntityManager::getInstance());
            const size_t selectionSize(selection.size());

            he::ge::Property* prop(propDesc.m_Property);
            he::ge::Property* tempProp(prop->clone());

            for (size_t i(0); i < selectionSize && !isMixed; ++i)
            {
                he::ge::Entity* entity(entityManager->getEntity(selection[i]));
                if (entity)
                {
                    he::ge::EntityComponent* const comp(id == he::HEFS::strEntity? entity : entity->getComponent(id));
                    if (comp != nullptr && comp->getProperty(prop))
                    {
                        if (hasResult)
                        {
                            isMixed = !tempProp->equals(prop);
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

            // Set the property value
            ::setProperty(list, propDesc, isMixed);
        });

        // Add event handler is the propery is edited
        he::eventCallback1<void, he::ge::Property*> valueChangedCallback(
        [this, list](he::ge::Property* prop)
        { 
            const he::FixedString& compID(list->getComponentType());
            he::ge::EntityManager* const entityManager(he::ge::EntityManager::getInstance());
            const hs::TSelectionSet& selection(hs::SelectionManger::getInstance()->getSelection());
            selection.forEach([&compID, prop, entityManager](const he::ObjectHandle handle)
            {
                he::ge::Entity* entity(entityManager->getEntity(handle));
                if (entity)
                {
                    he::ge::EntityComponent* const comp(compID == he::HEFS::strEntity? entity : entity->getComponent(compID));
                    if (comp)
                    {
                        comp->setProperty(prop);
                    }
                }
            });
        });
        list->ValueChanged += valueChangedCallback;
    }
}

void EntityPropertiesForm::clearPanel()
{
    m_UI->m_EntityProperties->clear();
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
        // Fill entity properties
        createComponentProperties(he::HEFS::strEntity, selection);

        // Fill component properties
        TComponentList list;
        fillPropertyList(selection, list);
        list.forEach([this, &selection](const he::FixedString& id)
        {
            createComponentProperties(id, selection);
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
