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
//Created: 08/11/2011

#ifndef _HE_SOUNDFILE_H_
#define _HE_SOUNDFILE_H_
#pragma once

#include <string>
#include "sndfile.h"
#include "HappyTypes.h"

namespace he {
namespace sfx {

struct SoundFileProperties
{
	std::string filePath;
	uint32 samplesCount;
	uint32 channelsCount;
	uint32 samplerate;

};

class SoundFile
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	SoundFile(const std::string& filePath);
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
	SNDFILE* m_pSoundFile;

	std::string m_FilePath;

	uint32 m_NrSamples;
	uint32 m_Samplerate;
	uint32 m_ChannelsCount;
};

} } //end namespace

#endif
