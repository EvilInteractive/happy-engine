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

#ifndef _HE_SOUND2D_H_
#define _HE_SOUND2D_H_
#pragma once

#include "al.h"
#include "HappyTypes.h"
#include "vorbis/vorbisfile.h"
#include "ISound.h"

namespace he {
namespace sfx {

class Sound2D : public ISound
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	Sound2D(uint source, uint buffer, SOUND_TYPE type);

    virtual ~Sound2D();

	/* GENERAL */
	void play(bool forceRestart = false);
	void stop();
	void pause();

	/* SETTERS */
	void setVolume(float volume); // 0.0f = sound turned off, 1.0f = normal volume
	void setPanning(float leftPercentage = 0.5f); // relation between left & right, 0.5f = L50% R50%
	void setLooping(int nrLoops = 0); // -1 = infinite looping, 0 = no looping

	/* GETTERS */
	uint getSource() const;
	uint getBuffer() const;
	SOUND_STATE getState() const;
	SOUND_TYPE getType() const;

	float getVolume() const;
	float getLength() const;

private:

	/* DATAMEMBERS */
	uint m_Source;
	uint m_Buffer;
	uint m_Stream;

	SOUND_TYPE m_Type;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Sound2D(const Sound2D&);
    Sound2D& operator=(const Sound2D&);
};

} } //end namespace

#endif
