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

#include "HeAssert.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Sound2D.h"
#include "Sound3D.h"

namespace he {
namespace sfx {

/* CONSTRUCTOR - DESTRUCTOR */
SoundEngine::SoundEngine() :	m_pALContext(nullptr),
								m_pALDevice(nullptr)
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

		emptyBuffers(pSound->getSource());

		if (pSound->getType() == SOUND_TYPE_STREAM)
		{
			alDeleteBuffers(STREAM_BUFFERS, m_SoundBuffers[pSound->getBuffer()]);
		}
		else
		{
			alDeleteBuffers(1, m_SoundBuffers[pSound->getBuffer()]);
		}

		alDeleteSources(1, &m_SoundSources[pSound->getSource()]);

		delete pSound;
		pSound = nullptr;
	});

	alcMakeContextCurrent(0);
	alcDestroyContext(m_pALContext);
	alcCloseDevice(m_pALDevice);
}

bool SoundEngine::streamSound(SoundFile& stream, ALuint buffer)
{
	short pData[STREAM_BUFFER_SIZE];
	uint samples(0);
	uint samplesRead(0);

	// read data, keep reading until read data == buffer_size
	while (samplesRead < STREAM_BUFFER_SIZE)
	{
		samples = stream.read(pData, STREAM_BUFFER_SIZE - samplesRead);

		if (samples > 0)
			samplesRead += samples;
		else if (samples == 0) // nothing to read anymore
			break;
	}

	// if nothing to read, ie. file end
	if (samplesRead == 0)
		return false;

	SoundFileProperties props(stream.getProperties());

	// fill buffer with new read data
	alBufferData(buffer, getALFormatFromChannels(props.channelsCount), pData, samplesRead * sizeof(short), props.samplerate);

	return true;
}

void SoundEngine::emptyBuffers(uint source)
{
	int queued;

	ALuint alSource(m_SoundSources[source]);

	alGetSourcei(alSource, AL_BUFFERS_QUEUED, &queued);

	while(queued--)
	{
		ALuint buffer;

		alSourceUnqueueBuffers(alSource, 1, &buffer);
	}
}

