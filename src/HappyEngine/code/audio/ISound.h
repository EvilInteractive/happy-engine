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
//Created: 11/10/2011

#ifndef _HE_I_SOUND_H_
#define _HE_I_SOUND_H_
#pragma once

#include "HappyTypes.h"

namespace he {
namespace sfx {

	enum SOUND_STATE
	{
		SOUND_STATE_PLAYING,
		SOUND_STATE_STOPPED,
		SOUND_STATE_PAUSED
	};

	enum SOUND_TYPE
	{
		SOUND_TYPE_STATIC,
		SOUND_TYPE_STREAM
	};

class ISound
{
public:

	virtual ~ISound() {}

	virtual void play(bool forceRestart) = 0;
	virtual void stop() = 0;
	virtual uint getSource() const = 0;
	virtual uint getBuffer() const = 0;
	virtual uint getSoundFile() const = 0;
	virtual bool getLooping() const = 0;
	virtual float getLength() const = 0;
	virtual SOUND_STATE getState() const = 0;
	virtual SOUND_TYPE getType() const = 0;

};

} } //end namespace

#endif
