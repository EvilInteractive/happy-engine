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
//Author:  Bastian Damman
//Created: 15/12/2012

#ifndef _HE_MaterialGeneratorNodeFactory_H_
#define _HE_MaterialGeneratorNodeFactory_H_
#pragma once

#include "Singleton.h"
#include "ObjectFactory.h"
#include "MaterialGeneratorNode.h"


namespace he {
namespace tools {
enum MaterialGeneratorNodeType;

class MaterialGeneratorNodeFactory : public ObjectFactory<MaterialGeneratorNode>, public Singleton<MaterialGeneratorNodeFactory>
{
public:
    MaterialGeneratorNodeFactory();
    virtual ~MaterialGeneratorNodeFactory();

    MaterialGeneratorNode* create(const MaterialGeneratorNodeType type);
    MaterialGeneratorNode* create(const MaterialGeneratorNodeType type, const Guid& id);
    void destroy(MaterialGeneratorNode* const node);

private:
    virtual ObjectHandle create() { LOG(LogType_ProgrammerAssert, "Disabled!"); return ObjectHandle::unassigned; }

    //Disable default copy constructor and default assignment operator
    MaterialGeneratorNodeFactory(const MaterialGeneratorNodeFactory&);
    MaterialGeneratorNodeFactory& operator=(const MaterialGeneratorNodeFactory&);
};

} } //end namespace

#endif
