#include "HappySandBoxPCH.h" 

#include "GameStateEdit.h"
#include "Sandbox.h"
#include "FlyCamera.h"
#include "system/UIManager.h"
#include "system/EditorPickingManager.h"
#include "system/SelectionManager.h"

#include <IPlugin.h>
#include <View.h>
#include <ControlsManager.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <PickResult.h>
#include <PickingComponent.h>
#include <Ray.h>

namespace hs {

GameStateEdit::GameStateEdit()
    : m_EditorCamera(NEW FlyCamera())
{
    m_EditorCamera->setLens();
    m_EditorCamera->setNearFarPlane(1.0f, 500.0f);
    m_EditorCamera->lookAt(he::vec3::zero, he::vec3::forward, he::vec3::up);
}

GameStateEdit::~GameStateEdit()
{
    delete m_EditorCamera;
}

bool GameStateEdit::enter()
{
    Sandbox* const sandBox(Sandbox::getInstance());
    he::pl::IPlugin* const plugin(sandBox->getGamePlugin());
    if (plugin != nullptr)
    {
        plugin->onPauseGame();
        he::gfx::ICamera* const gameCam(plugin->getActiveCamera());
        m_EditorCamera->lookAt(gameCam->getPosition(), gameCam->getPosition() + gameCam->getLook(), he::vec3::up);
        plugin->setActiveCamera(m_EditorCamera);
    }
    sandBox->getMainView()->setCamera(m_EditorCamera);

    UIManager::getInstance()->showEditorUI();

    return true;
}

void GameStateEdit::exit()
{
}

void GameStateEdit::tick( const float /*dTime*/ )
{
    he::io::ControlsManager* const controls(CONTROLS);
    he::io::IMouse* const mouse(controls->getMouse());
    he::io::IKeyboard* const keyboard(controls->getKeyboard());

    Sandbox* const sandbox(Sandbox::getInstance());
    he::pl::IPlugin* const plugin(sandbox->getGamePlugin());
    if (plugin != nullptr)
    {
        if (mouse->isButtonPressed(he::io::MouseButton_Left))
        {        
            if (keyboard->isKeyDown(he::io::Key_Ctrl) == false && keyboard->isKeyDown(he::io::Key_Alt) == false)
                SelectionManger::getInstance()->deselectAll();
            he::Ray ray(sandbox->getMainView(), m_EditorCamera, mouse->getPosition());
            he::ge::PickResult result;
            if (EditorPickingManager::getInstance()->pick(ray, result))
            {
                he::ge::PickingComponent* const comp(checked_cast<he::ge::PickingComponent*>(result.getObject()));
                if (keyboard->isKeyDown(he::io::Key_Alt) == false)
                    SelectionManger::getInstance()->select(comp->getEntityParent());
                else
                    SelectionManger::getInstance()->deselect(comp->getEntityParent());
            }
        }
    }
}

} //end namespace
