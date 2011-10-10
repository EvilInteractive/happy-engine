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

#include "SoundManager.h"
#include "HappyNew.h"

#include "Exception.h"
#include <iostream>
#include <sstream>

namespace he {
namespace sfx {

/* CONSTRUCTOR - DESTRUCTOR */
SoundManager::SoundManager() :	m_pALContext(nullptr),
								m_pALDevice(nullptr)
{
}

SoundManager::~SoundManager()
{
	shutdown();
}

void SoundManager::shutdown()
{
	alcMakeContextCurrent(0);
	alcDestroyContext(m_pALContext);
	alcCloseDevice(m_pALDevice);
}

//Sound::pointer SoundManager::loadWave(const std::string& /*path*/, bool /*stream*/)
//{
//	/*if (m_SoundBank.isAssetPresent(path))
//	{
//		return m_SoundBank.getAsset(path);
//	}
//	else
//	{*/
////		char* alBuffer;
//		//ALenum alFormatBuffer;
//		//ALsizei alFreqBuffer;
//		//ALsizei alBufferLen;
////		ALboolean alLoop;
//		ALuint alSource;
//		ALuint alSourceBuffer;
//
////		alSourceBuffer = alutCreateBufferFromFile((char*)path.c_str());//, &alFormatBuffer, (void**) &alBuffer, &alBufferLen, &alFreqBuffer, &alLoop);
//
//		alGenSources(1, &alSource);
//		//alGenBuffers(1, &alSourceBuffer);
//
//		//alBufferData(alSourceBuffer, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
//
//		alSourcei(alSource, AL_BUFFER, alSourceBuffer);
//
//		//alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
//
//		Sound::pointer sound(NEW Sound(alSource, alSourceBuffer));
//
//		m_SoundBank.addAsset(path, sound);
//
//		return sound;
//	//}
//}

//Sound::pointer SoundManager::loadOgg(const std::string& path, bool /*stream*/)
//{
//
//}

/* GENERAL */
void SoundManager::initialize()
{
	m_pALDevice = alcOpenDevice(NULL);

	if (m_pALDevice)
	{
		m_pALContext = alcCreateContext(m_pALDevice, 0);

		if (m_pALContext)
		{
			alcMakeContextCurrent(m_pALContext);
		}
		else
		{
			alcCloseDevice(m_pALDevice);

			throw(err::Exception(L"Creating OpenAL context failed!"));
		}
	}
	else
	{
		throw(err::Exception(L"Init OpenAL device failed!"));
	}
}

void SoundManager::deleteAllSounds()
{
	m_SoundBank.removeAllAssets();
}

//Sound::pointer SoundManager::loadSound(const std::string& path, bool /*stream*/)
//{
//	if (path.find(".wav"))
//	{
//		return loadWave(path);
//	}
//}

} } //end namespace