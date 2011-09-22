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

#ifndef _HE_MATRIX_H_
#define _HE_MATRIX_H_
#pragma once

#include "PxMat44.h"
#include "Vector3.h"
#include "Vector4.h"

namespace happyengine {
namespace math {

class Matrix
{
private:
    physx::pubfnd3::PxMat44 m_Matrix;

public:
	Matrix();
	explicit Matrix(physx::pubfnd3::PxMat44 mat);
	Matrix(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3);
	Matrix( float _00, float _01, float _02, float _03,
            float _10, float _11, float _12, float _13,
            float _20, float _21, float _22, float _23,
            float _30, float _31, float _32, float _33 );
    ~Matrix();
    //default copy constructor and assignment operator are fine

    //static constructors
    static Matrix createPerspectiveLH(float fov, float viewWidth, float viewHeight, float zNear, float zFar);
    static Matrix createPerspectiveRH(float fov, float viewWidth, float viewHeight, float zNear, float zFar);
    static Matrix createOrthoLH(float left, float right, float top, float bottom, float zNear, float zFar);
    static Matrix createOrthoRH(float left, float right, float top, float bottom, float zNear, float zFar);
    static Matrix createLookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up);
    static Matrix createLookAtRH(const Vector3& eye, const Vector3& target, const Vector3& up);
    static Matrix createTranslation(const Vector3& translation);
    static Matrix createRotation(const Vector3& axis, float radians);
    static Matrix createScale(const Vector3& scale);
    static Matrix createScale(float xScale, float yScale, float zScale);
    static Matrix createScale(float scale);

    //operators
    Matrix operator*(const Matrix& mat);
    Vector3 operator*(const Vector3& vec);
    Vector4 operator*(const Vector4& vec);

    //getters
    void toFloatArray(float arr[16]) const;
    math::Vector3 getTranslation() const;
    const physx::pubfnd3::PxMat44& getPhyicsMatrix() const;
    math::Matrix inverse() const;

	//Static
    static const Matrix Identity;
};

} } //end namespace

#endif
