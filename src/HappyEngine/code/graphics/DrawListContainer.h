//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
//Created: 17/12/2011

#ifndef _HE_DRAWLIST_CONTAINER_H_
#define _HE_DRAWLIST_CONTAINER_H_
#pragma once

#include "boost/function.hpp"
#include <vector>
#include "HappyTypes.h"

namespace he {
namespace gfx {

class IDrawable;
class Camera;

class DrawListContainer
{
public:
    DrawListContainer();
    virtual ~DrawListContainer();

    enum Filter
    {
        F_Main_Opac = 1 << 0,
        F_Main_Tranlucent = 1 << 1,
        F_Loc_AfterPost = 1 << 2,
        F_Loc_BeforePost = 1 << 3,
        F_Sub_Single = 1 << 4,
        F_Sub_Skinned = 1 << 5,
        F_Sub_Instanced = 1 << 6,
    };
    void for_each(uint filter, const boost::function<void(IDrawable*)>& f) const;

    void insert(IDrawable* pDrawable);
    void remove(const IDrawable* pDrawable);

    typedef std::vector<IDrawable*> Container; 

private:

    static const int BEFOREPOST_INDEX = 0;
    static const int AFTERPOST_INDEX = 1;
    static const int MAX_I0 = 2;

    static const int OPAC_INDEX = 0;
    static const int TRANSLUCENT_INDEX = 1;
    static const int MAX_I1 = 2;

    static const int SINGLE_INDEX = 0;
    static const int SKINNED_INDEX = 1;
    static const int INSTANCE_INDEX = 2;
    static const int MAX_I2 = 3;

    Container m_DrawList[MAX_I0][MAX_I1][MAX_I2]; //Loc - Main - Sub

    //Disable default copy constructor and default assignment operator
    DrawListContainer(const DrawListContainer&);
    DrawListContainer& operator=(const DrawListContainer&);
};

} } //end namespace

#endif
