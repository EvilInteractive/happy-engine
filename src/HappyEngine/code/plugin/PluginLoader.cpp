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

PluginLoader::PluginWrapper::PluginWrapper( const he::Path buildFolder, const he::String buildName ) 
: m_Plugin(nullptr), m_ModuleHandle(NULL), m_Folder(std::move(buildFolder)), m_Name(std::move(buildName))
{

}

he::Path PluginLoader::PluginWrapper::getBuildPath() const
{
#ifdef HE_WINDOWS
    return m_Folder.append(m_Name + ".dll");
#else
    return m_Folder.append("lib" + m_Name + ".dylib");
#endif
}

he::Path PluginLoader::PluginWrapper::getRunPath() const
{
#ifdef HE_WINDOWS
    return m_Folder.append(m_Name + "_running.dll");
#else
    return m_Folder.append("lib" + m_Name + "_running.dylib");
#endif
}

he::Path PluginLoader::PluginWrapper::getPdbBuildPath() const
{
#ifdef HE_WINDOWS
    return m_Folder.append(m_Name + ".pdb");
#else
    #error ?
#endif
}

he::Path PluginLoader::PluginWrapper::getPdbRunPath() const
{
#ifdef HE_WINDOWS
    return m_Folder.append(m_Name + "_running.pdb");
#else
#error ?
#endif
}

bool PluginLoader::PluginWrapper::createRunLib() const
{
#ifdef HE_WINDOWS
    Path buildPath(getBuildPath());
    Path runPath(getRunPath());
    bool result(CopyFile(buildPath.str().c_str(), runPath.str().c_str(), FALSE) == TRUE);
    #ifdef HE_DEBUG
    Path pdbPath(getPdbBuildPath());
    Path runPdbPath(getPdbRunPath());
    result &= CopyFile(pdbPath.str().c_str(), runPdbPath.str().c_str(), FALSE) == TRUE;
    #endif
#else
    #error Implement!
#endif
    return result;
}

PluginLoader::PluginLoader()
{
}

PluginLoader::~PluginLoader()
{
}

// http://en.wikipedia.org/wiki/Dynamic_loading
IPlugin* PluginLoader::loadPlugin( PluginWrapper& wrapper )
{
    IPlugin* result(wrapper.m_Plugin);
    const bool isReload(wrapper.m_ModuleHandle != NULL);
    if (isReload)
    {
#ifdef HE_WINDOWS
        FreeLibrary(wrapper.m_ModuleHandle);
#else
        dlclose(wrapper.m_ModuleHandle);
#endif
        wrapper.m_ModuleHandle = nullptr;
    }
    if (wrapper.createRunLib())
    {
#ifdef HE_WINDOWS
        PLUGIN_HANDLE mod(LoadLibrary(wrapper.getRunPath().str().c_str())); // if it fails, convert to backslashes
#else
        PLUGIN_HANDLE mod(dlopen(wrapper.getRunPath().str().c_str(), RTLD_LAZY));
#endif
        if (mod != NULL)
        {
            wrapper.m_ModuleHandle = mod;
            if (!isReload)
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
                        wrapper.m_Plugin = plugin;
                        m_Plugins.add(std::move(wrapper));
                        result = plugin;
                    }
                    else
                    {
                        HE_WARNING("Could not load plugin: '%s'\n because the plugin == nullptr.", wrapper.m_Name.c_str());
                    }
                }
                else
                {
                    HE_WARNING("Could not load plugin: '%s'\n because I could not retrieve the plugin pointer.", wrapper.m_Name.c_str());
                }
            }
        }
        else
        {
            HE_WARNING("Could not load plugin: '%s', file not found?", wrapper.m_Name.c_str());
        }
    }
    else
    {
        HE_WARNING("Could not load plugin: '%s', could not create runlib (file copy failed)", wrapper.m_Name.c_str());
    }
    return result;
}

IPlugin* PluginLoader::loadPlugin( const he::Path& path, const char* fileName )
{
    PluginWrapper wrapper(path, fileName);
    return loadPlugin(wrapper);
}

bool PluginLoader::reloadPlugin( IPlugin* const plugin )
{
    size_t index(0);
    if (m_Plugins.find_if([plugin](const PluginWrapper& wrapper) { return wrapper.m_Plugin == plugin; }, index ))
    {
        PluginWrapper& wrapper(m_Plugins[index]);
        loadPlugin(wrapper);
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
        HEDelete(wrapper.m_Plugin);
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
