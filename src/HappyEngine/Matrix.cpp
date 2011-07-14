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

#include "Matrix.h"
#include "MathFunctions.h"

namespace happyengine {
namespace math {

Matrix::Matrix(): m_Matrix(physx::pubfnd3::PxVec4(1.0f, 1.0f, 1.0f, 1.0f))
{
}
Matrix::Matrix(physx::pubfnd3::PxMat44 mat): m_Matrix(mat)
{
}
Matrix::Matrix( float _00, float _01, float _02, float _03,
                float _10, float _11, float _12, float _13,
                float _20, float _21, float _22, float _23,
                float _30, float _31, float _32, float _33 ) : 
    m_Matrix(physx::pubfnd3::PxVec4(_00, _10, _20, _30),
             physx::pubfnd3::PxVec4(_01, _11, _21, _31),
             physx::pubfnd3::PxVec4(_02, _12, _22, _32),
             physx::pubfnd3::PxVec4(_03, _13, _23, _33))
{
}
Matrix::~Matrix()
{
}

//static constructors
Matrix Matrix::createPerspectiveRH(float fov, float viewWidth, float viewHeight, float zNear, float zFar)
{
    float _11 = cosf(fov * 0.5f) / sinf(fov * 0.5f);
    float _00 = _11 * viewHeight / viewWidth;
    return Matrix(
         _00, 0.0f, 0.0f                            , 0.0f,
        0.0f,  _11, 0.0f                            , 0.0f,
        0.0f, 0.0f, (zFar) / (zFar - zNear)         , (zFar * zNear) / (zFar - zNear),
        0.0f, 0.0f, -1.0f                           , 0.0f);
}
Matrix Matrix::createPerspectiveLH(float fov, float viewWidth, float viewHeight, float zNear, float zFar)
{
    float _11 = cosf(fov * 0.5f) / sinf(fov * 0.5f);
    float _00 = _11 * viewHeight / viewWidth;
    return Matrix(
         _00, 0.0f, 0.0f                             , 0.0f,
        0.0f,  _11, 0.0f                             , 0.0f,
        0.0f, 0.0f, (zFar) / (zFar - zNear)          , -(zFar * zNear) / (zFar - zNear),
        0.0f, 0.0f, 1.0f                             , 0.0f);
}
Matrix Matrix::createOrthoLH(float left, float right, float top, float bottom, float zNear, float zFar)
{
    return Matrix(
        2.0f / (right - left),                    0.0f,                    0.0f,   (left + right) / (left - right),
                         0.0f,   2.0f / (top - bottom),                    0.0f,   (top + bottom) / (bottom - top),
                         0.0f,                    0.0f,   1.0f / (zFar - zNear),            zNear / (zNear - zFar),
                         0.0f,                    0.0f,                    0.0f,                             1.0f);
}
Matrix Matrix::createOrthoRH(float left, float right, float top, float bottom, float zNear, float zFar)
{
    return Matrix(
        2.0f / (right - left),                    0.0f,                    0.0f,   (left + right) / (left - right),
                         0.0f,   2.0f / (top - bottom),                    0.0f,   (top + bottom) / (bottom - top),
                         0.0f,                    0.0f,   1.0f / (zNear - zFar),            zNear / (zNear - zFar),
                         0.0f,                    0.0f,                    0.0f,                             1.0f);
}
Matrix Matrix::createLookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    Vector3 zaxis = normalize(target - eye);
    Vector3 xaxis = normalize(cross(normalize(up), zaxis));
    Vector3 yaxis = cross(zaxis, xaxis);

    return Matrix(
           xaxis.x,  xaxis.y,  xaxis.z, -dot(xaxis, eye),
           yaxis.x,  yaxis.y,  yaxis.z, -dot(yaxis, eye),
           zaxis.x,  zaxis.y,  zaxis.z, -dot(zaxis, eye),
              0.0f,     0.0f,     0.0f,            1.0f);     
}
Matrix Matrix::createLookAtRH(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    Vector3 zaxis = normalize(eye - target);
    Vector3 xaxis = normalize(cross(normalize(up), zaxis));
    Vector3 yaxis = cross(zaxis, xaxis);

    return Matrix(
           -xaxis.x,  -xaxis.y,  -xaxis.z,  dot(xaxis, eye),
            yaxis.x,   yaxis.y,   yaxis.z, -dot(yaxis, eye),
           -zaxis.x,  -zaxis.y,  -zaxis.z, -dot(zaxis, eye),
              0.0f,     0.0f,     0.0f,            1.0f);     
}
Matrix Matrix::createTranslation(const Vector3& translation)
{
    return Matrix(1.0f, 0.0f, 0.0f, translation.x,
                  0.0f, 1.0f, 0.0f, translation.y,
                  0.0f, 0.0f, 1.0f, translation.z,
                  0.0f, 0.0f, 0.0f, 1.0f);

}
Matrix Matrix::createRotation(const Vector3& axis, float radians)
{
    float cosAlpha(cosf(radians));
    float sinAlpha(sinf(radians));

    return Matrix(
        cosAlpha + sqr(axis.x) * (1 - cosAlpha),                    axis.x * axis.y * (1 - cosAlpha) - axis.z * sinAlpha,       axis.x * axis.z * (1 - cosAlpha) + axis.y * sinAlpha,       0.0f,
        axis.x * axis.y * (1 - cosAlpha) + axis.z * sinAlpha,       cosAlpha + sqr(axis.y) * (1 - cosAlpha),                    axis.x * axis.z * (1 - cosAlpha) - axis.y * sinAlpha,       0.0f,
        axis.x * axis.z * (1 - cosAlpha) - axis.y * sinAlpha,       axis.y * axis.z * (1 - cosAlpha) + axis.x * sinAlpha,       cosAlpha + sqr(axis.z) * (1 - cosAlpha),                    0.0f,
        0.0f,                                                       0.0f,                                                       0.0f,                                                       1.0f);
}
Matrix Matrix::createScale(const Vector3& scale)
{
    return Matrix(scale.x, 0.0f, 0.0f,    0.0f,
                  0.0f, scale.y, 0.0f,    0.0f,
                  0.0f,    0.0f, scale.z, 0.0f,
                  0.0f,    0.0f, 0.0f,    1.0f);
}
Matrix Matrix::createScale(float xScale, float yScale, float zScale)
{
    return Matrix(xScale,  0.0f,   0.0f,  0.0f,
                  0.0f,  yScale,   0.0f,  0.0f,
                  0.0f,    0.0f, zScale,  0.0f,
                  0.0f,    0.0f,   0.0f,  1.0f);
}
Matrix Matrix::createScale(float scale)
{
    return Matrix(scale,  0.0f,  0.0f,  0.0f,
                  0.0f,  scale,  0.0f,  0.0f,
                  0.0f,   0.0f, scale,  0.0f,
                  0.0f,   0.0f,  0.0f,  1.0f);
}

Matrix Matrix::operator*(const Matrix& mat)
{
    return Matrix(m_Matrix * mat.m_Matrix);
}
Vector3 Matrix::operator*(const Vector3& vec)
{
    return Vector3(m_Matrix.transform(physx::pubfnd3::PxVec3(vec.x, vec.y, vec.z)));
}

void Matrix::toFloatArray(float arr[16]) const
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

} } //end namespace
