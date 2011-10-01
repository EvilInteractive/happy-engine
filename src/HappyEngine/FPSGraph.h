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
//Author: Sebastiaan Sprengers 
//Created: 30/08/2011

#ifndef _FPS_GRAPH_H_
#define _FPS_GRAPH_H_
#pragma once

#define BOOST_DISABLE_ASSERTS

#include <vector>
#include "HappyTypes.h"
#include "Font.h"

namespace happyengine {
namespace tools {

class FPSGraph
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	FPSGraph();
    virtual ~FPSGraph();

	/* GENERAL */
	void tick(float dTime, float interval = 0.5f);
	void draw();

	/* GETTERS */
	uint FPSGraph::getMaxFPS() const;
	uint FPSGraph::getMinFPS() const;

private:

	/* DATAMEMBERS */
	std::vector<happyengine::uint> m_FpsHistory;

	float m_GameTime;
	float m_TBase;
	float m_CurrentDTime;
	float m_Interval;
	uint m_CurrentFPS;
	
	graphics::Font::pointer m_pFont;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    FPSGraph(const FPSGraph&);
    FPSGraph& operator=(const FPSGraph&);
};

} } //end namespace

#endif
