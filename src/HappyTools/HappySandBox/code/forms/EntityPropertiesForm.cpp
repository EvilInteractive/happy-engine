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

EntityPropertiesForm::EntityPropertiesForm(QWidget *parent) :
    QWidget(parent),
    m_UI(NEW Ui::EntityPropertiesForm),
    m_GameStateChangedCallback(std::bind(&EntityPropertiesForm::OnGameStateChanged, this, std::placeholders::_1, std::placeholders::_2)),
    m_SelectionChangedCallback(std::bind(&EntityPropertiesForm::OnSelectionChanged, this))
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
    void FillPropertyList(const hs::SelectionSet& selection, TComponentList& outList)
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

    EntityPropertyList* OpenComponent(Ui::EntityPropertiesForm* container, const he::String& name)
    {
        EntityPropertyList* list(NEW EntityPropertyList(container->m_ComponentPanel));
        container->m_ComponentPanel->addItem(list, QString(name.c_str()));
        return list;
    }

    void CloseComponent(EntityPropertyList* /*list*/)
    {

    }

    void AddProperty(EntityPropertyList* list, const he::ge::PropertyDesc& prop)
    {
        list->AddProperty(prop);
    }

}

void EntityPropertiesForm::AddComponent(const he::FixedString& id)
{
    hs::EntityManager* entityMan(hs::Sandbox::getInstance()->getEntityManager());
    he::ge::EntityComponentDesc* desc(entityMan->getComponentDescriptor(id));
    if (desc)
    {
        EntityPropertyList* list(OpenComponent(m_UI, desc->m_DisplayName));
        desc->m_Properties.forEach([list](const he::ge::PropertyDesc& prop)
        {
            AddProperty(list, prop);
        });
        CloseComponent(list);
    }
}

void EntityPropertiesForm::ClearPanel()
{
    while (m_UI->m_ComponentPanel->count() > 0)
    {
        QWidget* comp(m_UI->m_ComponentPanel->widget(0));
        m_UI->m_ComponentPanel->removeItem(0);
        delete comp;
    }
}

// event handling
void EntityPropertiesForm::OnGameStateChanged(const hs::EGameState from, const hs::EGameState to)
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

void EntityPropertiesForm::OnSelectionChanged()
{
    ClearPanel();
    const hs::SelectionSet& selection(hs::SelectionManger::getInstance()->getSelection());
    if (selection.size() > 0)
    {
        TComponentList list;
        FillPropertyList(selection, list);
        list.forEach([this](const he::FixedString& id){ AddComponent(id); });
    }
}
