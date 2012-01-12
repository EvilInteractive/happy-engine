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
//Author:  Sebastiaan Sprengers
//Created: 11/12/2011

#ifndef _DEBUG_INFO_H_
#define _DEBUG_INFO_H_
#pragma once

#include "FPSGraph.h"
#include "ITickable.h"
#include "Font.h"

namespace happysandbox {

class SceneInfo : public he::game::ITickable
{
public:

    /* CONSTRUCTOR & DESTRUCTOR */
    SceneInfo();
    virtual ~SceneInfo();
    
    /* GENERAL */
    void tick(float dTime);
    void draw();

private:

    he::tools::FPSGraph* m_pFPSGraph;

    he::gfx::Font::pointer m_pFont;

    he::vec3 m_CamPos;

    he::vec2 m_ScreenDimensions;

    /* DEFAULT COPY & ASSIGNMENT */
    SceneInfo(const SceneInfo&);
    SceneInfo& operator=(const SceneInfo&);
};

} //end namespace

#endif
