#include "HappySandBoxPCH.h"
#include "EntityPropertySliderFeel.h"
#include "ui_EntityPropertySliderFeel.h"

namespace hs {

EntityPropertySliderFeel::EntityPropertySliderFeel(QWidget *parent) :
    QWidget(parent),
    m_UI(NEW Ui::EntityPropertySliderFeel)
{
    m_UI->setupUi(this);
}

EntityPropertySliderFeel::~EntityPropertySliderFeel()
{
    delete m_UI;
}

}