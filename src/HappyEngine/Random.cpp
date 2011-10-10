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
#include "StdAfx.h" 

#include "Random.h"
#include "HappyNew.h"

namespace he {

Random::Random(uint seed): 
    m_RandomEngine(seed),
    m_Generator(m_RandomEngine, boost::uniform_real<double>(0.0, 1.0)),
    m_Seed(seed)
{
}

Random::~Random()
{
}

int Random::nextInt(int min, int max)
{
    return static_cast<int>(min + m_Generator() * (max - min));
}
float Random::nextFloat(float min, float max)
{
    return static_cast<float>(min + m_Generator() * (max - min));
}
double Random::nextDouble(double begin, double max)
{
    return begin + m_Generator() * (max - begin);
}

uint Random::getSeed() const
{
    return m_Seed;
}

} //end namespace