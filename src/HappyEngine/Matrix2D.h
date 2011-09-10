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
//Created: 10/09/2011

#ifndef _HE_MATRIX_2D_H_
#define _HE_MATRIX_2D_H_
#pragma once

#include "PxMat33.h"
#include "Vector2.h"

namespace happyengine {
namespace math {

class Matrix2D
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	Matrix2D();
	Matrix2D(physx::pubfnd3::PxMat33 mat);
	Matrix2D( float _00, float _01, float _02,
			  float _10, float _11, float _12,
			  float _20, float _21, float _22 );
    ~Matrix2D();

	/* DEFAULT COPY & ASSIGNMENT OPERATOR */

	/* STATIC CONSTRUCTORS */
	static Matrix2D createTranslaton(const Vector2& translation);
	static Matrix2D createRotation(const float radians);
	static Matrix2D createScale(const Vector2& scale);

	/* OPERATORS */
    Matrix2D operator*(const Matrix2D& mat);
    Vector2 operator*(const Vector2& vec);

	/* GETTERS */
	math::Vector2 getTranslation() const;

	/* STATIC */
	static const Matrix2D Identity;

private:

    /* DATAMEMBERS */
	physx::pubfnd3::PxMat33 m_Matrix;
};

} } //end namespace

#endif
