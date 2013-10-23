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
//Created: 2013/10/10

#ifndef _HE_MidiEvents_H_
#define _HE_MidiEvents_H_
#pragma once

namespace he {
namespace io {

//////////////////////////////////////////////////////////////////////////
// Meta Events
//////////////////////////////////////////////////////////////////////////
    struct MidiMetaEventSequenceNumber
    {
        uint8 m_MSB;
        uint8 m_LSB;
    };

    struct MidiMetaEventText
    {
        size_t m_TextLength;
        const char* m_Text;
    };

    struct MidiMetaEventCopyrightNotice
    {
        size_t m_TextLength;
        const char* m_Text;
    };

    struct MidiMetaEventTrackName
    {
        size_t m_TextLength;
        const char* m_Text;
    };

    struct MidiMetaEventInstrumentName
    {
        size_t m_TextLength;
        const char* m_Text;
    };

    struct MidiMetaEventLyrics
    {
        size_t m_TextLength;
        const char* m_Text;
    };

    struct MidiMetaEventMarker
    {
        size_t m_TextLength;
        const char* m_Text;
    };

    struct MidiMetaEventCuePoint
    {
        size_t m_TextLength;
        const char* m_Text;
    };

    struct MidiMetaEventMidiChannelPrefix
    {
        uint8 m_Channel;
    };

    struct MidiMetaEventEndOfTrack
    {
    };

    struct MidiMetaEventSetTempo
    {
        static const int s_MicroSecondsPerMinute = 60000000;

        size_t getBeatsPerMinute() const { return s_MicroSecondsPerMinute / m_MPQN; }
        size_t getMicrosecondsPerQuarterNote() const { return m_MPQN; }

        size_t m_MPQN;
    };

    struct MidiMetaEventSMPTEOffset
    {
        uint8 m_Hour; // 0 - 23
        uint8 m_Min; // 0 - 59
        uint8 m_Sec; // 0 - 59
        uint8 m_Frame; // 0 - 30 (framerate dependant)
        uint8 m_SubFrame; // 0 - 99 (100 subframes per frame)
        uint8 m_FrameRate; // 24, 25, 29 (30 dropFrame), 30
    };

    /*
    Time Signature
    This meta event is used to set a sequences time signature. 
    The time signature defined with 4 bytes, a numerator, a denominator, a metronome pulse and number of 32nd notes per MIDI quarter-note. 
    The numerator is specified as a literal value, but the denominator is specified as (get ready) 
    the value to which the power of 2 must be raised to equal the number of subdivisions per whole note. 
    For example, a value of 0 means a whole note because 2 to the power of 0 is 1 (whole note), 
    a value of 1 means a half-note because 2 to the power of 1 is 2 (half-note), and so on. The metronome pulse specifies how often 
    the metronome should click in terms of the number of clock signals per click, which come at a rate of 24 per quarter-note. 
    For example, a value of 24 would mean to click once every quarter-note (beat) and a value of 48 would mean to click once every half-note (2 beats). 
    And finally, the fourth byte specifies the number of 32nd notes per 24 MIDI clock signals. This value is usually 8 
    because there are usually 8 32nd notes in a quarter-note. At least one Time Signature Event should appear in the first track chunk 
    (or all track chunks in a Type 2 file) before any non-zero delta time events. If one is not specified 4/4, 24, 8 should be assumed.
    */
    struct MidiMetaEventTimeSignature
    {
        uint8 m_Numerator;      // default 4
        uint8 m_Denominator;    // default 4  (read value ^2)
        uint8 m_MetronomeTicks; // default 24
        uint8 m_32nds;          // default 8
    };

    struct MidiMetaEventKeySignature
    {
        int8 m_Key; // -7 (#flats) -> 7 (#sharps)
        bool m_Scale; // false: major; true: minor
    };

//////////////////////////////////////////////////////////////////////////
// Channel Events
//////////////////////////////////////////////////////////////////////////

