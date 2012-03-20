//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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

#ifndef _HE_HAPPYPCH_H_
#define _HE_HAPPYPCH_H_
#pragma once

#pragma message("-------------------------------------------")
#pragma message("HappyEngine: Compiling precompiled headers.")
#pragma message("-------------------------------------------\n")

#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <deque>

#include "boost/shared_ptr.hpp"
#pragma warning (disable : 4244)
#include "boost/thread.hpp"
#pragma warning (default : 4244)
#include "boost/chrono.hpp"

#define SDL_NO_COMPAT
#include "SDL.h"
#include "GL/glew.h"

#include "HappyTypes.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

#include "HeAssert.h"
#include "HappyMemory.h"
#include "HappyNew.h"
#include "MathConstants.h"
#include "MathFunctions.h"

#include "Profiler.h"

#include "event.h"

#include "HappyEngine.h"
#include "Console.h"

#define HE_INFO(str) {std::cout << "Info: " << str << "\n"; \
                      if (HAPPYENGINE != nullptr) CONSOLE->addMessage(str, CMSG_TYPE_ENGINE);}
#define HE_ERROR(str) {std::cout << "Error: " << str << "\n"; \
                       if (HAPPYENGINE != nullptr) CONSOLE->addMessage(str, CMSG_TYPE_ERROR);} 
#define HE_WARNING(str) {std::cout << "Warning: " << str << "\n"; \
                         if (HAPPYENGINE != nullptr) CONSOLE->addMessage(str, CMSG_TYPE_WARNING); }

std::string itoa(int i);

#endif
