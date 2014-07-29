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
//Created: 2013/04/20
#include "HappySandBoxPCH.h" 
#include "SandboxEntityComponentFactory.h"

#include "EditorComponent.h"

namespace hs {

he::ge::EntityComponent* SandboxEntityComponentFactory::createEntityComponent( const he::FixedString& type ) const
{
    he::ge::EntityComponent* result(nullptr);
    if (type == HSFS::strEditorComponent)
    {
        result = NEW EditorComponent();
    }
    return result;
}

void SandboxEntityComponentFactory::fillComponentDescList( he::PrimitiveList<he::ge::EntityComponentDesc*>& /*list*/ ) const
{
}


} //end namespace
