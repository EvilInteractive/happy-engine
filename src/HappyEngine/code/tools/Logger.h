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

#ifndef _HE_Logger_H_
#define _HE_Logger_H_
#pragma once

namespace he {
enum LogType
{
    LogType_ProgrammerAssert,
    LogType_ArtAssert,
    LogType_Error,
    LogType_Warning,
    LogType_Info
};
namespace io {
    class BinaryVisitor;
}
namespace tools {

class Logger
{
public:
    //////////////////////////////////////////////////////////////////////////
    ///  Singleton -- singleton is implemented here because we can't use the Singleton base class because it uses HE_ASSERT
    //////////////////////////////////////////////////////////////////////////
    static void sdmInit();
    static void sdmDestroy();
    static void sdmVisit(he::io::BinaryVisitor& visitor);
    static Logger* getInstance()
    {
        return s_Instance;
    }

    //////////////////////////////////////////////////////////////////////////
    ///  Logger
    //////////////////////////////////////////////////////////////////////////
    ~Logger();

    void log(const LogType type, const char* file, const char* func, int line, const char* str, ...);
    void log(const LogType type, const char* file, const char* func, int line, const char* str, va_list& argList);
    
private:
    static Logger* s_Instance;
    
    Logger();

    boost::mutex m_Mutex;

    //Disable default copy constructor and default assignment operator
    Logger(const Logger&);
    Logger& operator=(const Logger&);
};

#define LOG(type, message, ...) he::tools::Logger::getInstance()->log(type, __FILE__, __HE_FUNCTION__, __LINE__, message, ##__VA_ARGS__)

} } //end namespace

#endif
