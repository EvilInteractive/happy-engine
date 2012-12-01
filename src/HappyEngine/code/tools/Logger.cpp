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
//Created: 26/05/2012
#include "HappyPCH.h" 

#include "Logger.h"

namespace he {
namespace tools {

Logger* Logger::s_Instance = nullptr;

Logger::Logger()
{
    boost::posix_time::ptime now(boost::posix_time::second_clock::local_time());

    std::ofstream output;
    output.open("log.log", std::ios_base::app);
    output << "\n";
    output << "-------------------------------------------------------------------\n";
    output << "-- Log started @" << boost::posix_time::to_simple_string(now) << "\n";
    output << "-------------------------------------------------------------------\n";
    output.close();
}


Logger::~Logger()
{
}

void Logger::log( const LogType type, const char* file, const char* func, int line, const char* str, ... )
{
    va_list argList;
    va_start(argList, str);
    log(type, file, func, line, str, argList);
    va_end(argList);
}

void Logger::log( const LogType type, const char* file, const char* func, int line, const char* str, const va_list& argList )
{
    char buff[1024];
    memset(buff, 0, 1024);
    vsnprintf(buff, 1024, str, argList);

    std::string typeString("");
    CMSG_TYPE consoleType(CMSG_TYPE_ENGINE);
    switch(type)
    {
        case LogType_ProgrammerAssert:  
        {
            typeString = "Programmer Assert"; 
            consoleType = CMSG_TYPE_ERROR;
            he::err::details::happyAssert(err::details::AssertType_Code, file, func, line, buff);
        } break;
        case LogType_ArtAssert:
        {
            typeString = "Art Assert"; 
            consoleType = CMSG_TYPE_ERROR;
            he::err::details::happyAssert(err::details::AssertType_Art, file, func, line, buff);
        } break;
        case LogType_Error:
            {
            typeString = "Error"; 
            consoleType = CMSG_TYPE_ERROR;
        } break;
        case LogType_Warning:
        {
            typeString = "Warning"; 
            consoleType = CMSG_TYPE_WARNING;
        } break;
        case LogType_Info:
        {
            typeString = "Info"; 
            consoleType = CMSG_TYPE_INFO;
        } break;
    }

    m_Mutex.lock();

    std::ofstream output;
    output.open("log.log", std::ios_base::app);
    if (output.is_open())
    {
        output << typeString << ": " << buff << "\n";
        output.close();
    }

    std::cout << typeString << ": " << buff << "\n";
    if (HAPPYENGINE != nullptr && CONSOLE != nullptr) 
    {
        CONSOLE->addMessage(buff, consoleType);
    }

    m_Mutex.unlock();
}

void Logger::sdmInit()
{
    HE_ASSERT(s_Instance == nullptr, "Logger is already initialized!");
    s_Instance = NEW Logger();
}

void Logger::sdmDestroy()
{
    delete s_Instance;
    s_Instance = nullptr;
}

Logger* Logger::getInstance()
{
    return s_Instance;
}

} } //end namespace
