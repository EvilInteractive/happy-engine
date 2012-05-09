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
#include "HappyPCH.h" 

#include "mat33.h"
#include "HappyNew.h"

namespace he {

/* CONSTRUCTOR - DESTRUCTOR */
mat33::mat33() :	m_Matrix(physx::pubfnd3::PxMat33::createIdentity())
{
}

mat33::mat33(physx::pubfnd3::PxMat33 mat) :	m_Matrix(mat)
{
}

mat33::mat33( float _00, float _01, float _02,
					float _10, float _11, float _12,
					float _20, float _21, float _22 ) :	
	m_Matrix(physx::pubfnd3::PxVec3(_00, _01, _02),
			 physx::pubfnd3::PxVec3(_10, _11, _12),
			 physx::pubfnd3::PxVec3(_20, _21, _22))
{
}

mat33::~mat33()
{
}

/* STATIC CONSTRUCTORS */
mat33 mat33::createTranslaton(const vec2& translation)
{
	return mat33(1.0f, 0.0f, translation.x,
					0.0f, 1.0f, translation.y,
					0.0f, 1.0f, 0.0f );
}

mat33 mat33::createRotation(const float radians)
{
	return mat33(cosf(radians), -sinf(radians), 0.0f,
					sinf(radians), cosf(radians), 0.0f,
					0.0f, 0.0f, 1.0f );
}

mat33 mat33::createScale(const vec2& scale)
{
	return mat33(scale.x, 0.0f, 0.0f,
					0.0f, scale.y, 0.0f,
					0.0f, 0.0f, 1.0f);
}

/* OPERATORS */
mat33 mat33::operator*(const mat33& mat)
{
	return mat33(m_Matrix * mat.m_Matrix);
}

vec2 mat33::operator*(const vec2& vec)
{
	return vec2(m_Matrix.transform(physx::pubfnd3::PxVec3(vec.x, vec.y, 0.0f)).x, m_Matrix.transform(physx::pubfnd3::PxVec3(vec.x, vec.y, 0.0f)).y);
}

/* GETTERS */
vec2 mat33::getTranslation() const
{
	return vec2(m_Matrix.column2.x, m_Matrix.column2.y);
}

mat44 mat33::getMat44() const
{
    return mat44(   m_Matrix.column0.x, m_Matrix.column0.y, m_Matrix.column0.z, 0,
                    m_Matrix.column1.x, m_Matrix.column1.y, m_Matrix.column1.z, 0,
                    m_Matrix.column2.x, m_Matrix.column2.y, m_Matrix.column2.z, 0,
                    0, 0, 0, 1);
}

/* STATIC */
const mat33 mat33::Identity = mat33(1.0f, 0.0f, 0.0f,
											 0.0f, 1.0f, 0.0f,
											 0.0f, 0.0f, 1.0f);

} //end namespace