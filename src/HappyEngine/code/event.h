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

#ifndef _HE_EVENT_H_
#define _HE_EVENT_H_
#pragma once

#include "boost/function.hpp"
#include <vector>

namespace he {

template<typename returnType>
class event
{
private: 
    typedef boost::function<returnType()> function;

public:
    event() {}
    ~event() {}

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

    //Disable default copy constructor and default assignment operator
    event(const event&);
    event& operator=(const event&);
};

} //end namespace

#endif
