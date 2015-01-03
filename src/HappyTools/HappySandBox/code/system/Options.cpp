//HappyEngine Copyright (C) 2011 - 2015  Evil Interactive
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
//Created: 2015/01/03
#include "HappySandBoxPCH.h"
#include "Options.h"

#include <Property.h>
#include <PropertyDesc.h>
#include <PropertyFeel.h>
#include <PropertyConverter.h>

#include <JsonFileReader.h>
#include <JsonFileWriter.h>

namespace hs {

Options::Options()
{
    addPathProperty(HSFS::strEditorDataPath, "Editor data path", "The path where all the editor related data will be saved", he::Path::getUserDir().append("Editor"), true);
}

Options::~Options()
{
    m_Options.forEach([](const he::FixedString& /*key*/, he::ge::PropertyDesc* value)
    {
        delete value->m_Property;
        delete value->m_Converter;
        delete value->m_Feel;
        delete value;
    });
}

void Options::save()
{
    he::Path path(he::Path::getUserDir().append("Editor/config.json"));

    he::io::JsonFileWriter writer;
    if (writer.open(path))
    {
        visit(&writer);
        writer.close();
    }
}

void Options::load()
{
    he::Path path(he::Path::getUserDir().append("Editor/config.json"));

    he::io::JsonFileReader reader;
    if (reader.open(path))
    {
        visit(&reader);
        reader.close();
    }
}

void Options::visit( he::io::StructuredVisitor* visitor )
{
    if (visitor->enterNode(HSFS::strConfig))
    {
        he::String strval;
        m_Options.forEach([visitor, &strval](const he::FixedString& key, he::ge::PropertyDesc* value)
        {
            if (visitor->isWriting())
                strval = value->m_Converter->toString(value->m_Property);
            if (visitor->visit(key, strval))
            {
                if (visitor->isReading())
                {
                    value->m_Converter->fromString(value->m_Property, strval);
                }
            }
        });
        visitor->exitNode(HSFS::strConfig);
    }
}

void Options::addPathProperty( const he::FixedString& name, const char* displayName, const char* tooltip, he::Path defaultValue, const bool isDirectory, const he::String& filter /*= he::String()*/ )
{
    he::ge::Property* prop(NEW he::ge::Property());
    prop->init<he::Path>(name, std::move(defaultValue));
    he::ge::PropertyDesc* propDesc(NEW he::ge::PropertyDesc(
        prop, displayName, tooltip, NEW he::ge::PropertyConverterPath, NEW he::ge::PropertyFeelPath(isDirectory, filter)));
    m_Options.setAt(name, propDesc);
}

const he::Path& Options::getPathOption( const he::FixedString& name ) const
{
    he::ge::PropertyDesc* const* desc = m_Options.find(name);
    if (desc)
    {
        return (*desc)->m_Property->get<he::Path>();
    }
    else
    {
        LOG(he::LogType_ProgrammerAssert, "Could not find option with name %s", name.c_str());
        return he::Path::getBinPath();
    }
}


}

