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
#include "StdAfx.h" 

#include "mat44.h"
#include "MathFunctions.h"

namespace he {

mat44::mat44(): m_Matrix(physx::pubfnd3::PxVec4(1.0f, 1.0f, 1.0f, 1.0f))
{
}
mat44::mat44(physx::pubfnd3::PxMat44 mat): m_Matrix(mat)
{
}
mat44::mat44(const vec4& col0, const vec4& col1, const vec4& col2, const vec4& col3):
    m_Matrix(physx::pubfnd3::PxVec4(col0.x, col0.y, col0.z, col0.w),
             physx::pubfnd3::PxVec4(col1.x, col1.y, col1.z, col1.w),
             physx::pubfnd3::PxVec4(col2.x, col2.y, col2.z, col2.w),
             physx::pubfnd3::PxVec4(col3.x, col3.y, col3.z, col3.w))
{
}
mat44::mat44( float _00, float _01, float _02, float _03,
                float _10, float _11, float _12, float _13,
                float _20, float _21, float _22, float _23,
                float _30, float _31, float _32, float _33 ) : 
m_Matrix(physx::pubfnd3::PxVec4(_00, _10, _20, _30),
         physx::pubfnd3::PxVec4(_01, _11, _21, _31),
         physx::pubfnd3::PxVec4(_02, _12, _22, _32),
         physx::pubfnd3::PxVec4(_03, _13, _23, _33))
{
}
mat44::~mat44()
{
}

//static constructors
mat44 mat44::createPerspectiveRH(float fov, float viewWidth, float viewHeight, float zNear, float zFar)
{
    float _11 = cosf(fov * 0.5f) / sinf(fov * 0.5f);
    float _00 = _11 * viewHeight / viewWidth;
    return mat44(
         _00, 0.0f, 0.0f                            , 0.0f,
        0.0f,  _11, 0.0f                            , 0.0f,
        0.0f, 0.0f, (zFar) / (zFar - zNear)         , (zFar * zNear) / (zFar - zNear),
        0.0f, 0.0f, -1.0f                           , 0.0f);
}
mat44 mat44::createPerspectiveLH(float fov, float viewWidth, float viewHeight, float zNear, float zFar)
{
    float _11 = cosf(fov * 0.5f) / sinf(fov * 0.5f);
    float _00 = _11 * viewHeight / viewWidth;
    return mat44(
         _00, 0.0f, 0.0f                     , 0.0f,
        0.0f,  _11, 0.0f                     , 0.0f,
        0.0f, 0.0f, (zFar) / (zFar - zNear)  , -(zFar * zNear) / (zFar - zNear),
        0.0f, 0.0f, 1.0f                     , 0.0f);
}
mat44 mat44::createOrthoLH(float left, float right, float top, float bottom, float zNear, float zFar)
{
    return mat44(
        2.0f / (right - left),                    0.0f,                    0.0f,   (left + right) / (left - right),
                         0.0f,   2.0f / (top - bottom),                    0.0f,   (top + bottom) / (bottom - top),
                         0.0f,                    0.0f,   1.0f / (zFar - zNear),            zNear / (zNear - zFar),
                         0.0f,                    0.0f,                    0.0f,                             1.0f);
}
mat44 mat44::createOrthoRH(float left, float right, float top, float bottom, float zNear, float zFar)
{
    return mat44(
        2.0f / (right - left),                    0.0f,                    0.0f,   (left + right) / (left - right),
                         0.0f,   2.0f / (top - bottom),                    0.0f,   (top + bottom) / (bottom - top),
                         0.0f,                    0.0f,   1.0f / (zNear - zFar),            zNear / (zNear - zFar),
                         0.0f,                    0.0f,                    0.0f,                             1.0f);
}
mat44 mat44::createLookAtLH(const vec3& eye, const vec3& target, const vec3& up)
{
    vec3 zaxis(normalize(target - eye));
    vec3 xaxis(normalize(cross(normalize(up), zaxis)));
    vec3 yaxis(cross(zaxis, xaxis));

    return mat44(
           xaxis.x,  xaxis.y,  xaxis.z, -dot(xaxis, eye),
           yaxis.x,  yaxis.y,  yaxis.z, -dot(yaxis, eye),
           zaxis.x,  zaxis.y,  zaxis.z, -dot(zaxis, eye),
              0.0f,     0.0f,     0.0f,            1.0f);     
}
mat44 mat44::createLookAtRH(const vec3& eye, const vec3& target, const vec3& up)
{
    vec3 zaxis(normalize(eye - target));
    vec3 xaxis(normalize(cross(normalize(up), zaxis)));
    vec3 yaxis(cross(zaxis, xaxis));

    return mat44(
           -xaxis.x,  -xaxis.y,  -xaxis.z,  dot(xaxis, eye),
            yaxis.x,   yaxis.y,   yaxis.z, -dot(yaxis, eye),
           -zaxis.x,  -zaxis.y,  -zaxis.z, -dot(zaxis, eye),
              0.0f,     0.0f,     0.0f,            1.0f);     
}
mat44 mat44::createTranslation(const vec3& translation)
{
    return mat44(1.0f, 0.0f, 0.0f, translation.x,
                  0.0f, 1.0f, 0.0f, translation.y,
                  0.0f, 0.0f, 1.0f, translation.z,
                  0.0f, 0.0f, 0.0f, 1.0f);

}
mat44 mat44::createRotation(const vec3& axis, float radians)
{
    float cosAlpha(cosf(radians));
    float sinAlpha(sinf(radians));

    return mat44(
        cosAlpha + sqr(axis.x) * (1 - cosAlpha),                    axis.x * axis.y * (1 - cosAlpha) - axis.z * sinAlpha,       axis.x * axis.z * (1 - cosAlpha) + axis.y * sinAlpha,       0.0f,
        axis.x * axis.y * (1 - cosAlpha) + axis.z * sinAlpha,       cosAlpha + sqr(axis.y) * (1 - cosAlpha),                    axis.y * axis.z * (1 - cosAlpha) - axis.x * sinAlpha,       0.0f,
        axis.x * axis.z * (1 - cosAlpha) - axis.y * sinAlpha,       axis.y * axis.z * (1 - cosAlpha) + axis.x * sinAlpha,       cosAlpha + sqr(axis.z) * (1 - cosAlpha),                    0.0f,
        0.0f,                                                       0.0f,                                                       0.0f,                                                       1.0f);
}
mat44 mat44::createScale(const vec3& scale)
{
    return mat44(scale.x, 0.0f, 0.0f,    0.0f,
                  0.0f, scale.y, 0.0f,    0.0f,
                  0.0f,    0.0f, scale.z, 0.0f,
                  0.0f,    0.0f, 0.0f,    1.0f);
}
mat44 mat44::createScale(float xScale, float yScale, float zScale)
{
    return mat44(xScale,  0.0f,   0.0f,  0.0f,
                  0.0f,  yScale,   0.0f,  0.0f,
                  0.0f,    0.0f, zScale,  0.0f,
                  0.0f,    0.0f,   0.0f,  1.0f);
}
mat44 mat44::createScale(float scale)
{
    return mat44(scale,  0.0f,  0.0f,  0.0f,
                  0.0f,  scale,  0.0f,  0.0f,
                  0.0f,   0.0f, scale,  0.0f,
                  0.0f,   0.0f,  0.0f,  1.0f);
}

mat44 mat44::operator*(const mat44& mat) const
{
    return mat44(m_Matrix * mat.m_Matrix);
}
vec3 mat44::operator*(const vec3& vec) const
{
    return vec3(m_Matrix.transform(physx::pubfnd3::PxVec3(vec.x, vec.y, vec.z)));
}
vec4 mat44::operator*(const vec4& vec) const
{
    return vec4(m_Matrix.transform(physx::pubfnd3::PxVec4(vec.x, vec.y, vec.z, vec.w)));
}

void mat44::toFloatArray(float arr[16]) const
{
    arr[0] = m_Matrix.column0.x;
    arr[1] = m_Matrix.column0.y;
    arr[2] = m_Matrix.column0.z;
    arr[3] = m_Matrix.column0.w;

    arr[4] = m_Matrix.column1.x;
    arr[5] = m_Matrix.column1.y;
    arr[6] = m_Matrix.column1.z;
    arr[7] = m_Matrix.column1.w;

    arr[8] = m_Matrix.column2.x;
    arr[9] = m_Matrix.column2.y;
    arr[10] = m_Matrix.column2.z;
    arr[11] = m_Matrix.column2.w;

    arr[12] = m_Matrix.column3.x;
    arr[13] = m_Matrix.column3.y;
    arr[14] = m_Matrix.column3.z;
    arr[15] = m_Matrix.column3.w;
}

vec3 mat44::getTranslation() const
{
    return vec3(m_Matrix.column3.x, m_Matrix.column3.y, m_Matrix.column3.z);
}
const physx::pubfnd3::PxMat44& mat44::getPhyicsMatrix() const
{
    return m_Matrix;
}
mat44 mat44::inverse() const
{
    return mat44(m_Matrix.inverseRT());
}

float mat44::operator()( int row, int column ) const
{
    return m_Matrix(row, column);
}

//Static
const mat44 mat44::Identity = mat44(1, 0, 0, 0,
                                       0, 1, 0, 0,
                                       0, 0, 1, 0,
                                       0, 0, 0, 1);

} //end namespace
