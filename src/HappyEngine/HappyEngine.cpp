//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include "HappyEngine.h"
#include <iostream>

namespace happyengine {

HappyEngine::HappyEngine()
{
}
HappyEngine::~HappyEngine()
{
    std::cout << "--Thank you for using HappyEngine--\n";
}

void HappyEngine::init()
{
    using namespace std;
    cout << "       ******************************       \n";
    cout << "  ****************        ****************  \n";
    cout << "*************  HappyEngine :D  *************\n";
    cout << "  ***************          ***************  \n";
    cout << "       ******************************       \n\n\n";
}

HappyEngine::pointer HappyEngine::getPointer()
{
    if (s_pHappyEngine.get() == nullptr)
        s_pHappyEngine = boost::shared_ptr<HappyEngine>(new HappyEngine());
    return s_pHappyEngine;
}

} //end namespace
