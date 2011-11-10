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
//Author:  Sebastiaan Sprengers
//Created: 09/11/2011

#ifndef _HE_SSAO_H_
#define _HE_SSAO_H_
#pragma once

#include "HappyTypes.h"
#include "Texture2D.h"

namespace he {
namespace gfx {

struct SSAOSettings
{
	float radius;
	float intensity;
	float scale;
	float bias;
	byte minIterations;
	byte maxIterations;
};

class SSAO
{
public:

	/* CONSTRUCTOR - DESCTRUCTOR */
	SSAO(uint normalBuffer, uint positionBuffer);
    virtual ~SSAO();

	/* GENERAL */

	/* SETTERS */
	void setSettings(const SSAOSettings& settings);

private:

    /* DEFAULT COPY & ASSIGNMENT */
    SSAO(const SSAO&);
    SSAO& operator=(const SSAO&);
};

} } //end namespace

#endif
