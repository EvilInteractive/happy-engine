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
//Created: 2015/02/14

#ifndef EntityComponentFrame_h__
#define EntityComponentFrame_h__

#include <QWidget>

namespace Ui {
    class EntityComponentFrame;
}

namespace hs {

class EntityComponentFrame : public QWidget
{
    Q_OBJECT

public:
    explicit EntityComponentFrame(QWidget *parent = 0);
    ~EntityComponentFrame();

    void setHeader(QWidget* header);
    void setContent(QWidget* content);
    void setExpanded(const bool expand);

    QWidget* getHeader() const { return m_Header; } 
    QWidget* getContent() const { return m_Content; }

    void resizeToFit();

private slots:
    void expandPressed(const bool val);

private:
    Ui::EntityComponentFrame* m_UI;

    QWidget* m_Header;
    QWidget* m_Content;
};

}

#endif // EntityComponentFrame_h__
