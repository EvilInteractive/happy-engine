////HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
////
////This file is part of HappyEngine.
////
////    HappyEngine is free software: you can redistribute it and/or modify
////    it under the terms of the GNU Lesser General Public License as published by
////    the Free Software Foundation, either version 3 of the License, or
////    (at your option) any later version.
////
////    HappyEngine is distributed in the hope that it will be useful,
////    but WITHOUT ANY WARRANTY; without even the implied warranty of
////    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
////    GNU Lesser General Public License for more details.
////
////    You should have received a copy of the GNU Lesser General Public License
////    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
////
////Author:  Sebastiaan Sprengers
////Created: 03/10/2011
//
//#include "StdAfx.h"
//
//#include "Sound3D.h"
//#include "HappyNew.h"
//#include "ogg/ogg.h"
//#include "vorbis/codec.h"
//
//namespace he {
//namespace sfx {
//
///* CONSTRUCTOR - DESTRUCTOR */
//Sound3D::Sound3D(	SoundEngine* pEngine,
//							const OggVorbis_File& oggStream) :	m_pSoundEngine(pEngine),
//																m_OggStream(oggStream),
//																m_State(STATE_STOPPED)
//{
//	alGenBuffers(2, m_AlSourceBuffer);
//	alGenSources(1, &m_AlSource);
//}
//
//Sound3D::~Sound3D()
//{
//	stop();
//
//	emptyBuffers();
//
//	alDeleteSources(1, &m_AlSource);
//	alDeleteBuffers(2, m_AlSourceBuffer);
//
//	ov_clear(&m_OggStream);
//
//	m_pSoundEngine = nullptr;
//}
//
//bool Sound3D::stream(ALuint buffer)
//{
//	char data[BUFFER_SIZE];
//	int size(0);
//	int section(0);
//	int result(0);
//
//	while (size < BUFFER_SIZE)
//	{
//		result = ov_read(&m_OggStream, data + size, BUFFER_SIZE - size, 0, 2, 1, &section);
//
//		if (result > 0)
//			size += result;
//		else if (result == 0)
//			break;
//	}
//
//	if (size == 0)
//		return false;
//
//	vorbis_info vorbisInfo(*ov_info(&m_OggStream, -1));
//	ALenum format;
//
//	if(vorbisInfo.channels == 1)
//        format = AL_FORMAT_MONO16;
//    else
//        format = AL_FORMAT_STEREO16;
//
//	alGetError();
//
//	alBufferData(buffer, format, data, size, vorbisInfo.rate);
//
//	ALenum error = alGetError();
//	
//	if (error == AL_INVALID_ENUM)
//		ASSERT(false, "Error filling buffer");
//
//	return true;
//}
//
//void Sound3D::emptyBuffers()
//{
//	int queued;
//
//	alGetSourcei(m_AlSource, AL_BUFFERS_QUEUED, &queued);
//    
//    while(queued--)
//    {
//        ALuint buffer;
//    
//        alSourceUnqueueBuffers(m_AlSource, 1, &buffer);
//    }
//}
//
///* GENERAL */
//void Sound3D::play()
//{
//	if (!stream(m_AlSourceBuffer[0]))
//		return;
//	if (!stream(m_AlSourceBuffer[1]))
//		return;
//
//	alSourceQueueBuffers(m_AlSource, 2, m_AlSourceBuffer);
//
//	alSourcePlay(m_AlSource);
//}
//void Sound3D::stop()
//{
//	alSourceStop(m_AlSource);
//}
//void Sound3D::pause()
//{
//}
//void Sound3D::tick()
//{
//	ALenum state;
//	alGetSourcei(m_AlSource, AL_SOURCE_STATE, &state);
//
//	if (state == AL_PLAYING)
//		m_State = STATE_PLAYING;
//	else if (state == AL_STOPPED)
//		m_State = STATE_STOPPED;
//	else if (state == AL_PAUSED)
//		m_State = STATE_PAUSED;
//
//	if (m_State != STATE_PLAYING)
//		return;
//
//	int processed(0);
//
//	alGetSourcei(m_AlSource, AL_BUFFERS_PROCESSED, &processed);
//
//	while(processed--)
//	{
//		ALuint buffer;
//
//		alSourceUnqueueBuffers(m_AlSource, 1, &buffer);
//
//		stream(buffer);
//
//		alSourceQueueBuffers(m_AlSource, 1, &buffer);
//	}
//
//	vec3 lPos(m_pSoundEngine->getListenerPos());
//	alSource3f(m_AlSource, AL_POSITION, lPos.x, lPos.y, lPos.z);
//}
//
///* SETTERS */
//void Sound3D::setVolume(float /*volume*/)
//{
//}
//void Sound3D::setMinVolume(float /*minVolume*/)
//{
//}
//void Sound3D::setMaxVolume(float /*maxVolume*/)
//{
//}
//void Sound3D::setPanning(float /*leftPercentage*/)
//{
//}
//void setLooping(int /*nrLoops*/)
//{
//}
//
///* GETTERS */
//Sound3D::STATE Sound3D::getState() const
//{
//	return m_State;
//}
//float Sound3D::getVolume() const
//{
//	return 0;
//}
//float Sound3D::getLength() const
//{
//	return 0;
//}
//
//} } //end namespace