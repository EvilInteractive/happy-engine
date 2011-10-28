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

#include "SoundEngine.h"
#include "HappyNew.h"

#include "Assert.h"
#include <iostream>
#include <sstream>
#include "Sound2D.h"
#include "Sound3D.h"

namespace he {
namespace sfx {

/* CONSTRUCTOR - DESTRUCTOR */
SoundEngine::SoundEngine() :	m_pALContext(nullptr),
								m_pALDevice(nullptr),
								m_BufferSize(4096 * 8)
{
}

SoundEngine::~SoundEngine()
{
	shutdown();
}

void SoundEngine::shutdown()
{
	std::for_each(m_SoundBank.begin(), m_SoundBank.end(), [&](ISound* pSound)
	{
		pSound->stop();

		if (pSound->getType() == SOUND_TYPE_STREAM)
		{
			emptyBuffers(pSound->getSource());

			alDeleteBuffers(2, m_SoundBuffers[pSound->getBuffer()]);

			ov_clear(&m_SoundStreams[m_SoundStreamIndex[pSound]]);
		}
		else
		{
			alDeleteBuffers(1, m_SoundBuffers[pSound->getBuffer()]);
		}

		alDeleteSources(1, m_SoundSources[pSound->getSource()]);

		delete pSound;
		pSound = nullptr;
	});

	alcMakeContextCurrent(0);
	alcDestroyContext(m_pALContext);
	alcCloseDevice(m_pALDevice);
}

uint SoundEngine::loadOggStream(const std::string& path)
{
	OggVorbis_File oggStream;

	if (ov_fopen(path.c_str(), &oggStream) < 0)
	{
		ASSERT(false,"Could not open ogg stream.");
	}

	m_SoundStreams.push_back(oggStream);

	return (m_SoundStreams.size() - 1);
}

bool SoundEngine::streamOgg(OggVorbis_File& stream, ALuint buffer)
{
	char data[BUFFER_SIZE];
	int size(0);
	int section(0);
	int result(0);

	while (size < BUFFER_SIZE)
	{
		result = ov_read(&stream, data + size, BUFFER_SIZE - size, 0, 2, 1, &section);

		if (result > 0)
			size += result;
		else if (result == 0)
			break;
	}

	if (size == 0)
		return false;

	vorbis_info vorbisInfo(*ov_info(&stream, -1));
	ALenum format;

	if(vorbisInfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else
		format = AL_FORMAT_STEREO16;

	alGetError();

	alBufferData(buffer, format, data, size, vorbisInfo.rate);

	ALenum error = alGetError();

	if (error == AL_INVALID_ENUM)
		ASSERT(false, "Error filling buffer");

	return true;
}

void SoundEngine::emptyBuffers(uint source)
{
	int queued;

	ALuint alSource(*m_SoundSources[source]);

	alGetSourcei(alSource, AL_BUFFERS_QUEUED, &queued);

	while(queued--)
	{
		ALuint buffer;

		alSourceUnqueueBuffers(alSource, 1, &buffer);
	}
}

/* GENERAL */
void SoundEngine::initialize()
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

			ASSERT(false, "Creating OpenAL context failed!");
		}
	}
	else
	{
		ASSERT(false,"Init OpenAL device failed!");
	}
}

void SoundEngine::tick()
{
	std::for_each(m_SoundBank.begin(), m_SoundBank.end(), [&](ISound* pSound)
	{
		if (pSound->getType() == SOUND_TYPE_STREAM)
		{
			if (pSound->getState() == SOUND_STATE_PLAYING)
			{
				int processed(0);
				ALuint source(*m_SoundSources[pSound->getSource()]);

				alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

				while (processed--)
				{
					ALuint buffer;

					alSourceUnqueueBuffers(source, 1, &buffer);

					streamOgg(m_SoundStreams[m_SoundStreamIndex[pSound]], buffer);

					alSourceQueueBuffers(source, 1, &buffer);
				}
			}
		}

		Sound2D* pS2D(dynamic_cast<Sound2D*>(pSound));

		if (pS2D != nullptr)
		{
			alSource3f(*m_SoundSources[pS2D->getSource()], AL_POSITION, getListenerPos().x, getListenerPos().y, getListenerPos().z);
		}
	});
}

void SoundEngine::deleteAllSounds()
{
	//m_SoundBank.removeAllAssets();
}

Sound2D* SoundEngine::loadSound2D(const std::string& path, bool stream)
{
	Sound2D* pSound(nullptr);

	if (path.find(".ogg"))
	{
		if (stream == true)
		{
			uint stream = loadOggStream(path);

			ALuint *source(NEW ALuint()), *buffers(NEW ALuint[2]);

			alGenSources(1, source);
			alGenBuffers(2, buffers);

			m_SoundSources.push_back(source);
			m_SoundBuffers.push_back(buffers);

			pSound = NEW Sound2D(m_SoundSources.size() - 1, m_SoundBuffers.size() - 1, SOUND_TYPE_STREAM);

			m_SoundStreamIndex[pSound] = stream;

			m_SoundBank.push_back(pSound);
		}
	}

	return pSound;
}

void SoundEngine::playSound(ISound* pSound, bool forceRestart)
{
	if (pSound->getState() == SOUND_STATE_PLAYING)
	{
		if (forceRestart == false)
			return;
		else
			stopSound(pSound);
	}

	if (pSound->getType() == SOUND_TYPE_STREAM)
	{
		if (!streamOgg(m_SoundStreams[m_SoundStreamIndex[pSound]], m_SoundBuffers[pSound->getBuffer()][0]))
			return;
		if (!streamOgg(m_SoundStreams[m_SoundStreamIndex[pSound]], m_SoundBuffers[pSound->getBuffer()][1]))
			return;
			
		alSourceQueueBuffers(*m_SoundSources[pSound->getSource()], 2, m_SoundBuffers[pSound->getBuffer()]);
	}

	alSourcePlay(*m_SoundSources[pSound->getSource()]);
}

void SoundEngine::stopSound(ISound* pSound)
{
	alSourceStop(*m_SoundSources[pSound->getSource()]);
	emptyBuffers(pSound->getSource());

	ov_time_seek(&m_SoundStreams[m_SoundStreamIndex[pSound]], 0.0);
}

/* SETTERS */
void SoundEngine::setListenerPos(const vec3& pos)
{
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

/* GETTERS */
vec3 SoundEngine::getListenerPos() const
{
	vec3 pos(0, 0, 0);

	alGetListener3f(AL_POSITION, &pos.x, &pos.y, &pos.z);
        
	return pos;
}

ALuint SoundEngine::getSource(uint source)
{
	return *m_SoundSources[source];
}

ALuint SoundEngine::getBuffer(uint buffer)
{
	return *m_SoundBuffers[buffer];
}

} } //end namespace