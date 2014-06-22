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
//Author:  
//Created: //

#ifndef _HT_SelectionManger_H_
#define _HT_SelectionManger_H_
#pragma once

#include <Singleton.h>
#include <IShapeDrawable.h>
#include <AABB.h>

#include "SandboxTypes.h"

namespace he {
namespace ge {
    class Entity;
} }

namespace hs {
    
    class SelectionManger : public he::Singleton<SelectionManger>, he::gfx::IShapeDrawable
{
public:
    SelectionManger();
    virtual ~SelectionManger();

    void init();
    void destroy();
    
    void deselect(he::ge::Entity* const entity);
    void deselectAll();
    void select(he::ge::Entity* const entity);

    const SelectionSet& getSelection() const;

    void drawShapes(he::gfx::ShapeRenderer* const renderer); // override

    const static he::Color s_SelectionColor;

private:
    void internalDeselect(he::ge::Entity* const entity);
    void internalSelect(he::ge::Entity* const entity);

    void recomputeBoundingBox();

    SelectionSet m_Selection;

    he::AABB m_AABB;

    /* DEFAULT COPY & ASSIGNMENT */
    SelectionManger(const SelectionManger&);
    SelectionManger& operator=(const SelectionManger&);
};

} //end namespace

#endif
