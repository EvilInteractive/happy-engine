//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 05/03/2013

#ifndef _HT_ENTITYMANAGER_H_
#define _HT_ENTITYMANAGER_H_
#pragma once

namespace he {
    namespace ge {
        class Entity;
    }
    namespace gfx {
        class Scene;
    }
}

namespace hs {

class EntityManager
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    EntityManager(he::gfx::Scene* scene);
    virtual ~EntityManager();

    /* GENERAL */
    he::ge::Entity* createEntity();

private:

    he::gfx::Scene* m_Scene;
    he::PrimitiveList<he::ge::Entity*> m_Entities;

    /* DEFAULT COPY & ASSIGNMENT */
    EntityManager(const EntityManager&);
    EntityManager& operator=(const EntityManager&);
};

} //end namespace

#endif
