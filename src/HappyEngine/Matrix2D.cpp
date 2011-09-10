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

#include "Matrix2D.h"
#include "HappyNew.h"

namespace happyengine {
namespace math { 

/* CONSTRUCTOR - DESTRUCTOR */
Matrix2D::Matrix2D() :	m_Matrix(physx::pubfnd3::PxMat33::createIdentity())
{
}

Matrix2D::Matrix2D(physx::pubfnd3::PxMat33 mat) :	m_Matrix(mat)
{
}

Matrix2D::Matrix2D( float _00, float _01, float _02,
					float _10, float _11, float _12,
					float _20, float _21, float _22 ) :	
	m_Matrix(physx::pubfnd3::PxVec3(_00, _01, _02),
			 physx::pubfnd3::PxVec3(_10, _11, _12),
			 physx::pubfnd3::PxVec3(_20, _21, _22))
{
}

Matrix2D::~Matrix2D()
{
}

/* STATIC CONSTRUCTORS */
Matrix2D Matrix2D::createTranslaton(const Vector2& translation)
{
	return Matrix2D(1.0f, 0.0f, translation.x,
					0.0f, 1.0f, translation.y,
					0.0f, 1.0f, 0.0f );
}

Matrix2D Matrix2D::createRotation(const float radians)
{
	return Matrix2D(cosf(radians), -sinf(radians), 0.0f,
					sinf(radians), cosf(radians), 0.0f,
					0.0f, 0.0f, 1.0f );
}

Matrix2D Matrix2D::createScale(const Vector2& scale)
{
	return Matrix2D(scale.x, 0.0f, 0.0f,
					0.0f, scale.y, 0.0f,
					0.0f, 0.0f, 1.0f);
}

/* OPERATORS */
Matrix2D Matrix2D::operator*(const Matrix2D& mat)
{
	return Matrix2D(m_Matrix * mat.m_Matrix);
}

Vector2 Matrix2D::operator*(const Vector2& vec)
{
	return Vector2(m_Matrix.transform(physx::pubfnd3::PxVec3(vec.x, vec.y, 0.0f)).x, m_Matrix.transform(physx::pubfnd3::PxVec3(vec.x, vec.y, 0.0f)).y);
}

/* GETTERS */
math::Vector2 Matrix2D::getTranslation() const
{
	return math::Vector2(m_Matrix.column2.x, m_Matrix.column2.y);
}

/* STATIC */
const Matrix2D Matrix2D::Identity = Matrix2D(1.0f, 0.0f, 0.0f,
											 0.0f, 1.0f, 0.0f,
											 0.0f, 0.0f, 1.0f);

} } //end namespace