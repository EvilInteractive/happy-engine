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

template<typename returnType>
class event0
{
private: 
    typedef boost::function0<returnType> function;

public:
    event0() {}
    ~event0() {}

    void operator+=(const function& func)
    {
        m_FuncList.push_back(func);
    }
    returnType operator()()
    {
        std::for_each(m_FuncList.cbegin(), m_FuncList.cend(), [](const function& func)
        {
            func();
        });
    }

    void clear()
    {
        m_FuncList.clear();
    }

private:
    std::vector<function> m_FuncList;
};

template<typename returnType, typename parameterType>
class event1
{
private: 
    typedef boost::function1<returnType, parameterType> function;

public:
    event1() {}
    ~event1() {}

    void operator+=(const function& func)
    {
        m_FuncList.push_back(func);
    }
    returnType operator()(parameterType par)
    {
        std::for_each(m_FuncList.cbegin(), m_FuncList.cend(), [&](const function& func)
        {
            func(par);
        });
    }

    void clear()
    {
        m_FuncList.clear();
    }

private:
    std::vector<function> m_FuncList;
};

template<typename returnType, typename parameterType1, typename parameterType2>
class event2
{
private: 
    typedef boost::function2<returnType, parameterType1, parameterType2> function;

public:
    event2() {}
    ~event2() {}

    void operator+=(const function& func)
    {
        m_FuncList.push_back(func);
    }
    returnType operator()(parameterType1 par, parameterType2 par2)
    {
        std::for_each(m_FuncList.cbegin(), m_FuncList.cend(), [&](const function& func)
        {
            func(par, par2);
        });
    }

    void clear()
    {
        m_FuncList.clear();
    }

private:
    std::vector<function> m_FuncList;
};

} //end namespace

#endif
