//HappyTest Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _PHYSICSDATA_H_
#define _PHYSICSDATA_H_
#pragma once

namespace happytest {

enum DriveableMaterial
{
    DM_Grass = 0,
    DM_Sand,
    DM_Wood,
    DM_Ice,
    DM_Tarmac,
    DM_Concrete,
    DM_Tyre,
    DM_Metal,
    MAX_DRIVEABLE_MATERIALS
};

enum TyreType
{
    TT_Slicks = 0,
    TT_Wets,
    MAX_TYRE_TYPES
};

} //end namespace

#endif
