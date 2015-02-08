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
//Created: 2013/03/09
#include "HappyPCH.h" 

#include "Timer.h"

namespace he {

Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::start()
{
    m_StartTime = std::chrono::high_resolution_clock::now();
    m_IsRunning = true;
}

void Timer::stop()
{
    m_StopTime = std::chrono::high_resolution_clock::now();
    m_IsRunning = false;
}

void Timer::restart()
{
    m_StartTime = std::chrono::high_resolution_clock::now();
    m_IsRunning = true;
}

size_t Timer::getElapsedNanoseconds()
{
    if (m_IsRunning)
    {
        m_StopTime = std::chrono::high_resolution_clock::now();        
    }

    std::chrono::high_resolution_clock::duration elapsedTime(std::chrono::high_resolution_clock::now() - m_StartTime);
    return static_cast<size_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsedTime).count());
}

size_t Timer::getElapsedMilliseconds()
{
    if (m_IsRunning)
    {
        m_StopTime = std::chrono::high_resolution_clock::now();
    }

    std::chrono::high_resolution_clock::duration elapsedTime(std::chrono::high_resolution_clock::now() - m_StartTime);
    return static_cast<size_t>(std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count());
}

double Timer::getElapsedSeconds()
{
    if (m_IsRunning)
    {
        m_StopTime = std::chrono::high_resolution_clock::now();        
    }

    std::chrono::duration<double> elapsedTime(std::chrono::high_resolution_clock::now() - m_StartTime);
    return elapsedTime.count();
}

float Timer::getElapsedSecondsF()
{
    if (m_IsRunning)
    {
        m_StopTime = std::chrono::high_resolution_clock::now();        
    }

    std::chrono::duration<float> elapsedTime(std::chrono::high_resolution_clock::now() - m_StartTime);
    return elapsedTime.count();
}

} //end namespace
