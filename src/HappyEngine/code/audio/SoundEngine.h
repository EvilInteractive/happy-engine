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

#ifndef _HE_SOUND_ENGINE_H_
#define _HE_SOUND_ENGINE_H_
#pragma once

#include "AL/al.h"
#include "AL/alc.h"
#include "ISound.h"
#include "SoundFile.h"

namespace he {
namespace sfx {

class Sound2D;
class Sound3D;

class SoundEngine
{
public:

    static const uint32 STREAM_BUFFER_SIZE = 4096;
    static const uint32 STREAM_BUFFERS = 4;

    struct AudioBuffer
    {
        ALuint buffers[STREAM_BUFFERS];

        AudioBuffer() { he_memset(buffers, 0, sizeof(ALuint) * STREAM_BUFFERS); }
        ~AudioBuffer() {}
    };

    /* CONSTRUCTOR - DESTRUCTOR */
    SoundEngine();
    virtual ~SoundEngine();

    /* GENERAL */
    void initialize();
    void tick(float dTime);
    void deleteAllSounds();

    Sound2D* loadSound2D(const he::String& path, bool stream = false);
    Sound3D* loadSound3D(const he::String& path, bool stream = false);

    void playSound(ISound* pSound, bool forceRestart = false);
    void stopSound(ISound* pSound);

    /* SETTERS */
    void setListenerPos(const vec3& pos);
    void setListenerVelocity(const vec3& vel);
    void setListenerOrientation(const vec3& forward, const vec3& up);

    /* GETTERS */
    vec3 getListenerPos() const;
    vec3 getListenerVelocity() const;
    void getListenerOrientation(vec3* forward, vec3* up) const;

    ALuint getALSource(uint32 source) const;
    const AudioBuffer& getALBuffer(uint32 buffer) const;
    SoundFile& getSoundFile(uint32 soundFile);

    float getPlayTime(ISound* pSound);

private:

    void shutdown();
    void emptyBuffers(uint32 source);

    bool streamSound(SoundFile& stream, ALuint buffer, bool toMono = false);
    void convertToMono(const he::PrimitiveList<short>& dataStereo, he::PrimitiveList<short>& dataMono);
    
    ALenum getALFormatFromChannels(uint32 channels) const;

    /* DATAMEMBERS */
    he::PrimitiveList<ISound*> m_SoundBank;
    he::ObjectList<AudioBuffer> m_SoundBuffers;
    he::PrimitiveList<ALuint> m_SoundSources;
    he::ObjectList<SoundFile> m_SoundFiles;
    he::Map<ISound*, float> m_SoundTime;

    ALCcontext* m_pALContext;
    ALCdevice* m_pALDevice;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    SoundEngine(const SoundEngine&);
    SoundEngine& operator=(const SoundEngine&);
};

} } //end namespace

#endif
