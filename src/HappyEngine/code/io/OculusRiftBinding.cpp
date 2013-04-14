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
#include "HappyPCH.h" 

#include "OculusRiftBinding.h"

#include <OVR.h>

namespace he {
namespace io {

struct OculusRiftDevice::DeviceContext
{
    DeviceContext()
    {
        // Fake values -- testin
        m_Info.HResolution            = 1280;
        m_Info.VResolution            = 800;
        m_Info.HScreenSize            = 0.14976f;
        m_Info.VScreenSize            = m_Info.HScreenSize / (m_Info.HResolution / static_cast<float>(m_Info.VResolution));
        m_Info.InterpupillaryDistance = 0.064f;
        m_Info.LensSeparationDistance = 0.064f;
        m_Info.EyeToScreenDistance    = 0.041f;
        /*m_Info.DistortionK[0]         = Distortion.K[0];
        m_Info.DistortionK[1]         = Distortion.K[1];
        m_Info.DistortionK[2]         = Distortion.K[2];
        m_Info.DistortionK[3]         = 0;*/
    }
    OVR::Ptr<OVR::HMDDevice> m_Device;
    OVR::Ptr<OVR::SensorDevice> m_Sensor;
    OVR::HMDInfo m_Info;
};

OculusRiftDevice::OculusRiftDevice()
    : m_Context(NEW DeviceContext())
{

}
OculusRiftDevice::~OculusRiftDevice()
{
    delete m_Context;
}

float OculusRiftDevice::getScreenWidth() const
{
    return m_Context->m_Info.HScreenSize;
}
float OculusRiftDevice::getScreenHeight() const
{
    return m_Context->m_Info.VScreenSize;
}
uint32 OculusRiftDevice::getScreenWidthResolution() const
{
    return m_Context->m_Info.HResolution;
}
uint32 OculusRiftDevice::getScreenHeightResolution() const
{
    return m_Context->m_Info.VResolution;
}
float OculusRiftDevice::getEyeToScreenDistance() const
{
    return m_Context->m_Info.EyeToScreenDistance;
}
float OculusRiftDevice::getLensSeparationDistance() const
{
    return m_Context->m_Info.LensSeparationDistance;
}
float OculusRiftDevice::getInterpupillaryDistance() const
{
    return m_Context->m_Info.InterpupillaryDistance;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class OculusAllocater : public OVR::Allocator
{
public:
    virtual void*   Alloc(OVR::UPInt size) { return he_malloc(size); }
    virtual void*   AllocDebug(OVR::UPInt size, const char* file, unsigned line) { return he_malloc_dbg(size, file, line); }
    virtual void*   Realloc(void* p, OVR::UPInt newSize) { return he_realloc(p, newSize); }
    virtual void    Free(void *p) { he_free(p); }
    virtual void*   AllocAligned(OVR::UPInt size, OVR::UPInt align) { return he_aligned_malloc(size, align); }
    virtual void    FreeAligned(void* p) { he_aligned_free(p); }
};

class OculusLogger : public OVR::Log
{
public:
    OculusLogger() : OVR::Log(OVR::LogMask_All) {}

    virtual void LogMessageVarg(OVR::LogMessageType messageType, const char* fmt, va_list argList)
    {
        switch (messageType)
        {
        case OVR::Log_Text: he::tools::Logger::getInstance()->log(LogType_Info, __FILE__, __HE_FUNCTION__, __LINE__, fmt, argList); break;
        case OVR::Log_Error:
        case OVR::Log_DebugText:
        case OVR::Log_Debug: he::tools::Logger::getInstance()->log(LogType_Error, __FILE__, __HE_FUNCTION__, __LINE__, fmt, argList); break;

        case OVR::Log_Assert: he::tools::Logger::getInstance()->log(LogType_ProgrammerAssert, __FILE__, __HE_FUNCTION__, __LINE__, fmt, argList); break;
        }
    }
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct OculusRiftBinding::OculusContext : public OVR::MessageHandler
{
    OculusContext(OculusRiftBinding* const parent) 
        : m_DeviceManager(nullptr)
        , m_Parent(parent) {}

    ~OculusContext()
    {
        m_DeviceManager->SetMessageHandler(nullptr);
        m_DeviceManager.Clear();
    }

    virtual void OnMessage(const OVR::Message& msg)
    {
        if (msg.Type == OVR::Message_DeviceAdded)
        {
            HE_INFO("OVR: DeviceManager reported device added.\n");
        }
        else if (msg.Type == OVR::Message_DeviceRemoved)
        {
            HE_INFO("OVR: DeviceManager reported device removed.\n");
        }
    }

    OculusRiftBinding* m_Parent;
    OVR::Ptr<OVR::DeviceManager> m_DeviceManager;
};

OculusRiftBinding::OculusRiftBinding()
    : m_Logger(nullptr)
    , m_Allocater(nullptr)
{
}

OculusRiftBinding::~OculusRiftBinding()
{
}

void OculusRiftBinding::init()
{
    HE_ASSERT(m_Logger == nullptr, "OVR already initialized");
    HE_ASSERT(m_Allocater == nullptr, "OVR already initialized");
    m_Logger = NEW OculusLogger();
    m_Allocater = NEW OculusAllocater();

    OVR::System::Init(m_Logger, m_Allocater);

    m_Context = NEW OculusContext(this);
    m_Context->m_DeviceManager = *OVR::DeviceManager::Create();
    m_Context->m_DeviceManager->SetMessageHandler(m_Context);

    createDevice();
}

void OculusRiftBinding::shutdown()
{
    releaseDevice(m_ConnectedDevices[0]);
    delete m_Context;
    m_Context = nullptr;

    OVR::System::Destroy();
    delete m_Logger;
    m_Logger = nullptr;
    delete m_Allocater;
    m_Allocater = nullptr;
}

OculusRiftDevice* OculusRiftBinding::createDevice()
{
    OVR::Ptr<OVR::HMDDevice> device(*m_Context->m_DeviceManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice());
    OculusRiftDevice* newDevice(NEW OculusRiftDevice());
    if (device != nullptr)
    {
        OculusRiftDevice::DeviceContext* const context(newDevice->m_Context);
        context->m_Device = device;
        context->m_Sensor = *device->GetSensor();

        if (device->GetDeviceInfo(&context->m_Info))
        {
        }
    }
    m_ConnectedDevices.add(newDevice);
    return newDevice;
}

void OculusRiftBinding::releaseDevice( OculusRiftDevice* const device )
{
    size_t index(0);
    if (m_ConnectedDevices.find(device, index))
    {
        OculusRiftDevice::DeviceContext* const context(m_ConnectedDevices[index]->m_Context);
        context->m_Sensor.Clear();
        context->m_Device.Clear();
        delete m_ConnectedDevices[index];
        m_ConnectedDevices.removeAt(index);
    }
    else
    {
        LOG(LogType_ProgrammerAssert, "OVR device does not exist!");
    }
}

OculusRiftDevice* OculusRiftBinding::getDevice( const size_t index ) const
{
    if (index < m_ConnectedDevices.size())
    {
        return m_ConnectedDevices[index];
    }
    else
    {
        return nullptr;
    }
}

} } //end namespace
