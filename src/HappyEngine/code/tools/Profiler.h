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
//Created: 21/11/2011

#ifndef _HE_PROFILER_H_
#define _HE_PROFILER_H_
#pragma once

#include <map>
#include <deque>
#include "boost/chrono.hpp"

namespace he {
namespace tools {

struct ProfileData
{
    boost::chrono::high_resolution_clock::time_point startTime;
    boost::chrono::high_resolution_clock::time_point endTime;

    double getDuration() const
    {
        boost::chrono::high_resolution_clock::duration elapsedTime(endTime - startTime);
        return elapsedTime.count() / static_cast<double>(boost::nano::den);
    }
};

class Profiler
{
public:
    static Profiler* getInstance();
    static void dispose();

    void begin(const std::string& name);
    void end(const std::string& name);

    void draw() const;
    
private:
    Profiler();
    virtual ~Profiler();

    static Profiler* s_Profiler;

    static const int MAX_DATA = 50;
    std::map<std::string, std::deque<ProfileData>> m_Data;

    //Disable default copy constructor and default assignment operator
    Profiler(const Profiler&);
    Profiler& operator=(const Profiler&);
};
#define PROFILER he::tools::Profiler::getInstance()

#define PROFILER_BEGIN(name) PROFILER->begin(name);
#define PROFILER_END(name) PROFILER->end(name);

} } //end namespace

#endif
