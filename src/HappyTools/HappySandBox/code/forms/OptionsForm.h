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

#ifndef OptionsForm_h__
#define OptionsForm_h__

#include <QWidget>

namespace Ui {
    class OptionsForm;
}

namespace he {
namespace ge {
    class Property;
} }

namespace hs {

class OptionsForm : public QWidget
{
    Q_OBJECT
public:
    explicit OptionsForm(QWidget* parent);
    ~OptionsForm();

public slots:
    void save();
    void load();

private:
    void OnValueChanged(const he::ge::Property* prop);

    Ui::OptionsForm* m_UI;
    bool m_IsDirty;

    OptionsForm(const OptionsForm&);
    OptionsForm& operator=(const OptionsForm&);
};

}

#endif // OptionsForm_h__
