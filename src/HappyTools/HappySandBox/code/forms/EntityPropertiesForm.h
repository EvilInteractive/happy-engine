#ifndef ENTITYPROPERTIESFORM_H
#define ENTITYPROPERTIESFORM_H

#include <QWidget>
#include "system/GameState.h"

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
    void ClearPanel();
    void AddComponent(const he::FixedString& id);

    // Events
    void OnGameStateChanged(const hs::EGameState from, const hs::EGameState to);
    void OnSelectionChanged();

    he::eventCallback2<void, hs::EGameState, hs::EGameState> m_GameStateChangedCallback;
    he::eventCallback0<void> m_SelectionChangedCallback;
    Ui::EntityPropertiesForm* m_UI;
};

#endif // ENTITYPROPERTIESFORM_H
