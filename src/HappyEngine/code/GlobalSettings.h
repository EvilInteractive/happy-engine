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
//Created: 28/05/2013

#ifndef _HE_GlobalSettings_H_
#define _HE_GlobalSettings_H_
#pragma once

#include "Singleton.h"
#include "RenderSettings.h"

namespace he {

namespace io {
    class StructuredVisitor;
}

class HAPPY_ENTRY GlobalSettings : public Singleton<GlobalSettings>
{
public:
    GlobalSettings();
    ~GlobalSettings();

    void load(const he::Path& file);
    void save(const he::Path& file);

    void visit(io::StructuredVisitor* const visitor);

    const gfx::RenderSettings& getRenderSettings() const { return m_RenderSettings; }
    gfx::RenderSettings& getRenderSettings() { return m_RenderSettings; }

private:
    gfx::RenderSettings m_RenderSettings;

    //Disable default copy constructor and default assignment operator
    GlobalSettings(const GlobalSettings&);
    GlobalSettings& operator=(const GlobalSettings&);
};

} //end namespace

#endif
