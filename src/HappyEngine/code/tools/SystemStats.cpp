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
//Author:  Sebastiaan Sprengers
//Created: 8/12/2012

#include "HappyPCH.h" 

#include "SystemStats.h"

#ifndef HE_WINDOWS
    
#else
    #include "windows.h"
    #include "psapi.h"

    #pragma comment(lib, "psapi.lib")
#endif

namespace he {
namespace tools {

/* CONSTRUCTOR - DESTRUCTOR */
SystemStats::SystemStats()
{
#ifdef HE_WINDOWS
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo(&sysInfo);
    m_NumProcessors = checked_numcast<uint8>(sysInfo.dwNumberOfProcessors);

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&m_LastCPU, &ftime, sizeof(FILETIME));

    m_Self = GetCurrentProcess();
    GetProcessTimes(m_Self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&m_LastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&m_LastUserCPU, &fuser, sizeof(FILETIME)); 
#else
    m_NumProcessors = checked_numcast<uint8>(sysconf( _SC_NPROCESSORS_ONLN ));
#endif
}

SystemStats::~SystemStats()
{
}

/* GETTERS */
uint32 SystemStats::getTotalVirtualMemory() const
{
    #ifndef HE_WINDOWS
    return 0;
    #else
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        uint32 totalVirtualMem((uint32)memInfo.ullTotalPageFile);

        return totalVirtualMem;
    #endif
}
uint32 SystemStats::getVirtualMemoryUsed() const
{
    #ifndef HE_WINDOWS
    return 0;
    #else
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)(&pmc), sizeof(pmc));
        uint32 virtualMemUsedByMe((uint32)pmc.PrivateUsage);

        return virtualMemUsedByMe;
    #endif
}
uint32 SystemStats::getTotalMemory() const
{
    #ifndef HE_WINDOWS
    return 0;
    #else
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        uint32 totalPhysMem((uint32)memInfo.ullTotalPhys);

        return totalPhysMem;
    #endif
}
uint32 SystemStats::getMemoryUsed() const
{
    #ifndef HE_WINDOWS
    return 0;
    #else
        PROCESS_MEMORY_COUNTERS pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
        uint32 physMemUsedByMe((uint32)pmc.WorkingSetSize);

        return physMemUsedByMe;
    #endif
}
float SystemStats::getCpuUsage()
{
    #ifndef HE_WINDOWS
    return 0;
    #else
        FILETIME ftime, fsys, fuser;
        ULARGE_INTEGER now, sys, user;
        float percent;
    
        GetSystemTimeAsFileTime(&ftime);
        memcpy(&now, &ftime, sizeof(FILETIME));

        GetProcessTimes(m_Self, &ftime, &ftime, &fsys, &fuser);
        memcpy(&sys, &fsys, sizeof(FILETIME));
        memcpy(&user, &fuser, sizeof(FILETIME));
        percent = (float)((sys.QuadPart - m_LastSysCPU.QuadPart) +
            (user.QuadPart - m_LastUserCPU.QuadPart));
        percent /= (now.QuadPart - m_LastCPU.QuadPart);
        percent /= m_NumProcessors;
        m_LastCPU = now;
        m_LastUserCPU = user;
        m_LastSysCPU = sys;
    

        return (percent * 100.0f);
    #endif
}

}} //end namespace
