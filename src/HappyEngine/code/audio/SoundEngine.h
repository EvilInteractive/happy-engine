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

#ifndef _HE_SOUND_ENGINE_H_
#define _HE_SOUND_ENGINE_H_
#pragma once

#include <string>
#include <map>
#include "al.h"
#include "alc.h"
#include "ogg/ogg.h"
#include "vorbis/vorbisfile.h"
#include "vorbis/codec.h"
#include "vec3.h"
#include "ISound.h"

namespace he {
namespace sfx {

class Sound2D;
class Sound3D;

class SoundEngine
{
public:

	static const uint STREAM_BUFFER_SIZE = 4096 * 8;
	static const uint STREAM_BUFFERS = 2;

	/* CONSTRUCTOR - DESTRUCTOR */
	SoundEngine();
    virtual ~SoundEngine();

	/* GENERAL */
	void initialize();
	void tick();
	void deleteAllSounds();

	Sound2D* loadSound2D(const std::string& path, bool stream = false);
	Sound3D* loadSound3D(const std::string& path, bool stream = false);

	void playSound(ISound* pSound, bool forceRestart = false);
	void stopSound(ISound* pSound);

	/* SETTERS */
	void setListenerPos(const vec3& pos);

	/* GETTERS */
	vec3 getListenerPos() const;

	ALuint getSource(uint source);
	ALuint* getBuffer(uint buffer);

private:

	void shutdown();

	uint loadOggStream(const std::string& path);
	bool streamOgg(OggVorbis_File& stream, ALuint buffer);
	void emptyBuffers(uint source);

	/* DATAMEMBERS */
	std::vector<ISound*> m_SoundBank;
	std::vector<ALuint*> m_SoundBuffers;
	std::vector<ALuint> m_SoundSources;
	std::vector<OggVorbis_File> m_SoundStreams;
	std::map<ISound*, uint> m_SoundStreamIndex;

	ALCcontext* m_pALContext;
	ALCdevice* m_pALDevice;

	uint m_BufferSize;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    SoundEngine(const SoundEngine&);
    SoundEngine& operator=(const SoundEngine&);
};

} } //end namespace

#endif