    ENUM(EMidiEventType, uint8)
    {
        eEventType_Invalid = 0x0,

        // Channel Events
        eEventType_NoteOff = 0x8,
        eEventType_NoteOn = 0x9,
        eEventType_NoteAftertouch = 0xA,
        eEventType_Controller = 0xB,
        eEventType_ProgramChange = 0xC,
        eEventType_ChannelAftertouch = 0xD,
        eEventType_PitchBend = 0xE,
        eEventType_Meta = 0xF,
        
        // Meta Events
        eEventType_SequenceNumber = 0x00 + eEventType_Meta,
        eEventType_Text = 0x01 + eEventType_Meta,
        eEventType_CopyrightNotice = 0x02 + eEventType_Meta,
        eEventType_TrackName = 0x03 + eEventType_Meta,
        eEventType_InstrumentName = 0x04 + eEventType_Meta,
        eEventType_Lyrics = 0x05 + eEventType_Meta,
        eEventType_Marker = 0x06 + eEventType_Meta,
        eEventType_CuePoint = 0x07 + eEventType_Meta,
        eEventType_MidiChannelPrefix = 0x20 + eEventType_Meta,
        eEventType_EndOfTrack = 0x2F + eEventType_Meta,
        eEventType_SetTempo = 0x51 + eEventType_Meta,
        eEventType_SMPTEOffset = 0x54 + eEventType_Meta,
        eEventType_TimeSignature = 0x58 + eEventType_Meta,
        eEventType_KeySignature = 0x59 + eEventType_Meta,
        eEventType_SequencerSpecific = 0x7F + eEventType_Meta,
    };

    // The Note On Event is used to signal when a MIDI key is pressed. This type of event has two parameters. 
    // The note number that specifies which of the 128 MIDI keys is being played and the velocity determines how fast/hard the key is pressed. 
    // The note number is normally used to specify the instruments musical pitch and the velocity is usually 
    // used to specify the instruments playback volume and intensity.
    struct MidiNoteOnEvent
    {
        int8 m_NoteNumber;
        int8 m_Velocity;
    };

    // The Note Off Event is used to signal when a MIDI key is released. 
    // These events have two parameters identical to a Note On event. 
    // The note number specifies which of the 128 MIDI keys is being played and the velocity determines how fast/hard the key was released. 
    // The note number is normally used to specify which previously pressed key is being released and the velocity is usually ignored, 
    // but is sometimes used to adjust the slope of an instrument's release phase
    struct MidiNoteOffEvent
    {
        int8 m_NoteNumber;
        int8 m_Velocity;
    };

    // The Note Aftertouch Event is used to indicate a pressure change on one of the currently pressed MIDI keys. 
    // It has two parameters. The note number of which key's pressure is changing and the aftertouch value which specifies 
    // amount of pressure being applied (0 = no pressure, 127 = full pressure). 
    // Note Aftertouch is used for extra expression of particular notes, often introducing or increasing some type of modulation 
    // during the instrument's sustain phase.
    struct MidiNoteAftertouchEvent
    {
        int8 m_NoteNumber;
        int8 m_AfterTouchValue;
    };

