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
//Author:  Bastian Damman
//Created: 27/11/2011
//Extended: Sebastiaan Sprengers

#ifndef _HE_EVENT_H_
#define _HE_EVENT_H_
#pragma once

namespace he {

namespace details {

template<typename T>
void defaultEventCombiner(T& inoutA, const T& inB)
{
    inoutA = inB;
}

template<typename T>
class EventCombiner
{
public:
    typedef boost::function2<void, T&, const T&> EventCombinerType;
    typedef T result_type;

    EventCombiner(
        const EventCombinerType& combiner = &defaultEventCombiner<T>,
        const T& defaultValue = T()):
      m_DefaultValue(defaultValue),
      m_EventCombiner(combiner) {}

    template<typename InputIterator>
    T operator()(InputIterator first, InputIterator last) const
    {
        if (first == last)
            return m_DefaultValue;

        T outValue(*first);
        while (++first != last) 
        {
            m_EventCombiner(outValue, *first);
        }

        return outValue;
    }

private:
    EventCombinerType m_EventCombiner;
    T m_DefaultValue;
};

}


template<typename ReturnType>
struct eventCallback0
{
    friend event0<ReturnType>;
    typedef boost::function0<ReturnType> Function;

public:
    explicit eventCallback0(const Function& function):
      m_Function(function) {}

private:
    boost::function0<ReturnType> m_Function;
    boost::signals::connection m_Connection;
};

template<typename returnType>
class event0
{
    typedef eventCallback0<returnType> function;
    typedef typename details::EventCombiner<returnType>::EventCombinerType EventCombinerType;
    typedef boost::signal0<returnType, details::EventCombiner<returnType>> Signal;
public:
    event0() {}
    event0(const EventCombinerType& combiner, const returnType& defaultValue):
        m_Combiner(combiner, defaultValue) {}
    ~event0() {}

    void operator+=(function& func)
    {
        func.m_Connection = m_Signal.connect(func.m_Function);
    }
    void operator-=(const function& func)
    {
        m_Signal.disconnect(func.m_Connection);
    }
    returnType operator()() const
    {
        return m_Signal.signal();
    }

    void clear()
    {
        m_Signal.disconnect_all_slots();
    }

private:
    Signal m_Signal;
    details::EventCombiner<returnType> m_Combiner;
};

} //end namespace

#endif
