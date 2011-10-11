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
//
//Author:  Bastian Damman
//Created: 01/09/2011

#ifndef _HE_RANDOM_H_
#define _HE_RANDOM_H_
#pragma once

#include "HappyTypes.h"
#include "boost/random.hpp"
#include <time.h>

namespace he {

class Random
{
public:
	explicit Random(uint seed = time(nullptr));
	//Random();
    virtual ~Random();

    int nextInt(int min, int max);
    float nextFloat(float min, float max);
    double nextDouble(double min, double max);

    uint getSeed() const;

private:

    uint m_Seed;

    boost::mt19937 m_RandomEngine;
    boost::variate_generator<boost::mt19937&, boost::uniform_real<double>> m_Generator;

    //Disable default copy constructor and default assignment operator
    Random(const Random&);
    Random& operator=(const Random&);
};

} //end namespace

#endif
