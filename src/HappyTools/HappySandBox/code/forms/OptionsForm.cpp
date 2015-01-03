//HappyEngine Copyright (C) 2011 - 2015  Evil Interactive
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
//Author:  Bastian Damman
//Created: 2015/01/03
#include "HappySandBoxPCH.h"
#include "OptionsForm.h"
#include "ui_OptionsForm.h"

#include "system/Options.h"
#include <PropertyDesc.h>

namespace hs {

OptionsForm::OptionsForm(QWidget* parent)
    : QWidget(parent)
    , m_UI(NEW Ui::OptionsForm)
{
    m_UI->setupUi(this);

    connect(m_UI->m_SaveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(m_UI->m_LoadButton, SIGNAL(clicked()), this, SLOT(load()));
}

OptionsForm::~OptionsForm()
{

}

void OptionsForm::save()
{
    hs::Options::getInstance()->save();
    m_IsDirty = false;
}

void OptionsForm::load()
{
    hs::Options::getInstance()->load();
    m_UI->m_OptionList->clear();
    hs::Options::getInstance()->getOptions().forEach([this](const he::FixedString& /*name*/, const he::ge::PropertyDesc* desc)
    {
        m_UI->m_OptionList->addProperty(*desc);
    });
}

void OptionsForm::OnValueChanged( const he::ge::Property* /*prop*/ )
{
    m_IsDirty = true;
}

}

