//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
//Created: 22/06/2012

#ifndef _HE_StopWatch_H_
#define _HE_StopWatch_H_
#pragma once

namespace he {
namespace tools {

class StopWatch
{
public:
    StopWatch();
    virtual ~StopWatch();

    void start(); // auto resets
    void stop();
    
    float getRunTime() const;

private:

    std::chrono::high_resolution_clock::time_point m_StartTime;
    std::chrono::high_resolution_clock::time_point m_StopTime;
    bool m_IsRunning;

    //Disable default copy constructor and default assignment operator
    StopWatch(const StopWatch&);
    StopWatch& operator=(const StopWatch&);
};

} } //end namespace

#endif
