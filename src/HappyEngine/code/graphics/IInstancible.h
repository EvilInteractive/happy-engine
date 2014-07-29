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
//Created: 28/12/2011

#ifndef _HE_IINSTANCIBLE_H_
#define _HE_IINSTANCIBLE_H_
#pragma once

namespace he {
class Bound;
namespace gfx {

class DynamicBuffer;
class HAPPY_ENTRY IInstancible : public IObject3D
{
public:
    virtual ~IInstancible() {}
    
    virtual void fillInstancingBuffer(DynamicBuffer& buffer) const = 0;
}; 

}}  //end namespace

#endif
