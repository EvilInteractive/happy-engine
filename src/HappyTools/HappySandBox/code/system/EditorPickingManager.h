//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Author:  Sebastiaan Sprengers
//Created: 11/11/2011

#ifndef _HE_EditorPickingManager_H_
#define _HE_EditorPickingManager_H_
#pragma once

#include "Singleton.h"
#include "IPickingManager.h"

namespace he {
class Ray;
namespace ge {
class PickResult;
class Pickable;
}}

namespace hs 
{

class EditorPickingManager : public he::ge::IPickingManager, public he::Singleton<EditorPickingManager>
{
    friend class he::Singleton<EditorPickingManager>;
public:
    virtual ~EditorPickingManager();

    bool pick(const he::Ray& ray, he::ge::PickResult& result);

    void addToPickList(he::ge::Pickable* const object);
    void removeFromPickList(he::ge::Pickable* const object);

private:
    EditorPickingManager();
    
    he::PrimitiveList<he::ge::Pickable*> m_PickList;

    /* DEFAULT COPY & ASSIGNMENT */
    EditorPickingManager(const EditorPickingManager&);
    EditorPickingManager& operator=(const EditorPickingManager&);
};

} //end namespace

#endif
