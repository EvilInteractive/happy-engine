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
//Created: 10/09/2011

#ifndef _HE_MATRIX_2D_H_
#define _HE_MATRIX_2D_H_
#pragma once

#include "PxMat33.h"
#include "vec2.h"

namespace he {

class mat33
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	mat33();
	mat33(physx::pubfnd3::PxMat33 mat);
	mat33( float _00, float _01, float _02,
			  float _10, float _11, float _12,
			  float _20, float _21, float _22 );
    ~mat33();

	/* DEFAULT COPY & ASSIGNMENT OPERATOR */

	/* STATIC CONSTRUCTORS */
	static mat33 createTranslaton(const vec2& translation);
	static mat33 createRotation(const float radians);
	static mat33 createScale(const vec2& scale);

	/* OPERATORS */
    mat33 operator*(const mat33& mat);
    vec2 operator*(const vec2& vec);

	/* GETTERS */
	vec2 getTranslation() const;
    mat44 getMat44() const;

	/* STATIC */
	static const mat33 Identity;

private:

    /* DATAMEMBERS */
	physx::pubfnd3::PxMat33 m_Matrix;
};

} //end namespace

#endif
