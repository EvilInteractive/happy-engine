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
//Created: 05/08/2012

#ifndef _HE_PhysicsUserData_H_
#define _HE_PhysicsUserData_H_
#pragma once

namespace he {
namespace px {

class PhysicsUserData
{
public:
    PhysicsUserData(): m_Rtti(0), m_Data(nullptr) {}
    ~PhysicsUserData() {}

    RTTItype getRTTI() const { return m_Rtti; }
    void* getData() const { return m_Data; }

    void setRTTI(RTTItype type) { m_Rtti = type; }
    void setData(void* data) { m_Data = data; }

private:
    RTTItype m_Rtti;
    void* m_Data;

    //Disable default copy constructor and default assignment operator
    PhysicsUserData(const PhysicsUserData&);
    PhysicsUserData& operator=(const PhysicsUserData&);
};

} } //end namespace

#endif
