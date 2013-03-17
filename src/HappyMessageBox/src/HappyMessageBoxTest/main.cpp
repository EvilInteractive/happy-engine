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
//Created: 2013/03/16
#include "HappyMessageBox.h"

#include <stdio.h>
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#pragma warning(disable:4100)
#include <Awesomium/WebCore.h>
#pragma warning(default:4100)

int main( int /*argc*/, char** /*args[]*/ )
{
    char workDir[FILENAME_MAX];
    if (GetCurrentDir(workDir, sizeof(workDir)))
    {
        Awesomium::WebCore::Initialize(Awesomium::WebConfig());
        std::string file(std::string(workDir) + "/../../data/messageBox.html");
        he::HappyMessageBox::init(file.c_str(), 1024, 512);
        he::HappyMessageBox::showAssert("Test", "Test");
        he::HappyMessageBox::destroy();
        Awesomium::WebCore::Shutdown();
    }
    return 0;
}
