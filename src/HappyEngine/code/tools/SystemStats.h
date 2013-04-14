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

#ifndef _HE_SYSTEMSTATS_H_
#define _HE_SYSTEMSTATS_H_
#pragma once

#include "Singleton.h"

namespace he {
namespace tools {

class SystemStats : public Singleton<SystemStats>
{
public:

    friend Singleton<SystemStats>;

    /* DESTRUCTOR */
    virtual ~SystemStats();
    
    /* GETTERS */
    uint32 getTotalVirtualMemory() const;
    uint32 getVirtualMemoryUsed() const;

    uint32 getTotalMemory() const;
    uint32 getMemoryUsed() const;

    float getCpuUsage();

    uint8 getNumCores() const { return m_NumProcessors; }

private:

    /* CONSTRUCTOR */
    SystemStats();

    /* MEMBERS */

#ifdef HE_WINDOWS
        ULARGE_INTEGER m_LastCPU;
        ULARGE_INTEGER m_LastSysCPU;
        ULARGE_INTEGER m_LastUserCPU;

        uint8 m_NumProcessors;
        HANDLE m_Self;
#else
        uint8 m_NumProcessors;
#endif

    /* DEFAULT COPY & ASSIGNMENT */
    SystemStats(const SystemStats&);
    SystemStats& operator=(const SystemStats&);
};

}} //end namespace

#endif
