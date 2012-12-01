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
//Author:  Sebastiaan Sprengers
//Created: 03/10/2011
#include "HappyPCH.h" 

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
            alDeleteBuffers(STREAM_BUFFERS, m_SoundBuffers[pSound->getBuffer()].buffers);
        }
        else
        {
            alDeleteBuffers(1, m_SoundBuffers[pSound->getBuffer()].buffers);
        }

        alDeleteSources(1, &m_SoundSources[pSound->getSource()]);

        delete pSound;
        pSound = nullptr;
    });

    alcMakeContextCurrent(0);
    alcDestroyContext(m_pALContext);
    alcCloseDevice(m_pALDevice);
}

bool SoundEngine::streamSound(SoundFile& stream, ALuint buffer, bool toMono)
{
    // if need to be converted to mono, double buffer size because mono data = 1/2 stereo data
    uint32 bufferSize(STREAM_BUFFER_SIZE);

    SoundFileProperties props(stream.getProperties());

    if (props.channelsCount == 2 && toMono)
        bufferSize *= 2;

    // data array
    he::PrimitiveList<short> data;
    data.resize(bufferSize);

    uint32 samplesRead(0);

    // read data
    samplesRead = stream.read(&data[0], bufferSize);

    // if nothing to read, ie. file end
    if (samplesRead == 0)
        return false;

    // convert to mono if needed
    if (props.channelsCount == 2 && toMono)
    {
        // only mono can be used in 3D space
        he::PrimitiveList<short> dataMono;

        convertToMono(data, dataMono);

        // fill buffer with new read data
        alBufferData(buffer, getALFormatFromChannels(1), &dataMono[0], (ALsizei)dataMono.size() * sizeof(short), props.samplerate);
    }
    else
    {
        // fill buffer with new read data
        alBufferData(buffer, getALFormatFromChannels(props.channelsCount), &data[0], samplesRead * sizeof(short), props.samplerate);
    }

    return true;
}

void SoundEngine::convertToMono(const he::PrimitiveList<short>& dataStereo, he::PrimitiveList<short>& dataMono)
{
    dataMono.clear();

    for (uint32 i(0); i < dataStereo.size() / 2; ++i)
    {
        dataMono.add(static_cast<short>((dataStereo[2 * i] + dataStereo[2 * i + 1]) * 0.5f));
    }
}

void SoundEngine::emptyBuffers(uint32 source)
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

ALenum SoundEngine::getALFormatFromChannels(uint32 channels) const
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

            LOG(LogType_ProgrammerAssert, "Creating OpenAL context failed!");
        }
    }
    else
    {
        LOG(LogType_ProgrammerAssert, "Init OpenAL device failed!");
    }

    alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
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

        if ( pSound->getLooping() &&
             m_SoundTime[pSound] >= pSound->getLength() )
        {
            pSound->play(true);

            m_SoundTime[pSound] = 0.0f;
        }
        else if (m_SoundTime[pSound] >= pSound->getLength())
        {
            stopSound(pSound);
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
                    if (streamSound(m_SoundFiles[pSound->getSoundFile()], buffer, pSound->getToMono()) == false)
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
    m_SoundFiles.add(SoundFile(path));

    SoundFile& soundFile = m_SoundFiles[m_SoundFiles.size() - 1];

    // try to open
    bool success(soundFile.open());

    std::string err("Failed to open sound file: ");
    err += path;

    HE_ASSERT(success == true, err.c_str());

    // get soundfile properties
    SoundFileProperties props(soundFile.getProperties());

    HE_ASSERT(props.channelsCount <= 2, "More channels than supported");

    if (stream == true)
    {
        // create source, buffer for file
        ALuint source(0);//, *buffers(NEW ALuint(STREAM_BUFFERS));
        AudioBuffer buffer;
        alGenSources(1, &source);
        alGenBuffers(STREAM_BUFFERS, buffer.buffers);

        m_SoundSources.add(source);
        m_SoundBuffers.add(buffer);

        // create sound
        pSound = NEW Sound2D((uint32)m_SoundSources.size() - 1, (uint32)m_SoundBuffers.size() - 1, (uint32)m_SoundFiles.size() - 1, SOUND_TYPE_STREAM);

        // put sound in soundbank
        m_SoundBank.add(pSound);
    }
    else
    {
        // create source, buffer for file
        ALuint source(0);
        AudioBuffer buffer;

        alGenSources(1, &source);
        alGenBuffers(1, buffer.buffers);

        m_SoundSources.add(source);
        m_SoundBuffers.add(buffer);

        // read data from soundfile
        he::PrimitiveList<short> data;
        data.resize(props.samplesCount);

        success = (soundFile.read(&data[0], props.samplesCount) == props.samplesCount);
        
        HE_ASSERT(success == true, err.c_str());

        // fill soundbuffer with data
        alBufferData(
            buffer.buffers[0],
            getALFormatFromChannels(props.channelsCount),
            &data[0],
            props.samplesCount * sizeof(short),
            props.samplerate );

        // bind buffer with source
        alSourceQueueBuffers(source, 1, buffer.buffers);

        // create sound
        pSound = NEW Sound2D((uint32)m_SoundSources.size() - 1, (uint32)m_SoundBuffers.size() - 1, (uint32)m_SoundFiles.size() - 1, SOUND_TYPE_STATIC);

        // put sound in soundbank
        m_SoundBank.add(pSound);
    }

    soundFile.close();

    // for 2D sound, position is relative to listener
    alSourcei(m_SoundSources[pSound->getSource()], AL_SOURCE_RELATIVE, AL_TRUE);

    // create soundtime, needed for looping
    m_SoundTime[pSound] = float(0.0f);

    return pSound;
}

