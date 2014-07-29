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

#ifndef _HE_SOUND2D_H_
#define _HE_SOUND2D_H_
#pragma once

#include "AL/al.h"
#include "HappyTypes.h"
#include "ISound.h"

namespace he {
namespace sfx {

class Sound2D : public ISound
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Sound2D(uint32 source, uint32 buffer, uint32 soundFile, SOUND_TYPE type);

    virtual ~Sound2D();

    /* GENERAL */
    void play(bool forceRestart = false);
    void stop();
    void pause();

    /* SETTERS */
    void setVolume(float volume); // 0.0f = sound turned off, 1.0f = normal volume
    void setLooping(bool loop); // -1 = infinite looping, 0 = no looping
    void setPitch(float pitch = 1.0f); // 1 = normal pitch

    /* GETTERS */
    uint32 getSource() const;
    uint32 getBuffer() const;
    uint32 getSoundFile() const;
    SOUND_STATE getState() const;
    SOUND_TYPE getType() const;

    float getVolume() const;
    bool getLooping() const;
    float getPitch() const;

    float getLength() const;
    float getPlayTime();

    bool getToMono() const;

private:

    /* DATAMEMBERS */
    uint32 m_SoundFile;
    uint32 m_Buffer;
    uint32 m_Source;

    SOUND_TYPE m_Type;

    bool m_bLooping;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Sound2D(const Sound2D&);
    Sound2D& operator=(const Sound2D&);
};

} } //end namespace

#endif
