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
#include "HappyPCH.h" 

#include "LightFactory.h"

#include "SpotLight.h"
#include "PointLight.h"

namespace he {
namespace gfx {

LightFactory::LightFactory()
{
    init(32, 32, "LightFactory");
}


LightFactory::~LightFactory()
{
}

void LightFactory::destroyLight( const ObjectHandle& handle )
{
    destroyObject(handle);
}

Light* LightFactory::get(const ObjectHandle handle ) const
{
    return ObjectFactory<Light>::get(handle);
}

he::ObjectHandle gfx::LightFactory::createPointLight()
{
    return registerObject(NEW PointLight());
}

he::ObjectHandle gfx::LightFactory::createSpotLight()
{
    return registerObject(NEW SpotLight());
}

SpotLight* gfx::LightFactory::getSpotLight( const ObjectHandle& handle ) const
{
    Light* light(get(handle));
    HE_ASSERT(light != nullptr, "Could not get light from light factory");
    HE_ASSERT(light->getType() == LightType_Spot, "Trying to get pointlight as spotlight!");
    return static_cast<SpotLight*>(light);
}

PointLight* gfx::LightFactory::getPointLight( const ObjectHandle& handle ) const
{
    Light* light(get(handle));
    HE_ASSERT(light != nullptr, "Could not get light from light factory");
    HE_ASSERT(light->getType() == LightType_Point, "Trying to get spotlight as pointlight!");
    return static_cast<PointLight*>(light);
}

} } //end namespace