Sound3D* SoundEngine::loadSound3D(const std::string& path, bool stream)
{
    stream = false; // only static for now

    Sound3D* pSound(nullptr);

    // new soundfile
    m_SoundFiles.add(SoundFile(path));

    SoundFile& soundFile = m_SoundFiles[m_SoundFiles.size() - 1];

    // try to open
    bool success(soundFile.open());

    std::string err("Failed to open sound file: ");
    err += path;

    HE_ASSERT(success == true, err.c_str());

    // get soundfile properties
    SoundFileProperties props(soundFile.getProperties());

    HE_ASSERT(props.channelsCount <= 2, "More channels than supported");

    if (stream == true)
    {
        // create source, buffer for file
        ALuint source(0);//, *buffers(NEW ALuint(STREAM_BUFFERS));
        AudioBuffer buffer;
        alGenSources(1, &source);
        alGenBuffers(STREAM_BUFFERS, buffer.buffers);

        m_SoundSources.add(source);
        m_SoundBuffers.add(buffer);

        // create sound
        pSound = NEW Sound3D((uint32)m_SoundSources.size() - 1, (uint32)m_SoundBuffers.size() - 1, (uint32)m_SoundFiles.size() - 1, SOUND_TYPE_STREAM);

        // put sound in soundbank
        m_SoundBank.add(pSound);
    }
    else
    {
        // create source, buffer for file
        ALuint source(0);//, buffer(0);
        AudioBuffer buffer;
        alGenSources(1, &source);
        alGenBuffers(1, buffer.buffers);

        m_SoundSources.add(source);
        m_SoundBuffers.add(buffer);

        // read data from soundfile
        he::PrimitiveList<short> data;
        data.resize(props.samplesCount);

        success = (soundFile.read(&data[0], props.samplesCount) == props.samplesCount);

        HE_ASSERT(success == true, err.c_str());

        if (props.channelsCount == 2)
        {
            // only mono can be used in 3D space
            he::PrimitiveList<short> dataMono;

            convertToMono(data, dataMono);

            // fill soundbuffer with data
            alBufferData(
                buffer.buffers[0],
                getALFormatFromChannels(1),
                &dataMono[0],
                (ALsizei)dataMono.size() * sizeof(short),
                props.samplerate);
        }
        else
        {
            // fill soundbuffer with data
            alBufferData(
                buffer.buffers[0],
                getALFormatFromChannels(1),
                &data[0],
                props.samplesCount * sizeof(short),
                props.samplerate );
        }


        // bind buffer with source
        alSourceQueueBuffers(source, 1, buffer.buffers);

        // create sound
        pSound = NEW Sound3D((uint32)m_SoundSources.size() - 1, (uint32)m_SoundBuffers.size() - 1, (uint32)m_SoundFiles.size() - 1, SOUND_TYPE_STATIC);

        // put sound in soundbank
        m_SoundBank.add(pSound);
    }

    soundFile.close();

    // for 3D sound, position is absolute to listener
    alSourcei(m_SoundSources[pSound->getSource()], AL_SOURCE_RELATIVE, AL_FALSE);

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
        //m_SoundFiles[pSound->getSoundFile()].seek(0);

        for (uint32 i(0); i < STREAM_BUFFERS; ++i)
        {
            if (!streamSound(m_SoundFiles[pSound->getSoundFile()], m_SoundBuffers[pSound->getBuffer()].buffers[i]))
                return;
        }

        ALuint source(m_SoundSources[pSound->getSource()]);
        //ALuint* buffers(m_SoundBuffers[pSound->getBuffer()]);
            
        alSourceQueueBuffers(source, STREAM_BUFFERS, &m_SoundBuffers[pSound->getBuffer()].buffers[0]);
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
void SoundEngine::setListenerVelocity(const vec3& vel)
{
    alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
}
void SoundEngine::setListenerOrientation(const vec3& forward, const vec3& up)
{
    float ori[6] = { forward.x, forward.y, forward.z,
                     up.x, up.y, up.z };

    alListenerfv(AL_ORIENTATION, ori);
}

/* GETTERS */
vec3 SoundEngine::getListenerPos() const
{
    vec3 pos;
    alGetListener3f(AL_POSITION, &pos.x, &pos.y, &pos.z);
        
    return pos;
}
vec3 SoundEngine::getListenerVelocity() const
{
    vec3 vel;
    alGetListener3f(AL_VELOCITY, &vel.x, &vel.y, &vel.z);

    return vel;
}
void SoundEngine::getListenerOrientation(vec3* forward, vec3* up) const
{
    float ori[6];
    alGetListenerfv(AL_ORIENTATION, ori);

    forward->x = ori[0];
    forward->y = ori[1];
    forward->z = ori[2];

    up->x = ori[3];
    up->y = ori[4];
    up->z = ori[5];
}

ALuint SoundEngine::getALSource(uint32 source) const
{
    return m_SoundSources[source];
}

const SoundEngine::AudioBuffer& SoundEngine::getALBuffer(uint32 buffer) const
{
    return m_SoundBuffers[buffer];
}

SoundFile& SoundEngine::getSoundFile(uint32 soundFile)
{
    return m_SoundFiles[soundFile];
}
float SoundEngine::getPlayTime(ISound* pSound)
{
    return m_SoundTime[pSound];
}

} } //end namespace