    // The Controller Event signals the change in a MIDI channels state. 
    // There are 128 controllers which define different attributes of the channel including volume, pan, 
    // modulation, effects, and more. This event type has two parameters. 
    // The controller number specifies which control is changing and the controller value defines it's new setting.
    struct MidiControllerEvent
    {
        enum EMidiControllerType
        {
            eMidiControllerType_BankSelect = 0x00,
            eMidiControllerType_Modulation = 0x01,
            eMidiControllerType_BreathController = 0x02,
            eMidiControllerType_FootController = 0x04,
            eMidiControllerType_PortamentoTime = 0x05,
            eMidiControllerType_DataEntry = 0x06, //(MSB)
            eMidiControllerType_MainVolume = 0x07,
            eMidiControllerType_Balance = 0x08,
            eMidiControllerType_Pan = 0x0A,
            eMidiControllerType_ExpressionController = 0x0B,
            eMidiControllerType_EffectControl1 = 0x0C,
            eMidiControllerType_EffectControl2 = 0x0D,
            eMidiControllerType_GeneralPurposeController1 = 0x10,
            eMidiControllerType_GeneralPurposeController2 = 0x11,
            eMidiControllerType_GeneralPurposeController3 = 0x12,
            eMidiControllerType_GeneralPurposeController4 = 0x13,
            eMidiControllerType_LSBControllers0 = 0x20,
            eMidiControllerType_LSBControllers1,
            eMidiControllerType_LSBControllers2,
            eMidiControllerType_LSBControllers3,
            eMidiControllerType_LSBControllers4,
            eMidiControllerType_LSBControllers5,
            eMidiControllerType_LSBControllers6,
            eMidiControllerType_LSBControllers7,
            eMidiControllerType_LSBControllers8,
            eMidiControllerType_LSBControllers9,
            eMidiControllerType_LSBControllers10,
            eMidiControllerType_LSBControllers11,
            eMidiControllerType_LSBControllers12,
            eMidiControllerType_LSBControllers13,
            eMidiControllerType_LSBControllers14,
            eMidiControllerType_LSBControllers15,
            eMidiControllerType_LSBControllers16,
            eMidiControllerType_LSBControllers17,
            eMidiControllerType_LSBControllers18,
            eMidiControllerType_LSBControllers19,
            eMidiControllerType_LSBControllers20,
            eMidiControllerType_LSBControllers21,
            eMidiControllerType_LSBControllers22,
            eMidiControllerType_LSBControllers23,
            eMidiControllerType_LSBControllers24,
            eMidiControllerType_LSBControllers25,
            eMidiControllerType_LSBControllers26,
            eMidiControllerType_LSBControllers27,
            eMidiControllerType_LSBControllers28,
            eMidiControllerType_LSBControllers29,
            eMidiControllerType_LSBControllers30,
            eMidiControllerType_LSBControllers31 = 0x3F,
            eMidiControllerType_DamperPedal = 0x40,// (sustain)
            eMidiControllerType_Portamento = 0x41,
            eMidiControllerType_Sostenuto = 0x42,
            eMidiControllerType_SoftPedal = 0x43,
            eMidiControllerType_LegatoFootswitch = 0x44,
            eMidiControllerType_Hold2 = 0x45,
            eMidiControllerType_SoundController1 = 0x46, //(default: Timber Variation)
            eMidiControllerType_SoundController2 = 0x47, //(default: Timber/Harmonic Content)
            eMidiControllerType_SoundController3 = 0x48, //(default: Release Time)
            eMidiControllerType_SoundController4 = 0x49, //(default: Attack Time)
            eMidiControllerType_SoundController6 = 0x4A,
            eMidiControllerType_SoundController7 = 0x4B,
            eMidiControllerType_SoundController8 = 0x4C,
            eMidiControllerType_SoundController9 = 0x4D,
            eMidiControllerType_SoundController10 = 0x4E,
            eMidiControllerType_GeneralPurposeController5 = 0x50,
            eMidiControllerType_GeneralPurposeController6 = 0x51,
            eMidiControllerType_GeneralPurposeController7 = 0x52,
            eMidiControllerType_GeneralPurposeController8 = 0x53,
            eMidiControllerType_PortamentoControl = 0x54,
            eMidiControllerType_Effects1Depth = 0x5B, //(formerly External Effects Depth)
            eMidiControllerType_Effects2Depth = 0x5C, //(formerly Tremolo Depth)
            eMidiControllerType_Effects3Depth = 0x5D, //(formerly Chorus Depth)
            eMidiControllerType_Effects4Depth = 0x5E, //(formerly Celeste Detune)
            eMidiControllerType_Effects5Depth = 0x5F, //(formerly Phaser Depth)
            eMidiControllerType_DataIncrement = 0x60,
            eMidiControllerType_DataDecrement = 0x61,
            eMidiControllerType_NonRegisteredParameterNumbeLSB = 0x62,
            eMidiControllerType_NonRegisteredParameterNumberMSB = 0x63,
            eMidiControllerType_RegisteredParameterNumberLSB = 0x64,
            eMidiControllerType_RegisteredParameterNumberMSB = 0x65,
            eMidiControllerType_ModeMessage1 = 0x79,
            eMidiControllerType_ModeMessage2 = 0x7A,
            eMidiControllerType_ModeMessage3 = 0x7B,
            eMidiControllerType_ModeMessage4 = 0x7C,
            eMidiControllerType_ModeMessage5 = 0x7D,
            eMidiControllerType_ModeMessage6 = 0x7E,
            eMidiControllerType_ModeMessage = 0x7F
        };
        int8 m_ControllerNumber;
        int8 m_ControllerValue;
    };

    // The Program Change Event is used to change which program (instrument/patch) should be played on the MIDI channel. 
    // This type of event takes only one parameter, the program number of the new instrument/patch.
    struct MidiProgramChangedEvent
    {
        int8 m_ProgramNumber;
    };

    // The Channel Aftertouch Event is similar to the Note Aftertouch message, 
    // except it effects all keys currently pressed on the specific MIDI channel. 
    // This type of event takes only one parameter, the aftertouch amount (0 = no pressure, 127 = full pressure).
    struct MidiChannelAftertouchEvent
    {
        int8 m_AfterTouchValue;
    };

    // The pitch value affects all playing notes on the current channel. 
    // Values below between ]0; s_Lowvalue] decrease the pitch, while values between ]0; s_HighValue] increase the pitch. 
    // The pitch range may vary from instrument to instrument, but is usually +/-2 semi-tones.
    struct MidiPitchBendEvent
    {
        const static int s_LowValue = -8192;
        const static int s_HighValue = 8192;
        int16 m_PitchBendValue;
    };


} } //end namespace

#endif
