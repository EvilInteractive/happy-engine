//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
//Created: 2014/11/22

#ifndef EntityPropertyList_h__
#define EntityPropertyList_h__

#include "PropertyList.h"

namespace hs {

class EntityPropertyList : public PropertyList
{
    Q_OBJECT
public:
    explicit EntityPropertyList(QWidget *parent = 0);
    ~EntityPropertyList();

    void setComponentType(const he::FixedString& compType) { m_ComponentType = compType; }
    const he::FixedString& getComponentType() const { return m_ComponentType; }

private:
    he::FixedString m_ComponentType;

    EntityPropertyList(const EntityPropertyList&);
    EntityPropertyList& operator=(const EntityPropertyList&);
};

}

#endif // EntityPropertyList_h__
