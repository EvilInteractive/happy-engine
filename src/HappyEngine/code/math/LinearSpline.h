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
//Author:  Bastian Damman
//Created: 08/09/2011

#ifndef _LINEAR_SPLINE_H_
#define _LINEAR_SPLINE_H_
#pragma once

#include "MathFunctions.h"

namespace he {

namespace splines {

template<typename T>
class LinearSpline : public ISpline<T>
{
public:
    LinearSpline(const T& p0, const T& p1) : m_P0(p0), m_P1(p1), m_DistanceTravelled(0), m_TotalDistance(arcLength(0, 1))
    {}
    virtual ISpline* clone() const
    {
        return NEW LinearSpline(*this);
    }
    virtual ~LinearSpline() {}

    virtual float arcLength(float t0, float t1) const
    {
        return length(getPoint(t1) - getPoint(t0));
    }

    virtual T getPoint(float t) const
    {
        return lerp(m_P0, m_P1, t);
    }

    virtual T move(float distance, float& realTravelledDistance)
    {
        if (m_DistanceTravelled + distance > m_TotalDistance)
        {
            realTravelledDistance = m_TotalDistance - m_DistanceTravelled;
            m_DistanceTravelled = m_TotalDistance;
            return getPoint(1.0f);
        }
        else
        {
            realTravelledDistance = distance;
            m_DistanceTravelled += distance;

            return lerp(m_P0, m_P1, m_DistanceTravelled / m_TotalDistance);
        }
    }

    virtual void reset()
    {
        m_DistanceTravelled = 0;
    }

private:
    T m_P0, m_P1;

    float m_DistanceTravelled;
    float m_TotalDistance;

    LinearSpline(const LinearSpline& other):
        m_P0(other.m_P0), m_P1(other.m_P1), 
        m_DistanceTravelled(other.m_DistanceTravelled), m_TotalDistance(other.m_TotalDistance)
    {
    }
    //Disable default assignment operator
    LinearSpline& operator=(const LinearSpline&);
};

} } //end namespace

#endif
