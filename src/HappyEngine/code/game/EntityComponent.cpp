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
//Created: 04/09/2012
#include "HappyPCH.h"
#include "EntityComponent.h"

#include "Property.h"
#include "Entity.h"

namespace he {
namespace ge {

IMPLEMENT_OBJECT(EntityComponent)

bool EntityComponent::setProperty( const Property* const inProperty )
{
    bool result(true);
    const he::FixedString& name(inProperty->getName());
    if (name == HEFS::strTranslate)
    {
        setLocalTranslate(inProperty->get<vec3>());
    }
    else if (name == HEFS::strRotate)
    {
        const vec3& rotate(inProperty->get<vec3>());
        setLocalRotate(mat33::createRotation3D(rotate));
    }
    else if (name == HEFS::strScale)
    {
        setLocalScale(inProperty->get<vec3>());
    }
    else
    {
        result = false;
    }
    return result;
}

bool EntityComponent::getProperty( Property* const inOutProperty )
{
    bool result(true);
    const he::FixedString& name(inOutProperty->getName());
    if (name == HEFS::strTranslate)
    {
        inOutProperty->set<vec3>(getLocalTranslate());
    }
    else if (name == HEFS::strRotate)
    {
        inOutProperty->set<vec3>(getLocalRotate().getEulerAngles());       
    }
    else if (name == HEFS::strScale)
    {
        inOutProperty->set<vec3>(getLocalScale());
    }
    else
    {
        result = false;
    }
    return result;
}

Entity* EntityComponent::getEntityParent()
{
    return checked_cast<Entity*>(checked_cast<Object3D*>(getParent()));
}

} } //end namespace
