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
#include "mat44.h"

namespace he {

/* CONSTRUCTOR - DESTRUCTOR */
mat33::mat33() :	m_Matrix(physx::PxMat33::createIdentity())
{
}

mat33::mat33(const physx::PxMat33& mat) :	m_Matrix(mat)
{
}

mat33::mat33( float _00, float _01, float _02,
                    float _10, float _11, float _12,
                    float _20, float _21, float _22 ) :	
    m_Matrix(physx::PxVec3(_00, _10, _20),
             physx::PxVec3(_01, _11, _21),
             physx::PxVec3(_02, _12, _22))
{
}

mat33::~mat33()
{
}

/* STATIC CONSTRUCTORS */
mat33 mat33::createTranslation2D(const vec2& translation)
{
    return mat33(1.0f, 0.0f, translation.x,
                    0.0f, 1.0f, translation.y,
                    0.0f, 0.0f, 1.0f );
}

mat33 mat33::createRotation2D(const float radians)
{
    return mat33(cosf(radians), -sinf(radians), 0.0f,
                 sinf(radians), cosf(radians), 0.0f,
                 0.0f, 0.0f, 1.0f );
}

mat33 mat33::createScale2D(const vec2& scale)
{
    return mat33(scale.x, 0.0f, 0.0f,
                 0.0f, scale.y, 0.0f,
                 0.0f, 0.0f, 1.0f);
}

/* OPERATORS */
mat33 mat33::operator*(const mat33& mat) const
{
    return mat33(m_Matrix * mat.m_Matrix);
}

vec2 mat33::operator*(const vec2& vec) const
{
    physx::PxVec3 pxVec(m_Matrix.transform(physx::PxVec3(vec.x, vec.y, 1.0f)));
    return vec2(pxVec.x, pxVec.y);
}
vec3 mat33::operator*(const vec3& vec) const
{
    physx::PxVec3 pxVec(m_Matrix.transform(physx::PxVec3(vec.x, vec.y, vec.z)));
    return vec3(pxVec.x, pxVec.y, pxVec.z);
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

bool mat33::operator==( const mat33& other ) const
{
    return  m_Matrix.column0 == other.m_Matrix.column0 && 
            m_Matrix.column1 == other.m_Matrix.column1 &&
            m_Matrix.column2 == other.m_Matrix.column2;
}

bool mat33::operator!=( const mat33& other ) const
{
    return  m_Matrix.column0 != other.m_Matrix.column0 || 
            m_Matrix.column1 != other.m_Matrix.column1 ||
            m_Matrix.column2 != other.m_Matrix.column2;
}

float mat33::operator()( int row, int column ) const
{
    return m_Matrix(row, column);
}

float& mat33::operator()( int row, int column )
{
    return m_Matrix(row, column);
}

he::mat33 mat33::createRotation3D( const vec3& axis, float radians )
{
    float cosAlpha(cosf(radians));
    float sinAlpha(sinf(radians));

    return mat33(
        cosAlpha + sqr(axis.x) * (1 - cosAlpha),                    axis.x * axis.y * (1 - cosAlpha) - axis.z * sinAlpha,       axis.x * axis.z * (1 - cosAlpha) + axis.y * sinAlpha,
        axis.x * axis.y * (1 - cosAlpha) + axis.z * sinAlpha,       cosAlpha + sqr(axis.y) * (1 - cosAlpha),                    axis.y * axis.z * (1 - cosAlpha) - axis.x * sinAlpha,
        axis.x * axis.z * (1 - cosAlpha) - axis.y * sinAlpha,       axis.y * axis.z * (1 - cosAlpha) + axis.x * sinAlpha,       cosAlpha + sqr(axis.z) * (1 - cosAlpha));

}

he::mat33 mat33::createRotation3D( const vec3& forward, const vec3& up, const vec3& right )
{
    return mat33(
        right.x,        up.x,           forward.x,
        right.y,        up.y,           forward.y,
        right.z,        up.z,           forward.z);
}

he::mat33 mat33::createRotation3D( const vec3& eulerAngle )
{
    return createRotation3D(vec3::forward, eulerAngle.z) * createRotation3D(vec3::up, eulerAngle.y) * createRotation3D(vec3::right, eulerAngle.x);
}

const vec3& mat33::getForward() const
{
    return *reinterpret_cast<const vec3*>(&m_Matrix.column2);
}

const vec3& mat33::getUp() const
{
    return *reinterpret_cast<const vec3*>(&m_Matrix.column1);
}

const vec3& mat33::getRight() const
{
    return *reinterpret_cast<const vec3*>(&m_Matrix.column0);
}

he::vec3 mat33::getEulerAngles() const
{
    const float m20(m_Matrix(2, 0));
    if(1.0f - fabs(m20) <= FLT_EPSILON)
    {
        vec3 result(0, 0, 0); // pitch, yaw, roll
        if(m20 < 0.f)
        {
            result.y = piOverTwo;
            result.x = atan2f(m_Matrix(0, 1), m_Matrix(0, 2));
        }
        else
        {
            result.y = -piOverTwo;
            result.x = atan2f(-m_Matrix(0, 1), m_Matrix(0, 2));
        }
        return result;
    }
    else
    {
        vec3 result(0, 0, 0); // pitch, yaw, roll
        vec3 result2(0, 0, 0);
        result.y = asinf(-m20);
        result2.y = pi - result.y;

        const float invCosYaw(1.0f / cos(result.y));
        result.x = atan2f(m_Matrix(2, 1) * invCosYaw, m_Matrix(2, 2) * invCosYaw);
        result.z = atan2f(m_Matrix(1, 0) * invCosYaw, m_Matrix(0, 0) * invCosYaw);

        const float invCosYaw2(1.0f / cos(result2.y));
        result2.x = atan2f(m_Matrix(2, 1) * invCosYaw2, m_Matrix(2, 2) * invCosYaw2);
        result2.z = atan2f(m_Matrix(1, 0) * invCosYaw2, m_Matrix(0, 0) * invCosYaw2);

        // minimize angle sum, apparently this works very well in practice.
        if (lengthSqr(result2) < lengthSqr(result))
        {
            return result2;
        }
        else
        {
            return result;
        }
    }
}

/* STATIC */
const mat33 mat33::Identity = mat33(1.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f);

} //end namespace