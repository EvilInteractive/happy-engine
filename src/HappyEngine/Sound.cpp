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
//Author:  Sebastiaan Sprengers
//Created: 03/10/2011
#include "StdAfx.h" 

#include "Sound.h"
#include "HappyNew.h"

namespace he {
namespace sfx {

/* CONSTRUCTOR - DESTRUCTOR */
Sound::Sound(ALuint alSource, ALuint alSourceBuffer) :	m_AlSource(alSource),
														m_AlSourceBuffer(alSourceBuffer	)
{
}

Sound::~Sound()
{
	alDeleteSources(1, &m_AlSource);
	alDeleteBuffers(1, &m_AlSourceBuffer);
}

/* GENERAL */
void Sound::play()
{
	alSourcePlay(m_AlSource);
}
void Sound::stop()
{
	alSourceStop(m_AlSource);
}
void Sound::pause()
{
}

/* SETTERS */
void Sound::setVolume(float /*volume*/)
{
}
void Sound::setMinVolume(float /*minVolume*/)
{
}
void Sound::setMaxVolume(float /*maxVolume*/)
{
}
void Sound::setPanning(float /*leftPercentage*/)
{
}
void setLooping(int /*nrLoops*/)
{
}

/* GETTERS */
float Sound::getVolume() const
{
	return 0;
}
float Sound::getLength() const
{
	return 0;
}

} } //end namespace