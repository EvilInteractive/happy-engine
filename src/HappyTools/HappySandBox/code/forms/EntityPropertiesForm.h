#ifndef ENTITYPROPERTIESFORM_H
#define ENTITYPROPERTIESFORM_H

#include <QWidget>
#include "system/GameState.h"

#include "system/SandboxTypes.h"

namespace Ui {
class EntityPropertiesForm;
}

class EntityPropertiesForm : public QWidget
{
    Q_OBJECT

public:
    explicit EntityPropertiesForm(QWidget *parent = 0);
    ~EntityPropertiesForm();

private:
    void clearPanel();
    void addComponent(const he::FixedString& id, const hs::TSelectionSet& selection);

    // Events
    void onGameStateChanged(const hs::EGameState from, const hs::EGameState to);
    void onSelectionChanged();

    he::eventCallback2<void, hs::EGameState, hs::EGameState> m_GameStateChangedCallback;
    he::eventCallback0<void> m_SelectionChangedCallback;
    Ui::EntityPropertiesForm* m_UI;
};

#endif // ENTITYPROPERTIESFORM_H
