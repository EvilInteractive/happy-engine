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
//Created: 08/11/2011

#ifndef _HE_SOUNDFILE_H_
#define _HE_SOUNDFILE_H_
#pragma once

#include "sndfile.h"

namespace he {
namespace sfx {

struct SoundFileProperties
{
    SoundFileProperties();
    SoundFileProperties(SoundFileProperties&& other);
    SoundFileProperties& operator=(SoundFileProperties&& other);

    he::String filePath;
    uint32 samplesCount;
    uint32 channelsCount;
    uint32 samplerate;

private:
    SoundFileProperties(const SoundFileProperties&);
    SoundFileProperties& operator=(const SoundFileProperties&);
};

class SoundFile
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    SoundFile();
    SoundFile(const he::String& filePath);
    virtual ~SoundFile();

    /* GENERAL */
    bool open();
    void close();
    void seek(uint32 timeOffset);
    uint32 read(short* pData, uint32 nrSamples);

    /* GETTERS */
    SoundFileProperties getProperties() const;

    /* DEFAULT COPY & ASSIGNMENT */
    SoundFile(const SoundFile& second);
    SoundFile& operator=(const SoundFile& second);

private:

    /* DATAMEMBERS */
    SNDFILE* m_SoundFile;

    he::String m_FilePath;

    uint32 m_NrSamples;
    uint32 m_Samplerate;
    uint32 m_ChannelsCount;
};

} } //end namespace

#endif
