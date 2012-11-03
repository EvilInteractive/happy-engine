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
    explicit Random(uint32 seed = time(nullptr));
    virtual ~Random();

    int nextInt(int min, int max) const;
    float nextFloat(float min, float max) const;
    double nextDouble(double min, double max) const;

    uint32 getSeed() const;
    void setSeed(uint32 seed);

private:

    uint32 m_Seed;

    boost::mt19937 m_RandomEngine;
    boost::variate_generator<boost::mt19937&, boost::uniform_real<double>> m_Generator;

    //Disable default copy constructor and assignment operator
    Random(const Random&);
    Random& operator=(const Random&);
};

} //end namespace

#endif
