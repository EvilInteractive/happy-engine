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
#include "HappyPCH.h"
#include "GlobalSettings.h"

#include "JsonFileReader.h"
#include "JsonFileWriter.h"

#define VERSION 1

namespace he {

GlobalSettings::GlobalSettings()
{

}

GlobalSettings::~GlobalSettings()
{

}

void GlobalSettings::load( const he::Path& file )
{
    io::JsonFileReader reader;
    if (reader.open(file))
    {
        visit(&reader);
        reader.close();
    }
    else
    {
        LOG(LogType_Error, "Unable to load settings file from file %s", file.str().c_str());
    }
}

void GlobalSettings::save( const he::Path& file )
{
    io::JsonFileWriter writer;
    if (writer.open(file))
    {
        visit(&writer);
        writer.close();
    }
    else
    {
        LOG(LogType_Error, "Unable to save settings file to %s", file.str().c_str());
    }
}

void GlobalSettings::visit( io::StructuredVisitor* const visitor )
{
    uint32 version(VERSION);
    visitor->visit(HEFS::strVersion, version);
    m_RenderSettings.visit(visitor);
}

} //end namespace
