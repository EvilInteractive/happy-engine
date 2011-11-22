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
#include "StdAfx.h" 

#include "Profiler.h"
#include "HappyNew.h"
#include "Happy2DRenderer.h"
#include "Text.h"

namespace he {
namespace tools {

Profiler* Profiler::s_Profiler = nullptr;

Profiler::Profiler()
{
    GUI->createLayer("profiler", 1);
}


Profiler::~Profiler()
{
}

Profiler* Profiler::getInstance()
{
    if (s_Profiler == nullptr)
        s_Profiler = NEW Profiler();
    return s_Profiler;
}

void Profiler::dispose()
{
    delete s_Profiler;
    s_Profiler = nullptr;
}

void Profiler::begin( const std::string& name )
{
    if (m_Data.find(name) == m_Data.cend())
        m_Data[name] = std::deque<ProfileData>();
    if (m_Data[name].size() == MAX_DATA)
        m_Data[name].pop_front();
    m_Data[name].push_back(ProfileData());
    m_Data[name].back().startTime = boost::chrono::high_resolution_clock::now();
}

void Profiler::end( const std::string& name )
{
    m_Data[name].back().endTime = boost::chrono::high_resolution_clock::now();
}

void Profiler::draw() const
{
    GUI->setLayer("profiler");
    gui::Text text;
    text.addLine("----PROFILER------------------------------");
    std::for_each(m_Data.cbegin(), m_Data.cend(), [&](const std::pair<std::string, std::deque<ProfileData>>& profilePair)
    {
        double avgTime(0.0);
        std::for_each(profilePair.second.cbegin(), profilePair.second.cend(), [&avgTime](const ProfileData& data)
        {
            avgTime += data.getDuration() * 1000;
        });
        char buff[14];
        std::sprintf(buff, ": %07.2f ms\0", avgTime / profilePair.second.size());
        text.addLine("-- " + profilePair.first + std::string(buff));        
    });
    text.addLine("------------------------------------------");

    GUI->drawText(text, vec2(12, 128));

    GUI->setLayer();     
}

} } //end namespace