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

#include "Sound2D.h"
#include "HappyNew.h"
#include "SoundEngine.h"
#include "HappyEngine.h"

namespace he {
namespace sfx {

/* CONSTRUCTOR - DESTRUCTOR */
Sound2D::Sound2D(uint source, uint buffer, uint soundFile, SOUND_TYPE type) :	m_Source(source),
                                                                                m_Buffer(buffer),
                                                                                m_SoundFile(soundFile),
                                                                                m_Type(type),
                                                                                m_bLooping(false)
{
}

Sound2D::~Sound2D()
{
    
}

/* GENERAL */
void Sound2D::play(bool forceRestart)
{
    AUDIO->playSound(this, forceRestart);
}
void Sound2D::stop()
{
    AUDIO->stopSound(this);
}
void Sound2D::pause()
{
    alSourcePause(AUDIO->getALSource(m_Source));
}

/* SETTERS */
void Sound2D::setVolume(float volume)
{
    alSourcef(AUDIO->getALSource(m_Source), AL_GAIN, volume);
}
void Sound2D::setLooping(bool loop)
{
    m_bLooping = loop;
}
void Sound2D::setPitch(float pitch)
{
    alSourcef(AUDIO->getALSource(m_Source), AL_PITCH, pitch);
}

/* GETTERS */
uint Sound2D::getSource() const
{
    return m_Source;
}
uint Sound2D::getBuffer() const
{
    return m_Buffer;
}
uint Sound2D::getSoundFile() const
{
    return m_SoundFile;
}
SOUND_STATE Sound2D::getState() const
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
SOUND_TYPE Sound2D::getType() const
{
    return m_Type;
}
float Sound2D::getVolume() const
{
    float volume;
    alGetSourcef(AUDIO->getALSource(m_Source), AL_GAIN, &volume);

    return volume;
}
bool Sound2D::getLooping() const
{
    return m_bLooping;
}
float Sound2D::getPitch() const
{
    float pitch;
    alGetSourcef(AUDIO->getALSource(m_Source), AL_PITCH, &pitch);

    return pitch;
}
float Sound2D::getLength() const
{
    SoundFileProperties props(AUDIO->getSoundFile(m_SoundFile).getProperties());

    return (static_cast<float>(props.samplesCount / props.samplerate / props.channelsCount / getPitch()));
}

float Sound2D::getPlayTime()
{
    return AUDIO->getPlayTime(this);
}

bool Sound2D::getToMono() const
{
    return false;
}

} } //end namespace