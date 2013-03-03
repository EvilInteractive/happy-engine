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

#ifndef _HE_IPlugin_H_
#define _HE_IPlugin_H_
#pragma once

namespace he {

namespace io {
    class BinaryVisitor;
}

namespace pl {

class IPlugin;

#ifdef HE_WINDOWS
#define PLUGIN_EXPORT extern "C" __declspec(dllexport)
typedef IPlugin* (*CreatePluginFunc)();
#else
#error Implement for non windows!
#endif

enum EPluginType
{
    ePluginType_ToolPlugin,
    ePluginType_ToolGamePlugin
};

class IPlugin
{
public:
    virtual ~IPlugin() {}

    virtual void sdmInit(io::BinaryVisitor& visitor) = 0;

    virtual void init() = 0;
    virtual void terminate() = 0;

    virtual he::pl::EPluginType getPluginType() const = 0;

    // Events
    virtual void onLoadLevel(const he::Path& path) = 0;
    virtual void onUnloadLevel() = 0;
    virtual void onLevelLoaded() = 0;
    virtual void onStartGame() = 0;
    virtual void onStopGame() = 0;
    virtual void onPauseGame() = 0;
    virtual void onResumeGame() = 0;
};

} } //end namespace

#endif
