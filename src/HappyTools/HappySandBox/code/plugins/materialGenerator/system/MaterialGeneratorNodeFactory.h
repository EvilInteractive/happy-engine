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
//Created: 15/12/2012

#ifndef _HE_MaterialGeneratorNodeFactory_H_
#define _HE_MaterialGeneratorNodeFactory_H_
#pragma once

#include "Singleton.h"
#include "ObjectFactory.h"

#include "MaterialGeneratorNode.h"


namespace hs {

class MaterialGeneratorNodeFactory : public he::ObjectFactory<MaterialGeneratorNode, he::NoCreateObjectAllocator<MaterialGeneratorNode>>, public he::Singleton<MaterialGeneratorNodeFactory>
{
public:
    MaterialGeneratorNodeFactory();
    virtual ~MaterialGeneratorNodeFactory();

    MaterialGeneratorNode* create(MaterialGraph* const graph, const he::FixedString& type);
    MaterialGeneratorNode* create(MaterialGraph* const graph, const he::FixedString& type, const he::Guid& id);
    void destroy(MaterialGeneratorNode* const node);

private:
    virtual he::ObjectHandle create() { LOG(he::LogType_ProgrammerAssert, "Disabled!"); return he::ObjectHandle::unassigned; }

    //Disable default copy constructor and default assignment operator
    MaterialGeneratorNodeFactory(const MaterialGeneratorNodeFactory&);
    MaterialGeneratorNodeFactory& operator=(const MaterialGeneratorNodeFactory&);
};

} //end namespace

#endif
