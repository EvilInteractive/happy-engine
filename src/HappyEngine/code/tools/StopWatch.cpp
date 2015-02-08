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
#include "HappyPCH.h" 

#include "StopWatch.h"

namespace he {
namespace tools {

StopWatch::StopWatch(): 
    m_IsRunning(false), 
    m_StartTime(std::chrono::high_resolution_clock::now()),
    m_StopTime(std::chrono::high_resolution_clock::now())

{
}


StopWatch::~StopWatch()
{
}

void StopWatch::start()
{
    m_StartTime = std::chrono::high_resolution_clock::now();
    m_StopTime = std::chrono::high_resolution_clock::now();
    m_IsRunning = true;
}

void StopWatch::stop()
{
    m_StopTime = std::chrono::high_resolution_clock::now();
    m_IsRunning = false;
}

float StopWatch::getRunTime() const
{
    std::chrono::high_resolution_clock::time_point endTime(m_StopTime);
    if (m_IsRunning == true)
    {
        endTime = std::chrono::high_resolution_clock::now();
    }

    std::chrono::duration<float> elapsedTime(endTime - m_StartTime);
    return elapsedTime.count();
}


} } //end namespace
