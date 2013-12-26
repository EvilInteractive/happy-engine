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

#ifndef _HE_PluginLoader_H_
#define _HE_PluginLoader_H_
#pragma once

namespace he {
namespace pl {
class IPlugin;

class HAPPY_ENTRY PluginLoader
{
#ifdef HE_WINDOWS
    typedef HMODULE PLUGIN_HANDLE;
    typedef FARPROC PLUGIN_FUNCTION;
#else
    typedef void* PLUGIN_HANDLE;
    typedef void* PLUGIN_FUNCTION;
#endif
    struct PluginWrapper
    {
        PluginWrapper() : m_Plugin(nullptr), m_ModuleHandle(NULL) {}
        ~PluginWrapper() {}

        explicit PluginWrapper(IPlugin* const plugin) : m_Plugin(plugin), m_ModuleHandle(NULL) {}
        PluginWrapper(IPlugin* const plugin, PLUGIN_HANDLE const mod) : m_Plugin(plugin), m_ModuleHandle(mod) {}

        IPlugin* m_Plugin;
        PLUGIN_HANDLE m_ModuleHandle;
    };
public:
    PluginLoader();
    ~PluginLoader();

    IPlugin* loadPlugin(const he::Path& path, const char* fileName);
    IPlugin* loadPlugin(IPlugin* const plugin);
    void unloadPlugin(IPlugin* const plugin);
    
private:
    ObjectList<PluginWrapper> m_Plugins;

    /* DEFAULT COPY & ASSIGNMENT */
    PluginLoader(const PluginLoader&);
    PluginLoader& operator=(const PluginLoader&);
};

} } //end namespace

#endif
