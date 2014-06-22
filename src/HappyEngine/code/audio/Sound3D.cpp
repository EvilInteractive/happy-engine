//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#include "Sound3D.h"
#include "HappyNew.h"
#include "SoundEngine.h"
#include "HappyEngine.h"

namespace he {
namespace sfx {

/* CONSTRUCTOR - DESTRUCTOR */
Sound3D::Sound3D(uint32 source, uint32 buffer, uint32 soundFile, SOUND_TYPE type) :	m_Source(source),
                                                                                m_Buffer(buffer),
                                                                                m_SoundFile(soundFile),
                                                                                m_Type(type),
                                                                                m_bLooping(false)
{
}

Sound3D::~Sound3D()
{

}

/* GENERAL */
void Sound3D::play(bool forceRestart)
{
    AUDIO->playSound(this, forceRestart);
}
void Sound3D::stop()
{
    AUDIO->stopSound(this);
}
void Sound3D::pause()
{
    alSourcePause(AUDIO->getALSource(m_Source));
}

/* SETTERS */
void Sound3D::setVolume(float volume)
{
    alSourcef(AUDIO->getALSource(m_Source), AL_GAIN, volume);
}
void Sound3D::setLooping(bool loop)
{
    m_bLooping = loop;
}
void Sound3D::setPitch(float pitch)
{
    alSourcef(AUDIO->getALSource(m_Source), AL_PITCH, pitch);
}
void Sound3D::setPosition(const vec3& pos)
{
    alSource3f(AUDIO->getALSource(m_Source), AL_POSITION, pos.x, pos.y, pos.z);
}
void Sound3D::setVelocity(const vec3& vel)
{
    alSource3f(AUDIO->getALSource(m_Source), AL_VELOCITY, vel.x, vel.y, vel.z);
}
void Sound3D::setMinimumDistance(float distance)
{
    alSourcef(AUDIO->getALSource(m_Source), AL_REFERENCE_DISTANCE, distance);
}
void Sound3D::setMaximumDistance(float distance)
{
    alSourcef(AUDIO->getALSource(m_Source), AL_MAX_DISTANCE, distance);
}
void Sound3D::setRolloffFactor(float factor)
{
    alSourcef(AUDIO->getALSource(m_Source), AL_ROLLOFF_FACTOR, factor);
}
void Sound3D::setMinimumVolume(float volume)
{
    alSourcef(AUDIO->getALSource(m_Source), AL_MIN_GAIN, volume);
}
void Sound3D::setMaximumVolume(float volume)
{
    alSourcef(AUDIO->getALSource(m_Source), AL_MAX_GAIN, volume);
}

/* GETTERS */
uint32 Sound3D::getSource() const
{
    return m_Source;
}
uint32 Sound3D::getBuffer() const
{
    return m_Buffer;
}
uint32 Sound3D::getSoundFile() const
{
    return m_SoundFile;
}
SOUND_STATE Sound3D::getState() const
{
    SOUND_STATE state(SOUND_STATE_STOPPED);
    ALenum alState;

    alGetSourcei(AUDIO->getALSource(m_Source), AL_SOURCE_STATE, &alState);

    if (alState == AL_PLAYING)
        state = SOUND_STATE_PLAYING;
    else if (alState == AL_PAUSED)
        state = SOUND_STATE_PAUSED;

    return state;
}
SOUND_TYPE Sound3D::getType() const
{
    return m_Type;
}
float Sound3D::getVolume() const
{
    float volume;
    alGetSourcef(AUDIO->getALSource(m_Source), AL_GAIN, &volume);

    return volume;
}
bool Sound3D::getLooping() const
{
    return m_bLooping;
}
float Sound3D::getPitch() const
{
    float pitch;
    alGetSourcef(AUDIO->getALSource(m_Source), AL_PITCH, &pitch);

    return pitch;
}
float Sound3D::getLength() const
{
    SoundFileProperties props(AUDIO->getSoundFile(m_SoundFile).getProperties());

    return (static_cast<float>(props.samplesCount / props.samplerate / props.channelsCount / getPitch()));
}

float Sound3D::getPlayTime()
{
    return AUDIO->getPlayTime(this);
}
vec3 Sound3D::getPosition() const
{
    vec3 pos;
    alGetSource3f(AUDIO->getALSource(m_Source), AL_POSITION, &pos.x, &pos.y, &pos.z);

    return pos;
}
vec3 Sound3D::getVelocity() const
{
    vec3 vel;
    alGetSource3f(AUDIO->getALSource(m_Source), AL_VELOCITY, &vel.x, &vel.y, &vel.z);

    return vel;
}
float Sound3D::getMinimumDistance() const
{
    float min;
    alGetSourcef(AUDIO->getALSource(m_Source), AL_REFERENCE_DISTANCE, &min);

    return min;
}
float Sound3D::getMaximumDistance() const
{
    float max;
    alGetSourcef(AUDIO->getALSource(m_Source), AL_MAX_DISTANCE, &max);

    return max;
}
float Sound3D::getRolloffFactor() const
{
    float factor;
    alGetSourcef(AUDIO->getALSource(m_Source), AL_ROLLOFF_FACTOR, &factor);

    return factor;
}
float Sound3D::getMinimumVolume() const
{
    float vol;
    alGetSourcef(AUDIO->getALSource(m_Source), AL_MIN_GAIN, &vol);

    return vol;
}
float Sound3D::getMaximumVolume() const
{
    float vol;
    alGetSourcef(AUDIO->getALSource(m_Source), AL_MAX_GAIN, &vol);

    return vol;
}

bool Sound3D::getToMono() const
{
    return true;
}

} } //end namespace