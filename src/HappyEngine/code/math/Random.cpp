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
#include "HappyPCH.h" 

#include "Random.h"
#include "HappyNew.h"

namespace he {

Random::Random(uint32 seed): 
    m_RandomEngine(seed),
    m_Generator(m_RandomEngine, boost::uniform_real<double>(0.0, 1.0)),
    m_Seed(seed)
{
}

Random::~Random()
{
}

int Random::nextInt(int min, int max) const
{
    return static_cast<int>(min + const_cast<Random*>(this)->m_Generator() * (max - min));
}
float Random::nextFloat(float min, float max) const
{
    return static_cast<float>(min + const_cast<Random*>(this)->m_Generator() * (max - min));
}
double Random::nextDouble(double begin, double max) const
{
    return begin + const_cast<Random*>(this)->m_Generator() * (max - begin);
}

uint32 Random::getSeed() const
{
    return m_Seed;
}
void Random::setSeed(uint32 seed)
{
    m_Seed = seed;
    m_RandomEngine.seed(seed);
}

} //end namespace