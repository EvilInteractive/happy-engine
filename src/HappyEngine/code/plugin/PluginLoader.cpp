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
//Created: 2013/03/03

#include "HappyPCH.h" 

#include "PluginLoader.h"
#include "IPlugin.h"

#include "BinaryStreamVisitor.h"
#include "StaticDataManager.h"

namespace he {
namespace pl {

PluginLoader::PluginLoader()
{
}

PluginLoader::~PluginLoader()
{
}

// http://en.wikipedia.org/wiki/Dynamic_loading
IPlugin* PluginLoader::loadPlugin( const he::Path& path )
{
    IPlugin* result(nullptr);
#ifdef HE_WINDOWS
    HMODULE mod(LoadLibrary(path.str().c_str())); // if it fails, convert to backslashes
    if (mod != NULL)
    {
        FARPROC proc(GetProcAddress(mod, "createPlugin"));
        if (proc != NULL)
        {
            CreatePluginFunc func(reinterpret_cast<CreatePluginFunc>(proc));
            IPlugin* const plugin(func());
            if (plugin != nullptr)
            {
                PluginWrapper wrapper(plugin, mod);
                m_Plugins.add(wrapper);
                io::BinaryStreamVisitor visitor;
                visitor.openWrite();
                StaticDataManager::visit(visitor);
                visitor.close();
                visitor.openRead();
                plugin->sdmInit(visitor);
                visitor.close();
                plugin->init();
                result = plugin;
            }
            else
            {
                HE_WARNING("Could not load plugin: '%s'\n because the plugin == nullptr.", path.str().c_str());
            }
        }
        else
        {
            HE_WARNING("Could not load plugin: '%s'\n because I could not retrieve the plugin pointer.", path.str().c_str());
        }
    }
    else
    {
        HE_WARNING("Could not load plugin: '%s', file not found?", path.str().c_str());
    }
#else
#error Implement dynamic linking for mac and linux
#endif

    return result;
}

IPlugin* PluginLoader::loadPlugin( IPlugin* const plugin )
{
    PluginWrapper wrapper(plugin);
    m_Plugins.add(wrapper);
    plugin->init();
    return plugin;
}

void PluginLoader::unloadPlugin( IPlugin* const plugin )
{
    size_t index(0);
    if (m_Plugins.find_if([plugin](const PluginWrapper& wrapper) { return wrapper.m_Plugin == plugin; }, index ))
    {
        const PluginWrapper& wrapper(m_Plugins[index]);
        wrapper.m_Plugin->terminate();
        delete wrapper.m_Plugin;
        if (wrapper.m_ModuleHandle != NULL)
        {
            FreeLibrary(wrapper.m_ModuleHandle);
        }
        m_Plugins.removeAt(index);
    }
    else
    {
        LOG(LogType_ProgrammerAssert, "Could not unload plugin, because it is not loaded");
    }
}

} } //end namespace