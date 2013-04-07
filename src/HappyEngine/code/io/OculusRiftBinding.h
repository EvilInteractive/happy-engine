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
//Created: 2013/03/30

#ifndef _HE_OculusRiftBinding_H_
#define _HE_OculusRiftBinding_H_
#pragma once

namespace he {
namespace io {
class OculusAllocater;
class OculusLogger;
class OculusRiftBinding;

class HAPPY_ENTRY OculusRiftDevice
{
    friend class OculusRiftBinding;
    struct DeviceContext;
public:
    OculusRiftDevice();
    ~OculusRiftDevice();

    float getScreenWidth() const;
    float getScreenHeight() const;
    uint32 getScreenWidthResolution() const;
    uint32 getScreenHeightResolution() const;
    float getEyeToScreenDistance() const;
    float getLensSeparationDistance() const;
    float getInterpupillaryDistance() const;

private:
    DeviceContext* m_Context;
    
    //Disable default copy constructor and default assignment operator
    OculusRiftDevice(const OculusRiftDevice&);
    OculusRiftDevice& operator=(const OculusRiftDevice&);
};

class HAPPY_ENTRY OculusRiftBinding
{
    struct OculusContext;
public:
    OculusRiftBinding();
    ~OculusRiftBinding();

    void init();
    void shutdown();

    OculusRiftDevice* createDevice();
    void releaseDevice(OculusRiftDevice* const device);

    OculusRiftDevice* getDevice(const size_t index) const;
    
private:
    OculusAllocater* m_Allocater;
    OculusLogger* m_Logger;
    OculusContext* m_Context;

    he::PrimitiveList<OculusRiftDevice*> m_ConnectedDevices;

    //Disable default copy constructor and default assignment operator
    OculusRiftBinding(const OculusRiftBinding&);
    OculusRiftBinding& operator=(const OculusRiftBinding&);
};

} } //end namespace

#endif