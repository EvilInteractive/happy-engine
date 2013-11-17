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
//Author:  Bastian Damman
//Created: 20/08/2011

#ifndef _HE_IPHYSICS_SHAPE_H_
#define _HE_IPHYSICS_SHAPE_H_
#pragma once

namespace he {
namespace px {

enum PhysicsShapeType
{
    PhysicsShapeType_Box = 1 << 1,
    PhysicsShapeType_Convex = 1 << 2,
    PhysicsShapeType_Sphere = 1 << 3,
    PhysicsShapeType_Capsule = 1 << 4,
    PhysicsShapeType_Concave = 1 << 5,
    PhysicsShapeType_Plane = 1 << 6,
    PhysicsShapeType_HeightField = 1 << 7,

    PhysicsShapeType_StaticCompatible = PhysicsShapeType_Box | PhysicsShapeType_Convex |  PhysicsShapeType_Sphere |
                                        PhysicsShapeType_Capsule | PhysicsShapeType_Concave | PhysicsShapeType_Plane |
                                        PhysicsShapeType_HeightField,
    PhysicsShapeType_DynamicCompatible = PhysicsShapeType_Box | PhysicsShapeType_Convex |  PhysicsShapeType_Sphere |
                                         PhysicsShapeType_Capsule,
    PhysicsShapeType_TriggerCompatible = PhysicsShapeType_Box | PhysicsShapeType_Convex |  PhysicsShapeType_Sphere |
                                         PhysicsShapeType_Capsule
};
    
class HAPPY_ENTRY IPhysicsShape                                            
{                                                              
    public:                                                        
    virtual ~IPhysicsShape() {}

    virtual PhysicsShapeType getType() const = 0;
};

} } //end namespace

#endif
