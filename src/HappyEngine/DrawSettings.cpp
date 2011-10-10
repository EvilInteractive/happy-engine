//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
//Created: 28/09/2011
#include "StdAfx.h" 

#include "DrawSettings.h"
#include "HappyNew.h"

namespace he {
namespace gfx {

DrawSettings::DrawSettings()
{
    setViewDistanceQuality(ViewDistanceQuality_Medium);
}


DrawSettings::~DrawSettings()
{
}

void DrawSettings::setViewDistanceQuality(ViewDistanceQuality quality)
{
    switch (quality)
    {
        case ViewDistanceQuality_Low: 
            m_FogStart = 300;
            m_FogEnd = 500;
            break;
        case ViewDistanceQuality_Medium: 
            m_FogStart = 500;
            m_FogEnd = 800;
            break;
        case ViewDistanceQuality_High: 
            m_FogStart = 800;
            m_FogEnd = 1000;
            break;
    };
}
float DrawSettings::getFogStart() const
{
    return m_FogStart;
}
float DrawSettings::getFogEnd() const
{
    return m_FogEnd;
}

} } //end namespace