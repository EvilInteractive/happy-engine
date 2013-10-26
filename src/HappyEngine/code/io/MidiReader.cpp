//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Author:  Bastian Damman
//Created: 2013/10/07
#include "HappyPCH.h" 

#include "MidiReader.h"
#include "BinaryFileVisitor.h"

namespace he {
namespace io {

namespace
{
    void readVariableLength(he::io::BinaryVisitor* const visitor, size_t& outValue)
    {
        outValue = 0;
        for (;;)
        {
            uint8 a;
            visitor->visit(a);
            outValue <<= 7; 
            outValue |= (a & 0x7F);
            if ((a & 0x80) == 0)
            {
                break;
            }
        }
    }
}

MidiReader::MidiReader()
{
}


MidiReader::~MidiReader()
{
}

bool MidiReader::load( const he::Path& path )
{
    BinaryFileVisitor visitor;
    if (visitor.openRead(path, true))
    {
        if (m_Header.load(&visitor) == false)
        {
            visitor.close();
            return false;
        }
        const uint16 trackCount(m_Header.getTrackCount());
        for (uint16 i(0); i < trackCount; ++i)
        {
            m_Tracks.add(TrackChunk());
            TrackChunk& track(m_Tracks.back());
            if (track.load(&visitor) == false)
            {
                visitor.close();
                return false;
            }
        }
    }
    else
    {
        LOG(LogType_ProgrammerAssert, "File not found when trying to read midi file %s", path.str().c_str());
        visitor.close();
        return false;
    }
    visitor.close();
    return true;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
MidiReader::Header::Header()
{

}

MidiReader::Header::~Header()
{

}

bool MidiReader::Header::isTicksPerBeat() const
{
    return (m_TimeDivision & 0x8000) == 0;
}

he::uint16 MidiReader::Header::getTimeDivisionInTickPerBeat() const
{
    HE_ASSERT(isTicksPerBeat(), "Getting time division in ticks per beat when we are not"); 
    return m_TimeDivision & 0x7fff;
}

float MidiReader::Header::getTimeDivisionFrameRate() const
{
    HE_ASSERT(isFramesPerSecond(), "Getting time division in frames per second when we are not"); 
    const uint16 tempResult(m_TimeDivision & 0x7F00);
    if (tempResult == 29)
    {
        return 29.97f;
    }
    else
    {
        return static_cast<float>(tempResult);
    }
}

he::uint16 MidiReader::Header::getTimeDivisionTicksPerFrame() const
{
    HE_ASSERT(isFramesPerSecond(), "Getting time division in frames per second when we are not"); 
    return m_TimeDivision & 0x00ff;
}

bool MidiReader::Header::load( BinaryVisitor* const visitor )
{
    uint32 chunkID(0);
    visitor->visit(chunkID);
    if (chunkID != 0x4D546864)
    {
        LOG(LogType_ProgrammerAssert, "Chunk ID is not == 0x4D546864 ('MThd') it is 0x%X, file is corrupt!", chunkID);
        return false;
    }
    uint32 chunkSize(0);
    visitor->visit(chunkSize);
    if (chunkSize != 6)
    {
        LOG(LogType_ProgrammerAssert, "Chunk size of the header != 6! This is not expected, file is corrupt");
        return false;
    }

    visitor->visit(m_Format);
    visitor->visit(m_Tracks);
    visitor->visit(m_TimeDivision);

    return true;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
MidiReader::TrackChunk::TrackChunk()
{

}

MidiReader::TrackChunk::~TrackChunk()
{
    const size_t stringCount(m_TextPool.size());
    for (size_t i(0); i < stringCount; ++i)
    {
        he_free(m_TextPool[i]);
    }
}

MidiReader::TrackChunk::TrackChunk(const TrackChunk& other)
{
    HE_ASSERT(other.m_EventData.size() == 0, "Trying to copy non empty track! This should not happen for efficienty");
}

MidiReader::TrackChunk& MidiReader::TrackChunk::operator=( const TrackChunk& other )
{
    HE_ASSERT(other.m_EventData.size() == 0, "Trying to copy non empty track! This should not happen for efficienty");
    return *this;
}

bool MidiReader::TrackChunk::load( BinaryVisitor* const visitor )
{
    uint32 chunkID(0);
    visitor->visit(chunkID);
    if (chunkID != 0x4D54726B)
    {
        LOG(LogType_ProgrammerAssert, "Chunk ID is not == 0x4D54726B ('MTrk'), file is corrupt!");
        return false;
    }
    
    uint32 chunkSize(0);
    visitor->visit(chunkSize);

    const size_t readBytes(visitor->getProcessedBytes());

    while (visitor->getProcessedBytes() - readBytes < chunkSize)
    {
        m_EventData.add(EventData());
        EventData& data(m_EventData.back());
        if (data.load(this, visitor) == false)
        {
            return false;
        }
    }

    return true;
}

const char* MidiReader::TrackChunk::readString( BinaryVisitor* const visitor, size_t eventLength )
{
    char* buff(static_cast<char*>(he_malloc(eventLength + 1)));
    m_TextPool.add(buff);
    visitor->visitBlob(buff, eventLength);
    std::reverse(buff, buff + eventLength);
    buff[eventLength] = '\0';
    return buff;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
MidiReader::EventData::EventData()
    : m_DeltaTime(0)
    , m_Type(eEventType_Invalid)
    , m_MidiChannel(-1)
{
    he_memset(&m_Event, 0, sizeof(Event));
}

MidiReader::EventData::~EventData()
{

}
bool MidiReader::EventData::load( TrackChunk* const parent, BinaryVisitor* const visitor )
{
    {
        readVariableLength(visitor, m_DeltaTime);
    }
    {
        uint8 data;
        visitor->visitBlob(&data, 1);
        m_Type = checked_numcast<EMidiEventType>((data & 0xF0) >> 4);
        m_MidiChannel = (data & 0x0F);
        switch (m_Type)
        {
        case eEventType_NoteOff: 
            {
                MidiNoteOffEvent ev;
                visitor->visit(ev.m_NoteNumber);
                visitor->visit(ev.m_Velocity);
                m_Event.m_NoteOff = ev;
            } break;
        case eEventType_NoteOn:
            {
                MidiNoteOnEvent ev;
                visitor->visit(ev.m_NoteNumber);
                visitor->visit(ev.m_Velocity);
                m_Event.m_NoteOn = ev;
            } break;
        case eEventType_NoteAftertouch:
            {
                MidiNoteAftertouchEvent ev;
                visitor->visit(ev.m_NoteNumber);
                visitor->visit(ev.m_AfterTouchValue);
                m_Event.m_NoteAftertouch = ev;
            } break;
        case eEventType_Controller:
            {
                MidiControllerEvent ev;
                visitor->visit(ev.m_ControllerNumber);
                visitor->visit(ev.m_ControllerValue);
                m_Event.m_ControllerChanged = ev;
            } break;
        case eEventType_ProgramChange:
            {
                MidiProgramChangedEvent ev;
                visitor->visit(ev.m_ProgramNumber);
                m_Event.m_ProgramChanged = ev;
            } break;
        case eEventType_ChannelAftertouch:
            {
                MidiChannelAftertouchEvent ev;
                visitor->visit(ev.m_AfterTouchValue);
                m_Event.m_ChannelAftertouch = ev;
            } break;
        case eEventType_PitchBend:
            {
                uint8 lsb, msb;
                visitor->visit(lsb);
                visitor->visit(msb);
                MidiPitchBendEvent ev;
                ev.m_PitchBendValue = ((msb & 0x7F) << 7) | (lsb & 0x7F);
                HE_ASSERT(ev.m_PitchBendValue <= MidiPitchBendEvent::s_HighValue && 
                          ev.m_PitchBendValue >= MidiPitchBendEvent::s_LowValue, "Invalid pitch value! %d", ev.m_PitchBendValue );
                m_Event.m_PitchBend = ev;
            } break;
        case eEventType_Meta:
            {
                m_MidiChannel = -1;
                if (data == 0xFF)
                {
                    uint8 metaType(0);
                    visitor->visit(metaType);
                    m_Type = checked_numcast<EMidiEventType>(eEventType_Meta + metaType);
                    size_t eventLength(0);
                    readVariableLength(visitor, eventLength);
                    switch (m_Type)
                    {
                    case he::io::eEventType_SequenceNumber:
                        {
                            MidiMetaEventSequenceNumber ev;
                            visitor->visit(ev.m_LSB);
                            visitor->visit(ev.m_MSB);
                            m_Event.m_SequenceNumber = ev;
                        } break;
                    case he::io::eEventType_Text:
                        {
                            MidiMetaEventText ev;
                            ev.m_TextLength = eventLength;
                            ev.m_Text = parent->readString(visitor, eventLength);
                            m_Event.m_Text = ev;
                        } break;
                    case he::io::eEventType_CopyrightNotice:
                        {
                            MidiMetaEventCopyrightNotice ev;
                            ev.m_TextLength = eventLength;
                            ev.m_Text = parent->readString(visitor, eventLength);
                            m_Event.m_CopyrightNotice = ev;
                        } break;
                    case he::io::eEventType_TrackName:
                        {
                            MidiMetaEventTrackName ev;
                            ev.m_TextLength = eventLength;
                            ev.m_Text = parent->readString(visitor, eventLength);
                            m_Event.m_TrackName = ev;
                        } break;
                    case he::io::eEventType_InstrumentName:
                        {
                            MidiMetaEventInstrumentName ev;
                            ev.m_TextLength = eventLength;
                            ev.m_Text = parent->readString(visitor, eventLength);
                            m_Event.m_InstrumentName = ev;
                        } break;
                    case he::io::eEventType_Lyrics:
                        {
                            MidiMetaEventLyrics ev;
                            ev.m_TextLength = eventLength;
                            ev.m_Text = parent->readString(visitor, eventLength);
                            m_Event.m_Lyrics = ev;
                        } break;
                    case he::io::eEventType_Marker:
                        {
                            MidiMetaEventMarker ev;
                            ev.m_TextLength = eventLength;
                            ev.m_Text = parent->readString(visitor, eventLength);
                            m_Event.m_EventMarker = ev;
                        } break;
                    case he::io::eEventType_CuePoint:
                        {
                            MidiMetaEventCuePoint ev;
                            ev.m_TextLength = eventLength;
                            ev.m_Text = parent->readString(visitor, eventLength);
                            m_Event.m_CuePoint = ev;
                        } break;
                    case he::io::eEventType_MidiChannelPrefix:
                        {
                            MidiMetaEventMidiChannelPrefix ev;
                            HE_ASSERT(eventLength == 1, "Channel event has wrong size!");
                            visitor->visit(ev.m_Channel);
                            m_Event.m_MidiChannelPrefix = ev;
                        } break;
                    case he::io::eEventType_EndOfTrack:
                        {
                            HE_ASSERT(eventLength == 0, "End of track event has wrong size!");
                            m_Event.m_EndOfTrack = MidiMetaEventEndOfTrack();
                        } break;
                    case he::io::eEventType_SetTempo:
                        {
                            HE_ASSERT(eventLength == 3, "Set Tempo event has wrong size!");
                            MidiMetaEventSetTempo ev;
                            ev.m_MPQN = 0;
                            visitor->visitBlob(&ev.m_MPQN, 3);
                            m_Event.m_SetTempo = ev;
                        } break;
                    case he::io::eEventType_SMPTEOffset:
                        {
                            HE_ASSERT(eventLength == 5, "SMPTEOffset event has wrong size!");
                            MidiMetaEventSMPTEOffset ev;
                            visitor->visit(ev.m_Hour);
                            visitor->visit(ev.m_Min);
                            visitor->visit(ev.m_Sec);
                            visitor->visit(ev.m_Frame);
                            visitor->visit(ev.m_SubFrame);
                            ev.m_FrameRate = ev.m_Hour & 0x60;
                            switch (ev.m_FrameRate)
                            {
                            case 0: ev.m_FrameRate = 24; break;
                            case 1: ev.m_FrameRate = 25; break;
                            case 2: ev.m_FrameRate = 29; break;
                            case 3: ev.m_FrameRate = 30; break;
                            default:
                                LOG(LogType_ProgrammerAssert, "Unexpected frame rate type");
                                break;
                            }
                            ev.m_Hour &= 0x1F;
                            m_Event.m_SMPTEOffset = ev;
                        } break;
                    case he::io::eEventType_TimeSignature:
                        {
                            HE_ASSERT(eventLength == 4, "TimeSignature event has wrong size!");
                            MidiMetaEventTimeSignature ev;
                            visitor->visit(ev.m_Numerator);
                            visitor->visit(ev.m_Denominator);
                            ev.m_Denominator *= ev.m_Denominator;
                            visitor->visit(ev.m_MetronomeTicks);
                            visitor->visit(ev.m_32nds);
                            m_Event.m_TimeSignature = ev;
                        } break;
                    case he::io::eEventType_KeySignature:
                        {
                            HE_ASSERT(eventLength == 2, "TimeSignature event has wrong size!");
                            MidiMetaEventKeySignature ev;
                            visitor->visit(ev.m_Key);
                            visitor->visit(ev.m_Scale);
                            m_Event.m_KeySignature = ev;
                        } break;
                    case he::io::eEventType_SequencerSpecific:
                        {
                            visitor->skipBytes(eventLength);
                        } break;
                    default:
                        visitor->skipBytes(eventLength);
                        LOG(LogType_ProgrammerAssert, "Found an invalid midi meta event! Trying to skip.");
                        return false;
                    }
                }
                else
                {
                    size_t eventLength(0);
                    readVariableLength(visitor, eventLength);
                    visitor->skipBytes(eventLength);
                }
            } break;
        default:
            {
                LOG(LogType_ProgrammerAssert, "Found an invalid midi event! File corrupt.");
                return false;
            }
        }

    }
    return true;
}


} } //end namespace
