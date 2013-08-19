//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 2013/03/09

#ifndef _HE_Timer_H_
#define _HE_Timer_H_
#pragma once

namespace he {

class HAPPY_ENTRY Timer
{
public:
    Timer();
    ~Timer();

    void start();
    void stop();
    void restart();

    size_t getElapsedNanoseconds();
    size_t getElapsedMilliseconds();
    double getElapsedSeconds();
    float getElapsedSecondsF();

    bool isRunning() const { return m_IsRunning; }

private:

    bool m_IsRunning;
    boost::chrono::high_resolution_clock::time_point m_StartTime;
    boost::chrono::high_resolution_clock::time_point m_StopTime;

    //Disable default copy constructor and default assignment operator
    Timer(const Timer&);
    Timer& operator=(const Timer&);
};

} //end namespace

#endif
