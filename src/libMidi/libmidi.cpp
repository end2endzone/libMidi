/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

//
// Description:
//   Library for building a simple song based on 
//   single channel MIDI file.
//

#include "libmidi/libmidi.h"
#include "libmidi/pitches.h"
#include "libmidi/instruments.h"

#include "varlength.h"

namespace libmidi
{

typedef uint32_t HEADER_ID;
static const HEADER_ID MIDI_FILE_ID = 0x4d546864; //"MThd"
static const HEADER_ID MIDI_TRACK_HEADER_ID = 0x4d54726b; //"MTrk"

typedef uint16_t HEADER_MIDI_TYPE;

//events handling
typedef uint8_t EVENT_STATUS;
static const EVENT_STATUS NOTE_ON_CHANNEL_0           = (EVENT_STATUS)0x90;
static const EVENT_STATUS NOTE_OFF_CHANNEL_0          = (EVENT_STATUS)0x80;
static const EVENT_STATUS AFTER_TOUCH_CHANNEL_0       = (EVENT_STATUS)0xA0;
static const EVENT_STATUS CONTROL_CHANGE_CHANNEL_0    = (EVENT_STATUS)0xB0;
static const EVENT_STATUS PROGRAM_CHANGE_CHANNEL_0    = (EVENT_STATUS)0xC0;
static const EVENT_STATUS CHANNEL_PRESSURE_CHANNEL_0  = (EVENT_STATUS)0xD0;
static const EVENT_STATUS PITCH_WHEEL_CHANNEL_0       = (EVENT_STATUS)0xE0;
static const EVENT_STATUS EVENT_META                  = (EVENT_STATUS)0xFF;

typedef int8_t EVENT_PITCH; //note code
static const EVENT_PITCH ALL_NOTES_OFF = (EVENT_PITCH)0x7B;

typedef int8_t EVENT_VOLUME; //from 0 to 7F
static const EVENT_VOLUME MIN_VOLUME = (EVENT_VOLUME)0x00;
static const EVENT_VOLUME MAX_VOLUME = (EVENT_VOLUME)0x7F;

typedef int8_t META_TYPE;
static const META_TYPE META_SEQUENCE_NUMBER                = (META_TYPE)0x00;
static const META_TYPE META_TEXT_EVENT                     = (META_TYPE)0x01;
static const META_TYPE META_COPYRIGHT_NOTICE               = (META_TYPE)0x02;
static const META_TYPE META_SEQUENCE_OR_TRACK_NAME         = (META_TYPE)0x03;
static const META_TYPE META_INSTRUMENT_NAME                = (META_TYPE)0x04;
static const META_TYPE META_LYRIC_TEXT	                   = (META_TYPE)0x05;
static const META_TYPE META_MARKER_TEXT	                   = (META_TYPE)0x06;
static const META_TYPE META_CUE_POINT                      = (META_TYPE)0x07;
static const META_TYPE META_MIDI_CHANNEL_PREFIX_ASSIGNMENT = (META_TYPE)0x20;
static const META_TYPE META_END_OF_TRACK                   = (META_TYPE)0x2F;
static const META_TYPE META_TEMPO_SETTING                  = (META_TYPE)0x51;
static const META_TYPE META_SMPTE_OFFSET                   = (META_TYPE)0x54;
static const META_TYPE META_TIME_SIGNATURE                 = (META_TYPE)0x58;
static const META_TYPE META_KEY_SIGNATURE                  = (META_TYPE)0x59;
static const META_TYPE META_SEQUENCER_SPECIFIC_EVENT       = (META_TYPE)0x7F;

typedef uint32_t VAR_LENGTH;

#pragma pack(push, 1) // exact fit - no padding
struct MIDI_HEADER
{
  HEADER_ID id; //see DEFAULT_FILE_HEADER
  uint32_t length;
  HEADER_MIDI_TYPE type;
  uint16_t numTracks;
  uint16_t ticksPerQuarterNote;
};
struct TRACK_HEADER
{
  HEADER_ID id; //see DEFAULT_FILE_HEADER
  uint32_t length; //including data size & track footer
};
struct NOTE_EVENT
{
  VAR_LENGTH ticks;
  EVENT_STATUS status;
  EVENT_PITCH pitch;
  EVENT_VOLUME volume;
};
struct META_EVENT
{
  VAR_LENGTH ticks;
  EVENT_STATUS status;
  META_TYPE type;
  VAR_LENGTH size;
};
#pragma pack(pop) //back to whatever the previous packing mode was


EVENT_PITCH findMidiPitchFromFrequency(unsigned short frequency)
{
  //According to General MIDI Lite, v1.0,
  //section 3.1.7
  //available at https://www.midi.org/images/downloads/GML-v1.pdf

  if (frequency <= NOTE_C0)
    return 0x0C;

#define DECLARE_PITCH(midiPitchId, note_frequency) {midiPitchId, note_frequency},

  struct PITCH_NOTE_PAIR
  {
    EVENT_PITCH pitch;
    int16_t frequency;
  };
  static const PITCH_NOTE_PAIR pairs[] = {
    DECLARE_PITCH(0x7F, NOTE_G9)
    DECLARE_PITCH(0x7E, NOTE_GB9)
    DECLARE_PITCH(0x7D, NOTE_F9)
    DECLARE_PITCH(0x7C, NOTE_E9)
    DECLARE_PITCH(0x7B, NOTE_EB9)
    DECLARE_PITCH(0x7A, NOTE_D9)
    DECLARE_PITCH(0x79, NOTE_DB9)
    DECLARE_PITCH(0x78, NOTE_C9)
    DECLARE_PITCH(0x77, NOTE_B8)
    DECLARE_PITCH(0x76, NOTE_BB8)
    DECLARE_PITCH(0x75, NOTE_A8)
    DECLARE_PITCH(0x74, NOTE_AB8)
    DECLARE_PITCH(0x73, NOTE_G8)
    DECLARE_PITCH(0x72, NOTE_GB8)
    DECLARE_PITCH(0x71, NOTE_F8)
    DECLARE_PITCH(0x70, NOTE_E8)
    DECLARE_PITCH(0x6F, NOTE_EB8)
    DECLARE_PITCH(0x6E, NOTE_D8)
    DECLARE_PITCH(0x6D, NOTE_DB8)
    DECLARE_PITCH(0x6C, NOTE_C8)
    DECLARE_PITCH(0x6B, NOTE_B7)
    DECLARE_PITCH(0x6A, NOTE_BB7)
    DECLARE_PITCH(0x69, NOTE_A7)
    DECLARE_PITCH(0x68, NOTE_AB7)
    DECLARE_PITCH(0x67, NOTE_G7)
    DECLARE_PITCH(0x66, NOTE_GB7)
    DECLARE_PITCH(0x65, NOTE_F7)
    DECLARE_PITCH(0x64, NOTE_E7)
    DECLARE_PITCH(0x63, NOTE_EB7)
    DECLARE_PITCH(0x62, NOTE_D7)
    DECLARE_PITCH(0x61, NOTE_DB7)
    DECLARE_PITCH(0x60, NOTE_C7)
    DECLARE_PITCH(0x5F, NOTE_B6)
    DECLARE_PITCH(0x5E, NOTE_BB6)
    DECLARE_PITCH(0x5D, NOTE_A6)
    DECLARE_PITCH(0x5C, NOTE_AB6)
    DECLARE_PITCH(0x5B, NOTE_G6)
    DECLARE_PITCH(0x5A, NOTE_GB6)
    DECLARE_PITCH(0x59, NOTE_F6)
    DECLARE_PITCH(0x58, NOTE_E6)
    DECLARE_PITCH(0x57, NOTE_EB6)
    DECLARE_PITCH(0x56, NOTE_D6)
    DECLARE_PITCH(0x55, NOTE_DB6)
    DECLARE_PITCH(0x54, NOTE_C6)
    DECLARE_PITCH(0x53, NOTE_B5)
    DECLARE_PITCH(0x52, NOTE_BB5)
    DECLARE_PITCH(0x51, NOTE_A5)
    DECLARE_PITCH(0x50, NOTE_AB5)
    DECLARE_PITCH(0x4F, NOTE_G5)
    DECLARE_PITCH(0x4E, NOTE_GB5)
    DECLARE_PITCH(0x4D, NOTE_F5)
    DECLARE_PITCH(0x4C, NOTE_E5)
    DECLARE_PITCH(0x4B, NOTE_EB5)
    DECLARE_PITCH(0x4A, NOTE_D5)
    DECLARE_PITCH(0x49, NOTE_DB5)
    DECLARE_PITCH(0x48, NOTE_C5)
    DECLARE_PITCH(0x47, NOTE_B4)
    DECLARE_PITCH(0x46, NOTE_BB4)
    DECLARE_PITCH(0x45, NOTE_A4)
    DECLARE_PITCH(0x44, NOTE_AB4)
    DECLARE_PITCH(0x43, NOTE_G4)
    DECLARE_PITCH(0x42, NOTE_GB4)
    DECLARE_PITCH(0x41, NOTE_F4)
    DECLARE_PITCH(0x40, NOTE_E4)
    DECLARE_PITCH(0x3F, NOTE_EB4)
    DECLARE_PITCH(0x3E, NOTE_D4)
    DECLARE_PITCH(0x3D, NOTE_DB4)
    DECLARE_PITCH(0x3C, NOTE_C4)
    DECLARE_PITCH(0x3B, NOTE_B3)
    DECLARE_PITCH(0x3A, NOTE_BB3)
    DECLARE_PITCH(0x39, NOTE_A3)
    DECLARE_PITCH(0x38, NOTE_AB3)
    DECLARE_PITCH(0x37, NOTE_G3)
    DECLARE_PITCH(0x36, NOTE_GB3)
    DECLARE_PITCH(0x35, NOTE_F3)
    DECLARE_PITCH(0x34, NOTE_E3)
    DECLARE_PITCH(0x33, NOTE_EB3)
    DECLARE_PITCH(0x32, NOTE_D3)
    DECLARE_PITCH(0x31, NOTE_DB3)
    DECLARE_PITCH(0x30, NOTE_C3)
    DECLARE_PITCH(0x2F, NOTE_B2)
    DECLARE_PITCH(0x2E, NOTE_BB2)
    DECLARE_PITCH(0x2D, NOTE_A2)
    DECLARE_PITCH(0x2C, NOTE_AB2)
    DECLARE_PITCH(0x2B, NOTE_G2)
    DECLARE_PITCH(0x2A, NOTE_GB2)
    DECLARE_PITCH(0x29, NOTE_F2)
    DECLARE_PITCH(0x28, NOTE_E2)
    DECLARE_PITCH(0x27, NOTE_EB2)
    DECLARE_PITCH(0x26, NOTE_D2)
    DECLARE_PITCH(0x25, NOTE_DB2)
    DECLARE_PITCH(0x24, NOTE_C2)
    DECLARE_PITCH(0x23, NOTE_B1)
    DECLARE_PITCH(0x22, NOTE_BB1)
    DECLARE_PITCH(0x21, NOTE_A1)
    DECLARE_PITCH(0x20, NOTE_AB1)
    DECLARE_PITCH(0x1F, NOTE_G1)
    DECLARE_PITCH(0x1E, NOTE_GB1)
    DECLARE_PITCH(0x1D, NOTE_F1)
    DECLARE_PITCH(0x1C, NOTE_E1)
    DECLARE_PITCH(0x1B, NOTE_EB1)
    DECLARE_PITCH(0x1A, NOTE_D1)
    DECLARE_PITCH(0x19, NOTE_DB1)
    DECLARE_PITCH(0x18, NOTE_C1)
    DECLARE_PITCH(0x17, NOTE_B0)
    DECLARE_PITCH(0x16, NOTE_BB0)
    DECLARE_PITCH(0x15, NOTE_A0)
    DECLARE_PITCH(0x14, NOTE_AB0)
    DECLARE_PITCH(0x13, NOTE_G0)
    DECLARE_PITCH(0x12, NOTE_GB0)
    DECLARE_PITCH(0x11, NOTE_F0)
    DECLARE_PITCH(0x10, NOTE_E0)
    DECLARE_PITCH(0x0F, NOTE_EB0)
    DECLARE_PITCH(0x0E, NOTE_D0)
    DECLARE_PITCH(0x0D, NOTE_DB0)
    DECLARE_PITCH(0x0C, NOTE_C0)
    //DECLARE_PITCH(0x0B, NOTE_B)
    //DECLARE_PITCH(0x0A, NOTE_BB)
    //DECLARE_PITCH(0x09, NOTE_A)
    //DECLARE_PITCH(0x08, NOTE_AB)
    //DECLARE_PITCH(0x07, NOTE_G)
    //DECLARE_PITCH(0x06, NOTE_GB)
    //DECLARE_PITCH(0x05, NOTE_F)
    //DECLARE_PITCH(0x04, NOTE_E)
    //DECLARE_PITCH(0x03, NOTE_EB)
    //DECLARE_PITCH(0x02, NOTE_D)
    //DECLARE_PITCH(0x01, NOTE_DB)
    //DECLARE_PITCH(0x00, NOTE_C)
  };
#undef DECLARE_PITCH
  static const int16_t numPairs = sizeof(pairs)/sizeof(pairs[0]);

  //search for a perfect match
  for(int16_t i=0; i<numPairs; i++)
  {
    if (frequency == pairs[i].frequency)
      return pairs[i].pitch;
  }

  //not found. look for the closer one
  EVENT_PITCH closerPitch = 0x0C;
  int16_t minDiff = 0x7FFF;
  for(int16_t i=0; i<numPairs; i++)
  {
    //calculate frequency diff
    int16_t diff = abs(frequency - pairs[i].frequency);
    if (diff <= minDiff)
    {
      //closer match found
      closerPitch = pairs[i].pitch;
      minDiff = diff;
    }
  }
  return closerPitch;
}

template <typename T>
void swap_endian(T& u)
{
  if (sizeof(u) == 1)
    return;

  union
  {
    T u;
    unsigned char u8[sizeof(T)];
  } source, dest;

  source.u = u;

  for (size_t k = 0; k < sizeof(T); k++)
    dest.u8[k] = source.u8[sizeof(T) - k - 1];

  u = dest.u;
}

MidiFile::MidiFile()
{
  mTicksPerQuarterNote = MidiFile::DEFAULT_TICKS_PER_QUARTER_NOTE;
  mTempo = MidiFile::DEFAULT_TEMPO;
  mVolume = MAX_VOLUME;
  mInstrument = DEFAULT_INSTRUMENT;
  mTrackEndingPreference = STOP_PREVIOUS_NOTE;
  mType = MIDI_TYPE_0;
}

void MidiFile::addNote(uint16_t iFrequency, uint16_t iDurationMs)
{
  NOTE n;
  n.frequency = iFrequency;
  n.durationMs = iDurationMs;
  n.volume = mVolume;

  mNotes.push_back(n);
}

void MidiFile::addDelay(uint16_t iDurationMs)
{
  addNote(0, iDurationMs);
}

void MidiFile::setTicksPerQuarterNote(uint16_t iTicks)
{
  mTicksPerQuarterNote = iTicks;
}

void MidiFile::setBeatsPerMinute(uint16_t iBpm)
{
  mTempo = bpm2tempo(iBpm);
}

void MidiFile::setTempo(uint32_t iTempo)
{
  mTempo = iTempo;
}

void MidiFile::setName(const char * iName)
{
  if (iName == NULL)
    mName = "";
  else
    mName = iName;
}

void MidiFile::setVolume(int8_t iVolume)
{
  mVolume = iVolume;
  if (mVolume < MIN_VOLUME)
    mVolume = MIN_VOLUME;
  if (mVolume > MAX_VOLUME)
    mVolume = MAX_VOLUME;
}

void MidiFile::setInstrument(int8_t iInstrument)
{
  mInstrument = iInstrument;
  if (mInstrument < MIN_INSTRUMENT)
    mInstrument = MIN_INSTRUMENT;
  if (mInstrument > MAX_INSTRUMENT)
    mInstrument = MAX_INSTRUMENT;
}

void MidiFile::setTrackEndingPreference(TRACK_ENDING_PREFERENCE iTrackEndingPreference)
{
  mTrackEndingPreference = iTrackEndingPreference;
}

void MidiFile::setMidiType(MIDI_TYPE iType)
{
  mType = iType;
}

size_t fswapwrite(const MIDI_HEADER & iHeader, FILE * f)
{
  MIDI_HEADER tmp = iHeader;
  swap_endian(tmp.id);
  swap_endian(tmp.length);
  swap_endian(tmp.type);
  swap_endian(tmp.numTracks);
  swap_endian(tmp.ticksPerQuarterNote);
  return fwrite(&tmp, 1, sizeof(tmp), f);
}
size_t fswapwrite(const TRACK_HEADER & iHeader, FILE * f)
{
  TRACK_HEADER tmp = iHeader;
  swap_endian(tmp.id);
  swap_endian(tmp.length);
  return fwrite(&tmp, 1, sizeof(tmp), f);
}
size_t fwriteevent(const NOTE_EVENT & e, bool isRunningStatus, FILE * f)
{
  size_t writeSize = 0;

  writeSize += fwriteVariableLength(e.ticks, 1, f);
  if (!isRunningStatus)
  {
    writeSize += fwrite(&e.status, 1, sizeof(e.status), f);
  }
  writeSize += fwrite(&e.pitch, 1, sizeof(e.pitch), f);
  writeSize += fwrite(&e.volume, 1, sizeof(e.volume), f);

  return writeSize;
}
size_t fswapwrite(const META_EVENT & e, FILE * f)
{
  size_t writeSize = 0;

  writeSize += fwriteVariableLength(e.ticks, 1, f);
  writeSize += fwrite(&e.status, 1, sizeof(e.status), f);
  writeSize += fwrite(&e.type, 1, sizeof(e.type), f);
  writeSize += fwriteVariableLength(e.size, 1, f);

  return writeSize;
}
size_t fswapwrite(const HEADER_ID & id, FILE * f)
{
  HEADER_ID tmp = id;
  swap_endian(tmp);
  return fwrite(&tmp, 1, sizeof(tmp), f);
}

uint32_t MidiFile::bpm2tempo(uint16_t iBpm)
{
  //BPM 2 tempo
  uint32_t tempo = 1*MIN2USEC/((uint32_t)iBpm);
  return tempo;
}

uint16_t MidiFile::tempo2bpm(uint32_t iTempo)
{
  uint16_t bpm = 1*MIN2USEC/(iTempo);
  return bpm;
}

uint16_t MidiFile::duration2ticks(uint16_t iDurationMs, uint16_t iTicksPerQuarterNote, uint32_t iTempo)
{
  uint32_t noteticks = (1000*iDurationMs*iTicksPerQuarterNote)/iTempo;
  return (uint16_t)noteticks;
}

uint16_t MidiFile::ticks2duration(uint16_t iTicks, uint16_t iTicksPerQuarterNote, uint32_t iTempo)
{
  //formula: noteticks/mTicksPerQuarterNote*tempo/1000=notedurationMs
  uint32_t durationMs = (iTicks*iTempo)/(1000*iTicks);
  return (uint16_t)durationMs;
}

uint16_t MidiFile::duration2ticks(uint16_t iDurationMs)
{
  return duration2ticks(iDurationMs, mTicksPerQuarterNote, mTempo);
}

uint16_t MidiFile::ticks2duration(uint16_t iTicks)
{
  return ticks2duration(iTicks, mTicksPerQuarterNote, mTempo);
}

bool MidiFile::save(const char * iFile)
{
  MIDI_HEADER header;
  header.id = MIDI_FILE_ID;
  header.length = 6;
  header.type = (HEADER_MIDI_TYPE)mType;
  header.numTracks = 1;
  header.ticksPerQuarterNote = mTicksPerQuarterNote;

  TRACK_HEADER track;
  track.id = MIDI_TRACK_HEADER_ID;
  track.length = 0; //must be computed as we write data. The value will be written again once all the file is generated.

  FILE * fout = fopen(iFile, "wb");
  if (!fout)
    return false;

  //write midi file header
  fswapwrite(header, fout);

  //write track header
  long trackFileOffset = ftell(fout);
  fswapwrite(track, fout);

  if (mName != "")
  {
    META_EVENT e;
    e.ticks = 0;
    e.status = EVENT_META;
    e.type = META_SEQUENCE_OR_TRACK_NAME;
    e.size = mName.size();

    //dump
    track.length += fswapwrite(e, fout);
    track.length += fwrite(mName.c_str(), 1, e.size, fout);
  }

  //set a TEMPO
  if (mTempo != DEFAULT_TEMPO)
  {
    META_EVENT e;
    e.ticks = 0;
    e.status = EVENT_META;
    e.type = META_TEMPO_SETTING;
    e.size = 3;

    //dump
    track.length += fswapwrite(e, fout);

    //dump value
    uint32_t tempo = mTempo;
    swap_endian(tempo);
    char* tempoOut = (char*)&tempo;
    track.length += fwrite(&tempoOut[1], 1, 3, fout);
  }

  //set instrument
  if (mInstrument != DEFAULT_INSTRUMENT)
  {
    //Next to all TEMPO EVENT is the following 3 bytes which are still unknown
    unsigned char buffer[] = {0x00, 0xC0, (unsigned char)mInstrument};
    track.length += fwrite(buffer, 1, sizeof(buffer), fout);
  }

  uint16_t previousNoteTicks = 0;
  EVENT_PITCH previousPitch = 0;
  EVENT_STATUS previousStatus = 0;
  for(size_t i=0; i<mNotes.size(); i++)
  {
    const NOTE & n = mNotes[i];
    
    if (n.frequency)
    {
      //build an event for the note
      {
        NOTE_EVENT e;
        e.ticks = previousNoteTicks;
        e.status = NOTE_ON_CHANNEL_0;
        e.pitch = findMidiPitchFromFrequency(n.frequency);
        e.volume = n.volume;

        //dump
        bool isRunningStatus = (previousStatus == e.status);
        track.length += fwriteevent(e, isRunningStatus, fout);

        //remember status
        previousStatus = e.status;
        previousPitch = e.pitch;
        previousNoteTicks = duration2ticks(n.durationMs);
      }

      //if its the last note
      bool isLastNote = (i == mNotes.size()-1);
      if (isLastNote && (mTrackEndingPreference & STOP_ALL_NOTES) == STOP_ALL_NOTES)
      {
        //silence all notes
        NOTE_EVENT e;
        e.ticks = previousNoteTicks;
        e.status = CONTROL_CHANGE_CHANNEL_0;
        e.pitch = ALL_NOTES_OFF;
        e.volume = MIN_VOLUME;

        //dump
        bool isRunningStatus = (previousStatus == e.status);
        track.length += fwriteevent(e, isRunningStatus, fout);

        //remember status
        previousStatus = e.status;
        previousPitch = e.pitch;
        previousNoteTicks = 0; //next note shall begins right after this one
      }
      else
      {
        //more notes to come
        //now stop the note
        NOTE_EVENT e;
        e.ticks = previousNoteTicks;
        e.status = NOTE_OFF_CHANNEL_0;
        e.pitch = findMidiPitchFromFrequency(n.frequency);
        e.volume = mVolume;

        //dump
        bool isRunningStatus = (previousStatus == e.status);
        track.length += fwriteevent(e, isRunningStatus, fout);

        //remember status
        previousStatus = e.status;
        previousPitch = e.pitch;
        previousNoteTicks = 0; //next note shall begins right after this one
      }
    }
    else
    {
      //silenced delay
      previousNoteTicks = duration2ticks(n.durationMs);
    }
  }

  //add track footer
  {
    META_EVENT e;
    e.ticks = previousNoteTicks;
    e.status = EVENT_META;
    e.type = META_END_OF_TRACK;
    e.size = 0;

    //dump
    track.length += fswapwrite(e, fout);
  }

  //write TRACK headers again
  fseek(fout, trackFileOffset, SEEK_SET);
  fswapwrite(track, fout);

  fclose(fout);
  return true;
}

}; //namespace libmidi
