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
//Created: 27/11/2011

#ifndef _HE_FX_CURVE_H_
#define _HE_FX_CURVE_H_
#pragma once

#include "IFxVariable.h"
#include <vector>

namespace he {
namespace gfx {

template<typename T>
class FxCurve : public IFxVariable<T>
{
private:
    struct CurvePoint
    {
        float m_NormTime;
        T m_Value;

        CurvePoint(float normtime, const T& value): m_NormTime(normtime), m_Value(value)
        {}
    };

public:
    FxCurve() {}
    virtual ~FxCurve() {}

    typedef boost::shared_ptr<FxCurve<T>> pointer;
    //////////////////////////////////////////////////////////////////////////
    ///                             IFxVariable                            ///
    //////////////////////////////////////////////////////////////////////////
    virtual T getValue(float normTime) const
    {
        std::vector<CurvePoint>::const_iterator it(m_Points.cbegin());
        for (; it != m_Points.cend(); ++it)
        {
            if (it->m_NormTime > normTime)
            {
                const CurvePoint& endPoint(*it);
                if (it != m_Points.cbegin())
                {
                    const CurvePoint& beginPoint(*(it - 1));
                    return lerp(beginPoint.m_Value, endPoint.m_Value, (normTime - beginPoint.m_NormTime) / (endPoint.m_NormTime - beginPoint.m_NormTime));
                }
                else
                {
                    return endPoint.m_Value;
                }
            }
        }
        return m_Points.size() > 0? m_Points.back().m_Value : T();
    }

    //////////////////////////////////////////////////////////////////////////
    ///                             Properties                             ///
    //////////////////////////////////////////////////////////////////////////
    void addPoint(float normTime, const T& value) //normTime 0->1
    {
        m_Points.push_back(CurvePoint(normTime, value));
    }
    void finalize()
    {
        std::sort(m_Points.begin(), m_Points.end(), [](const CurvePoint& a, const CurvePoint& b)
        {
            return a.m_NormTime < b.m_NormTime;
        });
    }

private:

    std::vector<CurvePoint> m_Points;

    //Disable default copy constructor and default assignment operator
    FxCurve(const FxCurve&);
    FxCurve& operator=(const FxCurve&);
};

} } //end namespace

#endif
