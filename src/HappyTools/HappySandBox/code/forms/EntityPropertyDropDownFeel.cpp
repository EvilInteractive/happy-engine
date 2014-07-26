#include "HappySandBoxPCH.h"
#include "EntityPropertyDropDownFeel.h"
#include "ui_EntityPropertyDropDownFeel.h"

namespace hs {

EntityPropertyDropDownFeel::EntityPropertyDropDownFeel(QWidget *parent) :
    QWidget(parent),
    m_UI(NEW Ui::EntityPropertyDropDownFeel)
{
    m_UI->setupUi(this);
}

EntityPropertyDropDownFeel::~EntityPropertyDropDownFeel()
{
    delete m_UI;
}

}