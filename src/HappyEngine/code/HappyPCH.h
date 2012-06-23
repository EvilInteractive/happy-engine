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

#define __HE_FUNCTION__ __FUNCTION__
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <queue>
#include <set>
#include <algorithm>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdarg>

#include "boost/shared_ptr.hpp"
#pragma warning (disable : 4244)
#include "boost/thread.hpp"
#pragma warning (default : 4244)
#include "boost/chrono.hpp"
#include "boost/any.hpp"
#include "boost/function.hpp"
#include "boost/date_time.hpp"

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#undef MessageBox


#include "GL/glew.h"
#include "OpenGL.h"

#include "PxPhysicsAPI.h"

#include "HappyTypes.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat44.h"
#include "mat33.h"
#include "Color.h"

#include "HeAssert.h"
#include "ExternalError.h"
#include "HappyMemory.h"
#include "HappyNew.h"
#include "MathConstants.h"
#include "MathFunctions.h"

#include "Profiler.h"

#include "event.h"
#include "Path.h"

#include "Keys.h"

#include "HappyEngine.h"
#include "Console.h"

#include "Logger.h"
#include "HappyInfo.h"

#endif
