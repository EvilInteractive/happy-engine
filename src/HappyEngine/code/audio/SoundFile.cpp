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

#include "HappyPCH.h" 

#include "SoundFile.h"
#include "ContentManager.h"

namespace he {
namespace sfx {

/* CONSTRUCTOR - DESTRUCTOR */
SoundFile::SoundFile() :	
    m_FilePath(""),
    m_SoundFile(nullptr),
    m_NrSamples(0),
    m_Samplerate(0),
    m_ChannelsCount(0)
{
}

SoundFile::SoundFile(const std::string& filePath) :	m_FilePath(filePath),
                                                    m_SoundFile(nullptr),
                                                    m_NrSamples(0),
                                                    m_Samplerate(0),
                                                    m_ChannelsCount(0)
{
}

SoundFile::~SoundFile()
{
    close();
}

/* GENERAL */
bool SoundFile::open()
{
    close();

    SF_INFO fileInfo;
    std::string fullPath(CONTENT->getContentDir().str() + "audio/" + m_FilePath);
    m_SoundFile = sf_open(fullPath.c_str(), SFM_READ, &fileInfo);

    if (!m_SoundFile)
        return false;

    m_ChannelsCount = static_cast<uint32>(fileInfo.channels);
    m_NrSamples = static_cast<uint32>(fileInfo.frames) * m_ChannelsCount;
    m_Samplerate = static_cast<uint32>(fileInfo.samplerate);

    return true;
}

void SoundFile::close()
{
    if (m_SoundFile)
    {
        int err(sf_close(m_SoundFile));

        std::string errMsg("Can't close file: ");
        errMsg += getProperties().filePath;

        HE_ASSERT(err == 0, errMsg.c_str());

        m_SoundFile = nullptr;
    }
}

void SoundFile::seek(uint32 timeOffset)
{
    if (!m_SoundFile)
        return;

    sf_count_t frameOffset = static_cast<sf_count_t>(timeOffset * m_Samplerate / 1000);
    sf_seek(m_SoundFile, frameOffset, SEEK_SET);
}

uint32 SoundFile::read(short* pData, uint32 nrSamples)
{
    if (!m_SoundFile)
        open();

    uint32 readSamples(0);
    readSamples = static_cast<uint32>(sf_read_short(m_SoundFile, pData, static_cast<sf_count_t>(nrSamples)));

    return readSamples;
}

/* GETTERS */
SoundFileProperties SoundFile::getProperties() const
{
    SoundFileProperties prop;

    prop.filePath = m_FilePath;
    prop.channelsCount = m_ChannelsCount;
    prop.samplerate = m_Samplerate;
    prop.samplesCount = m_NrSamples;

    return prop;
}

/* DEFAULT COPY & ASSIGNMENT */
SoundFile::SoundFile(const SoundFile& second)
{
    m_SoundFile = second.m_SoundFile;
    m_FilePath = second.m_FilePath;
    m_NrSamples = second.m_NrSamples;
    m_Samplerate = second.m_Samplerate;
    m_ChannelsCount = second.m_ChannelsCount;
}

SoundFile& SoundFile::operator=(const SoundFile& second)
{
    m_SoundFile = second.m_SoundFile;
    m_FilePath = second.m_FilePath;
    m_NrSamples = second.m_NrSamples;
    m_Samplerate = second.m_Samplerate;
    m_ChannelsCount = second.m_ChannelsCount;

    return *this;
}

} } //end namespace