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

#ifndef _CATMULLROM_SPLINE_H_
#define _CATMULLROM_SPLINE_H_
#pragma once

#include <vector>
#include "MathFunctions.h"
#include "HappyTypes.h"
#include "ISpline.h"

namespace he {

namespace splines {

template<typename T>
class CatmullRomSpline : public ISpline<T>
{
public:
    static const int LOOKUPSIZE = 20;
    static const int ARCLENGTH_PRECISION = 400; //per 100 units ==> 400 = 4 iteration per unit

	CatmullRomSpline(const T& p0, const T& p1, const T& p2, const T& p3):
        m_DistanceTravelled(0.0f), m_PrevDistLookupIndex(0), m_TotalDistance(0.0f),
        m_P0(p0), m_P1(p1), m_P2(p2), m_P3(p3)
    {
        m_DistTrelation.reserve(LOOKUPSIZE + 1); //+0
        m_DistTrelation.push_back(std::make_pair(0.0f, 0.0f));
        for (int i = 1; i <= LOOKUPSIZE; ++i)
        {
            m_DistTrelation.push_back(std::make_pair(
                m_DistTrelation[i - 1].first + 
                arcLength((i - 1.0f)/LOOKUPSIZE, (float)i/LOOKUPSIZE), (float)i/LOOKUPSIZE));
        }
        m_TotalDistance = m_DistTrelation.back().first;
    }
    virtual ISpline* clone() const
    {
        return NEW CatmullRomSpline(*this);
    }
    virtual ~CatmullRomSpline() {}
    
    virtual float arcLength(float t0, float t1) const
    {
        float arclength(0);
        T p0(getPoint(t0));
        float approxLength(length(getPoint(t1) - p0));
        uint precision = static_cast<uint>(max<float>(approxLength * (ARCLENGTH_PRECISION / 100.0f), 2));

        for (uint i = 1; i <= precision; ++i)
        {
            T p1(getPoint(lerp(t0, t1, (float)i / precision)));
            arclength += length(p1 - p0);
            p0 = p1;
        }

        return arclength;
    }

    virtual T getPoint(float t) const
    {
        return catmullrom(m_P0, m_P1, m_P2, m_P3, t);
    }

    virtual T move(float distance, float& realTravelledDistance)
    {
        float nextDistance = m_DistanceTravelled + distance;
        if (nextDistance >= m_TotalDistance)
        {
            realTravelledDistance = m_TotalDistance - m_DistanceTravelled;
            m_DistanceTravelled = m_TotalDistance;
            return getPoint(1.0f);
        }
        else
        {
            for(;;)
            {
                ASSERT(m_PrevDistLookupIndex < m_DistTrelation.size(), "should've been catched by prev. if...");
                if (nextDistance < m_DistTrelation[m_PrevDistLookupIndex].first)
                {
                    float prev(m_DistTrelation[m_PrevDistLookupIndex - 1].first);
                    float next(m_DistTrelation[m_PrevDistLookupIndex + 0].first);
                    float interpolate( (nextDistance - prev) / (next - prev) );
                    float nextT( lerp(m_DistTrelation[m_PrevDistLookupIndex - 1].second, m_DistTrelation[m_PrevDistLookupIndex].second, interpolate) );
                    realTravelledDistance = distance;
                    m_DistanceTravelled += realTravelledDistance;
                    return getPoint(nextT);
                }
                else
                {
                    ++m_PrevDistLookupIndex;
                }
            }
        }
    }

    virtual void reset()
    {
        m_DistanceTravelled = 0;
        m_PrevDistLookupIndex = 0;
    }

private:

    T m_P0, m_P1, m_P2, m_P3;

    float m_DistanceTravelled;
    uint m_PrevDistLookupIndex;

    float m_TotalDistance;

    std::vector<std::pair<float, float>> m_DistTrelation;
    
    CatmullRomSpline(const CatmullRomSpline& other):        
        m_P0(other.m_P0), m_P1(other.m_P1),  
        m_P2(other.m_P2), m_P3(other.m_P3), 
        m_DistanceTravelled(other.m_DistanceTravelled),
        m_PrevDistLookupIndex(other.m_PrevDistLookupIndex),
        m_TotalDistance(other.m_TotalDistance),
        m_DistTrelation(other.m_DistTrelation)
    {
    }
    CatmullRomSpline& operator=(const CatmullRomSpline& other);
};

} } //end namespace

#endif
