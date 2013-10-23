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

#ifndef _HE_MidiReader_H_
#define _HE_MidiReader_H_
#pragma once

#include "MidiEvents.h"

namespace he {
namespace io {

class HAPPY_ENTRY MidiReader
{
    struct Header
    {
    public:
        Header();
        ~Header();

        uint16 getTrackCount() const { return m_Tracks; }

        bool isTicksPerBeat() const;
        uint16 getTimeDivisionInTickPerBeat() const;

        bool isFramesPerSecond() const { return !isTicksPerBeat(); }
        float getTimeDivisionFrameRate() const; // in frames / sec
        uint16 getTimeDivisionTicksPerFrame() const;

        bool load(BinaryVisitor* const visitor);
                
    private:
        uint16 m_Format;
        uint16 m_Tracks;
        uint16 m_TimeDivision;
    };
    struct MidiChannelEvent
    {
        int8 m_MidiChannel;

    };
    class TrackChunk;
    class EventData
    {
    public:
        EventData();
        ~EventData();

        size_t getDeltaTime() const { return m_DeltaTime; }
        int8 getMidiChannel() const { return m_MidiChannel; }
        EMidiEventType getEventType() const { return m_Type; } 

        bool load(TrackChunk* const parent, BinaryVisitor* const visitor);

    private:
        size_t m_DeltaTime;
        EMidiEventType m_Type;
        int8 m_MidiChannel;

        union Event
        {
            MidiNoteOffEvent m_NoteOff;
            MidiNoteOnEvent m_NoteOn;
            MidiNoteAftertouchEvent m_NoteAftertouch;
            MidiControllerEvent m_ControllerChanged;
            MidiProgramChangedEvent m_ProgramChanged;
            MidiChannelAftertouchEvent m_ChannelAftertouch;
            MidiPitchBendEvent m_PitchBend;

            MidiMetaEventSequenceNumber m_SequenceNumber;
            MidiMetaEventText m_Text;
            MidiMetaEventCopyrightNotice m_CopyrightNotice;
            MidiMetaEventTrackName m_TrackName;
            MidiMetaEventInstrumentName m_InstrumentName;
            MidiMetaEventLyrics m_Lyrics;
            MidiMetaEventMarker m_EventMarker;
            MidiMetaEventCuePoint m_CuePoint;
            MidiMetaEventMidiChannelPrefix m_MidiChannelPrefix;
            MidiMetaEventEndOfTrack m_EndOfTrack;
            MidiMetaEventSetTempo m_SetTempo;
            MidiMetaEventSMPTEOffset m_SMPTEOffset;
            MidiMetaEventTimeSignature m_TimeSignature;
            MidiMetaEventKeySignature m_KeySignature;
        } m_Event;

    };
    class TrackChunk
    {
        friend class EventData;
    public:
        TrackChunk();
        ~TrackChunk();
        TrackChunk(const TrackChunk& other);
        TrackChunk& operator=(const TrackChunk& other);

        const ObjectList<EventData>& getEventData() const { return m_EventData; }

        bool load(BinaryVisitor* const visitor);

    private:
        const char* readString( BinaryVisitor* const visitor, size_t eventLength );

        ObjectList<EventData> m_EventData;
        PrimitiveList<char*> m_TextPool;
    };
public:
    MidiReader();
    ~MidiReader();

    const Header& getHeader() { return m_Header; }
    const ObjectList<TrackChunk>& getTracks() const { return m_Tracks; }

    bool load(const he::Path& path);

private:
    Header m_Header;
    ObjectList<TrackChunk> m_Tracks;

    //Disable default copy constructor and default assignment operator
    MidiReader(const MidiReader&);
    MidiReader& operator=(const MidiReader&);
};

} } //end namespace

#endif
