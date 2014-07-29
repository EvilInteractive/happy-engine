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
#include "EntityComponentDesc.h"

#include "PropertyConverter.h"
#include "PropertyFeel.h"

namespace he {
namespace ge {

IMPLEMENT_OBJECT(EntityComponent)

void EntityComponent::fillEntityComponentDesc( EntityComponentDesc& desc )
{
    Property* posProp(NEW Property());
    posProp->init<vec3>(HEFS::strTranslate, vec3(0, 0, 0));
    desc.m_Properties.setAt(posProp->getName(), PropertyDesc(posProp, "Local Position", "Sets the local position of the component", 
        NEW PropertyConverterVec3(), NEW PropertyFeelDefault()));

    Property* rotProp(NEW Property());
    rotProp->init<vec3>(HEFS::strRotate, vec3(0, 0, 0));
    desc.m_Properties.setAt(rotProp->getName(), PropertyDesc(rotProp, "Local Rotation", "Sets the local rotation of the component", 
        NEW PropertyConverterVec3(), NEW PropertyFeelDefault()));

    Property* scaleProp(NEW Property());
    scaleProp->init<vec3>(HEFS::strScale, vec3(1, 1, 1));
    desc.m_Properties.setAt(scaleProp->getName(), PropertyDesc(scaleProp, "Local Scale", "Sets the local scale of the component", 
        NEW PropertyConverterVec3(), NEW PropertyFeelDefault()));
}

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
    return checked_cast<Entity*>(getParent());
}


} } //end namespace
