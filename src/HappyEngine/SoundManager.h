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

#ifndef _HE_SOUND_MANAGER_H_
#define _HE_SOUND_MANAGER_H_
#pragma once

#include "Sound.h"
#include "AssetContainer.h"
#include <string>
#include "al.h"
#include "alc.h"

namespace he {
namespace sfx {

class SoundManager
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	SoundManager();
    virtual ~SoundManager();

	/* GENERAL */
	void initialize();
	void deleteAllSounds();

	/*Sound::pointer loadSound(const std::string& path, bool stream = false);*/

private:

	void shutdown();

	//Sound::pointer loadWave(const std::string& path, bool stream = false);
	////Sound::pointer loadOgg(const std::string& path, bool stream = false);

	/* DATAMEMBERS */
	ct::AssetContainer<Sound::pointer> m_SoundBank;

	ALCcontext* m_pALContext;
	ALCdevice* m_pALDevice;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    SoundManager(const SoundManager&);
    SoundManager& operator=(const SoundManager&);
};

} } //end namespace

#endif
