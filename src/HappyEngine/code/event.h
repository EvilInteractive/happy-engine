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
bool defaultEventCombiner(T& inoutA, const T& inB)
{
    inoutA = inB;
    return false;
}

}

template<typename T>
class EventCombiner
{
public:
    // return true if it has to return immediately without validating others
    typedef boost::function2<bool, T&, const T&> EventCombinerType;
    typedef T result_type;

    EventCombiner():
    m_EventCombiner(&details::defaultEventCombiner<T>),
    m_DefaultValue(T()) {}
    EventCombiner(
        const EventCombinerType& combiner,
        const T& defaultValue):
      m_DefaultValue(defaultValue),
      m_EventCombiner(combiner) {}

    template<typename InputIterator>
    T operator()(InputIterator first, InputIterator last) const
    {
        if (first == last)
            return m_DefaultValue;

        T outValue(*first);
        while (++first != last && !m_EventCombiner(outValue, *first));

        return outValue;
    }

private:
    EventCombinerType m_EventCombiner;
    T m_DefaultValue;
};

template<>
class EventCombiner<void>
{
public:
    // return true if it has to return immediately without validating others
    typedef void result_type;

    EventCombiner() {}

    template<typename InputIterator>
    void operator()(InputIterator first, InputIterator last) const
    {
        while (++first != last)
            *first;

        return;
    }
};


//////////////////////////////////////////////////////////////////////////
// event0
//////////////////////////////////////////////////////////////////////////
template<typename ReturnType>
class event0;

template<typename ReturnType>
struct eventCallback0
{
    template<typename T> friend class event0;
    typedef boost::function0<ReturnType> Function;

public:
    eventCallback0() {}
    explicit eventCallback0(const Function& Function):
      m_Function(Function) {}

private:
    Function m_Function;
    boost::signals::connection m_Connection;
};

template<typename ReturnType>
class event0
{
    typedef eventCallback0<ReturnType> Function;
    typedef boost::signal0<ReturnType, EventCombiner<ReturnType>> Signal;
public:
    event0() {}
    event0(const EventCombiner<ReturnType>& combiner):
        m_Combiner(combiner) {}
    ~event0() {}

    void operator+=(Function& func)
    {
        func.m_Connection = m_Signal.connect(func.m_Function);
    }
    void operator-=(const Function& func)
    {
        m_Signal.disconnect(func.m_Connection);
    }
    ReturnType operator()() const
    {
        return m_Signal();
    }

    void clear()
    {
        m_Signal.disconnect_all_slots();
    }

private:
    Signal m_Signal;
    EventCombiner<ReturnType> m_Combiner;

    event0(const event0&);
    event0& operator=(const event0&);
};

//////////////////////////////////////////////////////////////////////////
// event0
//////////////////////////////////////////////////////////////////////////
template<typename ReturnType, typename Arg1Type>
class event1;

template<typename ReturnType, typename Arg1Type>
struct eventCallback1
{
    template<typename T, typename Arg1Type> friend class event1;
    typedef boost::function1<ReturnType, Arg1Type> Function;

public:
    eventCallback1() {}
    explicit eventCallback1(const Function& Function):
    m_Function(Function) {}

private:
    Function m_Function;
    boost::signals::connection m_Connection;
};

template<typename ReturnType, typename Arg1Type>
class event1
{
    typedef eventCallback1<ReturnType, Arg1Type> Function;
    typedef boost::signal1<ReturnType, Arg1Type, EventCombiner<ReturnType>> Signal;
public:
    event1() {}
    event1(const EventCombiner<ReturnType>& combiner):
    m_Combiner(combiner) {}
    ~event1() {}

    void operator+=(Function& func)
    {
        func.m_Connection = m_Signal.connect(func.m_Function);
    }
    void operator-=(const Function& func)
    {
        m_Signal.disconnect(func.m_Connection);
    }
    ReturnType operator()() const
    {
        return m_Signal();
    }

    void clear()
    {
        m_Signal.disconnect_all_slots();
    }

private:
    Signal m_Signal;
    EventCombiner<ReturnType> m_Combiner;

    event1(const event1&);
    event1& operator=(const event1&);
};

} //end namespace

#endif
