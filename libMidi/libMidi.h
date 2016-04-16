//
//  libMidi Library - v1.0 - 04/15/2016
//  Copyright (C) 2016 Antoine Beauchamp
//  The code & updates for the library can be found on http://end2endzone.com
//
// AUTHOR/LICENSE:
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3.0 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License (LGPL-3.0) for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// DISCLAIMER:
//  This software is furnished "as is", without technical support, and with no 
//  warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
//  This libMidi library allows one to create basic midi file based on
//  tone frequency & duration.
//
// HISTORY:
// 04/15/2016 v1.0 - Initial release.
//

#ifndef LIBMIDI_H
#define LIBMIDI_H

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBMIDI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBMIDI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef _WIN32
#ifdef LIBMIDI_EXPORTS
#define LIBMIDI_API __declspec(dllexport)
#else
#define LIBMIDI_API __declspec(dllimport)
#endif
#else
#define LIBMIDI_API
#endif

#include <vector>
#include <string>
#include "miditypes.h"

class LIBMIDI_API MidiFile {
public:
  enum TRACK_ENDING_PREFERENCE;
  enum MIDI_TYPE;

	MidiFile(void);
  void addNote(uint16_t iFrequency, uint16_t iDurationMs);
  void addDelay(uint16_t iDurationMs);
  void setTicksPerQuarterNote(uint16_t iTicks);
  void setBeatsPerMinute(uint16_t iBpm);
  void setTempo(uint32_t iTempo); //usec per quarter note
  void setName(const char * iName);
  void setVolume(int8_t iVolume);
  void setTrackEndingPreference(TRACK_ENDING_PREFERENCE iTrackEndingPreference);
  void setMidiType(MIDI_TYPE iType);
  bool save(const char * iFile);

public:
  static const uint16_t DEFAULT_BPM = 120;
  static const uint32_t DEFAULT_TEMPO = 500000;
  static const uint32_t DEFAULT_TICKS_PER_QUARTER_NOTE = 480;
  static const uint32_t MIN2USEC = 60*1000*1000;
  enum TRACK_ENDING_PREFERENCE
  {
    STOP_PREVIOUS_NOTE = 1,
    STOP_ALL_NOTES = 2,
    TRACK_FOOTER_TICKS = 4,
  };
  enum MIDI_TYPE
  {
    MIDI_TYPE_0 = 0,
    MIDI_TYPE_1 = 1
  };

private:
  uint16_t computeTicks(uint16_t iDurationMs);

private:
  struct NOTE
  {
    uint16_t frequency;
    uint16_t durationMs;
    int8_t volume;
  };
  typedef std::vector<NOTE> NoteList;
  uint16_t mTicksPerQuarterNote;
  uint32_t mTempo; //usec per quarter note
  std::string mName;
  NoteList mNotes;
  int8_t mVolume; //from 0x00 to 0x7f
  TRACK_ENDING_PREFERENCE mTrackEndingPreference;
  MIDI_TYPE mType;
};

#endif //LIBMIDI_H