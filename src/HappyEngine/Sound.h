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

#ifndef _HE_SOUND_H_
#define _HE_SOUND_H_
#pragma once

#include "al.h"

#include "boost/shared_ptr.hpp"

namespace happyengine {
namespace audio {

class Sound
{
public:

	typedef boost::shared_ptr<Sound> pointer;

	/* CONSTRUCTOR - DESTRUCTOR */
	Sound(ALuint alSource, ALuint alSourceBuffer);
    virtual ~Sound();

	/* GENERAL */
	void play();
	void stop();
	void pause();

	/* SETTERS */
	void setVolume(float volume); // 0.0f = sound turned off, 1.0f = normal volume
	void setMinVolume(float minVolume);
	void setMaxVolume(float maxVolume);
	void setPanning(float leftPercentage = 0.5f); // relation between left & right, 0.5f = L50% R50%
	void setLooping(int nrLoops = 0); // -1 = infinite looping, 0 = no looping

	/* GETTERS */
	float getVolume() const;
	float getLength() const;

private:

	/* DATAMEMBERS */
	ALuint m_AlSource;
	ALuint m_AlSourceBuffer;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Sound(const Sound&);
    Sound& operator=(const Sound&);
};

} } //end namespace

#endif