ALenum SoundEngine::getALFormatFromChannels(uint channels) const
{
	/*switch (channels)
	{
		case 1: return AL_FORMAT_MONO16; break;
		case 2: return AL_FORMAT_STEREO16; break;
		case 4: return AL_FORMAT_QUAD16; break;
		case 6: return AL_FORMAT_MONO16; break;
		case 7: return AL_FORMAT_MONO16; break;
		case 8: return AL_FORMAT_MONO16; break;
	}*/

	if(channels == 1)
		return AL_FORMAT_MONO16;
	else
		return AL_FORMAT_STEREO16;
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

void SoundEngine::tick(float dTime)
{
	std::for_each(m_SoundBank.begin(), m_SoundBank.end(), [&](ISound* pSound)
	{
		// check for looping
		if (pSound->getState() == SOUND_STATE_PLAYING)
		{
			m_SoundTime[pSound] += dTime;
		}
		else if (	pSound->getState() == SOUND_STATE_STOPPED &&
					pSound->getLooping() &&
					m_SoundTime[pSound] >= pSound->getLength() )
		{
			pSound->play(true);

			m_SoundTime[pSound] = 0.0f;
		}

		// stream sound
		if (pSound->getType() == SOUND_TYPE_STREAM)
		{
			if (pSound->getState() == SOUND_STATE_PLAYING)
			{
				int processed(0);
				ALuint source(m_SoundSources[pSound->getSource()]);

				alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

				while (processed--)
				{
					ALuint buffer;

					alSourceUnqueueBuffers(source, 1, &buffer);

					// if nothing to read
					if (streamSound(m_SoundFiles[pSound->getSoundFile()], buffer) == false)
					{
						// check if sound is looping
						if (pSound->getLooping())
						{
							// reset reading from soundfile
							m_SoundFiles[pSound->getSoundFile()].seek(0);
						}
					}

					alSourceQueueBuffers(source, 1, &buffer);
				}
			}
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

	// new soundfile
	m_SoundFiles.push_back(SoundFile(path));

	SoundFile& soundFile = m_SoundFiles[m_SoundFiles.size() - 1];

	// try to open
	bool success(soundFile.open());

	std::string err("Failed to open sound file: ");
	err += path;

	ASSERT(success == true, err);

	if (stream == true)
	{
		// create source, buffer for file
		ALuint source(0), *buffers(NEW ALuint(STREAM_BUFFERS));

		alGenSources(1, &source);
		alGenBuffers(STREAM_BUFFERS, buffers);

		m_SoundSources.push_back(source);
		m_SoundBuffers.push_back(buffers);

		// create sound
		pSound = NEW Sound2D(m_SoundSources.size() - 1, m_SoundBuffers.size() - 1, m_SoundFiles.size() - 1, SOUND_TYPE_STREAM);

		// put sound in soundbank
		m_SoundBank.push_back(pSound);
	}
	else
	{
		// create source, buffer for file
		ALuint source(0), buffer(0);

		alGenSources(1, &source);
		alGenBuffers(1, &buffer);

		m_SoundSources.push_back(source);
		m_SoundBuffers.push_back(&buffer);

		// get soundfile properties
		SoundFileProperties props(soundFile.getProperties());

		// read data from soundfile
		short* pData(NEW short[props.samplesCount]);
		success = (soundFile.read(pData, props.samplesCount) == static_cast<short>(props.samplesCount));
		
		ASSERT(success == true, err);

		// fill soundbuffer with data
		alBufferData(
			buffer,
			getALFormatFromChannels(props.channelsCount),
			pData,
			props.samplesCount * sizeof(short),
			props.samplerate );

		// delete data
		delete[] pData;

		// bind buffer with source
		alSourceQueueBuffers(source, 1, &buffer);

		// all data already read, no need to keep open
		soundFile.close();

		// create sound
		pSound = NEW Sound2D(m_SoundSources.size() - 1, m_SoundBuffers.size() - 1, m_SoundFiles.size() - 1, SOUND_TYPE_STATIC);

		// put sound in soundbank
		m_SoundBank.push_back(pSound);
	}

	// for 2D sound, position is not relative to listener
	alSourcei(m_SoundSources[pSound->getSource()], AL_SOURCE_RELATIVE, 0);

	// create soundtime, needed for looping
	m_SoundTime[pSound] = float(0.0f);

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
		for (uint i(0); i < STREAM_BUFFERS; ++i)
		{
			if (!streamSound(m_SoundFiles[pSound->getSoundFile()], m_SoundBuffers[pSound->getBuffer()][i]))
				return;
		}

		ALuint source(m_SoundSources[pSound->getSource()]);
		ALuint* buffers(m_SoundBuffers[pSound->getBuffer()]);
			
		alSourceQueueBuffers(source, STREAM_BUFFERS, buffers);
	}

	alSourcePlay(m_SoundSources[pSound->getSource()]);
}

void SoundEngine::stopSound(ISound* pSound)
{
	alSourceStop(m_SoundSources[pSound->getSource()]);

	// reset buffers for streaming files
	if (pSound->getType() == SOUND_TYPE_STREAM)
	{
		emptyBuffers(pSound->getSource());
		m_SoundFiles[pSound->getSoundFile()].seek(0);
	}

	m_SoundTime[pSound] = 0.0f;
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

ALuint SoundEngine::getALSource(uint source) const
{
	return m_SoundSources[source];
}

ALuint* SoundEngine::getALBuffer(uint buffer) const
{
	return m_SoundBuffers[buffer];
}

SoundFile& SoundEngine::getSoundFile(uint soundFile)
{
	return m_SoundFiles[soundFile];
}

} } //end namespace