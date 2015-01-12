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
//Created: 2013/03/03

#include "HappyPCH.h" 

#include "PluginLoader.h"
#include "IPlugin.h"

#include "BinaryStreamVisitor.h"
#include "StaticDataManager.h"

#ifndef HE_WINDOWS
#include <dlfcn.h>
#endif

namespace he {
namespace pl {

PluginLoader::PluginLoader()
{
}

PluginLoader::~PluginLoader()
{
}

// http://en.wikipedia.org/wiki/Dynamic_loading
IPlugin* PluginLoader::loadPlugin( const he::Path& path, const char* fileName )
{
    IPlugin* result(nullptr);
#ifdef HE_WINDOWS
    he::String fullpath(path.str() + fileName + ".dll");
    PLUGIN_HANDLE mod(LoadLibrary(fullpath.c_str())); // if it fails, convert to backslashes
#else
    he::String fullpath(path.str() + "lib" + fileName + ".dylib");
    PLUGIN_HANDLE mod(dlopen(fullpath.c_str(), RTLD_LAZY));
#endif
    if (mod != NULL)
    {
#ifdef HE_WINDOWS
        PLUGIN_FUNCTION proc(GetProcAddress(mod, "createPlugin"));
#else
        PLUGIN_FUNCTION proc(dlsym(mod, "createPlugin"));
#endif
        if (proc != NULL)
        {
            CreatePluginFunc func(reinterpret_cast<CreatePluginFunc>(proc));
            IPlugin* const plugin(func());
            if (plugin != nullptr)
            {
                PluginWrapper wrapper(plugin, mod, fullpath);
                m_Plugins.add(wrapper);
                result = plugin;
            }
            else
            {
                HE_WARNING("Could not load plugin: '%s'\n because the plugin == nullptr.", fullpath.c_str());
            }
        }
        else
        {
            HE_WARNING("Could not load plugin: '%s'\n because I could not retrieve the plugin pointer.", fullpath.c_str());
        }
    }
    else
    {
        HE_WARNING("Could not load plugin: '%s', file not found?", fullpath.c_str());
    }

    return result;
}

bool PluginLoader::reloadPlugin( IPlugin* const plugin )
{
    size_t index(0);
    if (m_Plugins.find_if([plugin](const PluginWrapper& wrapper) { return wrapper.m_Plugin == plugin; }, index ))
    {
        PluginWrapper& wrapper(m_Plugins[index]);
        if (wrapper.m_ModuleHandle != NULL)
        {
#ifdef HE_WINDOWS
            FreeLibrary(wrapper.m_ModuleHandle);
            wrapper.m_ModuleHandle = LoadLibrary(wrapper.m_PluginPath.c_str());
#else
            dlclose(wrapper.m_ModuleHandle);
#endif
        }
        m_Plugins.removeAt(index);
    }
    else
    {
        LOG(LogType_ProgrammerAssert, "Could not unload plugin, because it is not loaded");
    }
    return true;
}

IPlugin* PluginLoader::loadPlugin( IPlugin* const plugin )
{
    PluginWrapper wrapper(plugin);
    m_Plugins.add(wrapper);
    return plugin;
}

void PluginLoader::unloadPlugin( IPlugin* const plugin )
{
    size_t index(0);
    if (m_Plugins.find_if([plugin](const PluginWrapper& wrapper) { return wrapper.m_Plugin == plugin; }, index ))
    {
        const PluginWrapper& wrapper(m_Plugins[index]);
        delete wrapper.m_Plugin;
        if (wrapper.m_ModuleHandle != NULL)
        {
#ifdef HE_WINDOWS
            FreeLibrary(wrapper.m_ModuleHandle);
#else
            dlclose(wrapper.m_ModuleHandle);
#endif
        }
        m_Plugins.removeAt(index);
    }
    else
    {
        LOG(LogType_ProgrammerAssert, "Could not unload plugin, because it is not loaded");
    }
}


} } //end namespace
