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

#ifndef LIBMIDI_H
#define LIBMIDI_H

#include "libMidi\config.h"
#include "libMidi\version.h"

#include <stdint.h>
#include <vector>
#include <string>

/// <summary>
/// Defines the MidiFile class.
/// </summary>
class LIBMIDI_EXPORT MidiFile {
public:
  /// <summary>
  /// Predeclarations
  /// </summary>
  enum TRACK_ENDING_PREFERENCE;
  enum MIDI_TYPE;

  /// <summary>
  /// Construct a new instance of MidiFile.
  /// </summary>
	MidiFile(void);

  /// <summary>
  /// Sets the type of MIDI file.
  /// Supported values are defines by <typeparamref name="MIDI_TYPE">MIDI_TYPE</typeparamref>
  /// </summary>
  /// <remarks>
  /// The libMIDI library only supports a single track per MIDI file.
  /// Changing the MIDI type does not affect the capabilities of the library
  /// besides changing a single byte in the output file.
  /// </remarks>
  /// <param name="iType">The type of MIDI file.</param>
  void setMidiType(MIDI_TYPE iType);

  /// <summary>
  /// Sets the melody name.
  /// </summary>
  /// <param name="iName">The name of the melody. Set to NULL or EMPTY to disable name.</param>
  void setName(const char * iName);

  /// <summary>Set current volume for the following notes.</summary>
  /// <param name="iVolume">The volume value. min=0x00 max=0x7f</param>
  void setVolume(int8_t iVolume);

  /// <summary>Set current instrument.</summary>
  /// <param name="iInstrument">The instrument's id.</param>
  void setInstrument(int8_t iInstrument);

  /// <summary>Set the number of ticks per quarter note.</summary>
  /// <param name="iTicks">The ticks per quarter note.</param>
  void setTicksPerQuarterNote(uint16_t iTicks);

  /// <summary>Sets the number of beats per minute.</summary>
  /// <param name="iBpm">The number of beats per minute.</param>
  void setBeatsPerMinute(uint16_t iBpm);

  /// <summary>Sets the tempo in microseconds per quarter note.</summary>
  /// <param name="iTempo">The tempo in usec per second.</param>
  void setTempo(uint32_t iTempo);

  /// <summary>Defines if a MIDI track should end 
  /// with a <typeparamref name="STOP_PREVIOUS_NOTE">STOP_PREVIOUS_NOTE</typeparamref> message
  /// or a <typeparamref name="STOP_ALL_NOTES">STOP_ALL_NOTES</typeparamref> message.
  /// </summary>
  /// <param name="iTrackEndingPreference">The prefered track ending method.</param>
  void setTrackEndingPreference(TRACK_ENDING_PREFERENCE iTrackEndingPreference);

  /// <summary>Adds a note to the current melody.</summary>
  /// <param name="iFrequency">The frequency in Hz of the note.</param>
  /// <param name="iDurationMs">The duration of the note in milliseconds.</param>
  void addNote(uint16_t iFrequency, uint16_t iDurationMs);

  /// <summary>Adds a delay (silent note) to the current melody.</summary>
  /// <param name="iDurationMs">The delay duration in milliseconds.</param>
  void addDelay(uint16_t iDurationMs);

  /// <summary>Saves the current melody to a file.</summary>
  /// <param name="iFile">The path location where the file is to be saved.</param>
  /// <returns>True when the file is successfully saved. False otherwise.</returns>
  bool save(const char * iFile);

public:
  //public values & enums
  static const uint8_t  DEFAULT_INSTRUMENT = 0x00;
  static const uint16_t DEFAULT_BPM = 120;
  static const uint32_t DEFAULT_TEMPO = 500000;
  static const uint32_t DEFAULT_TICKS_PER_QUARTER_NOTE = 480;
  static const uint32_t MIN2USEC = 60*1000*1000;
  enum TRACK_ENDING_PREFERENCE
  {
    STOP_PREVIOUS_NOTE = 1,
    STOP_ALL_NOTES = 2,
  };
  enum MIDI_TYPE
  {
    MIDI_TYPE_0 = 0,
    MIDI_TYPE_1 = 1
  };

public:
  //static methods

  /// <summary>Convert a beat per minute (BPM) to a MIDI tempo.</summary>
  /// <param name="iBpm">The BPM value to convert.</param>
  /// <returns>A tempo value in BPM matching the given beat per minute.</returns>
  static uint32_t bpm2tempo(uint16_t iBpm);

  /// <summary>Converts a MIDI tempo to a beat per minute (BPM)</summary>
  /// <param name="iTempo">The tempo value to convert</param>
  /// <returns>A beat per minute value matching the given tempo.</returns>
  static uint16_t tempo2bpm(uint32_t iTempo);

  /// <summary>Converts a note duration to a number of ticks based on a given ticks per quarter note and a tempo.</summary>
  /// <param name="iDurationMs">The duration to convert in milliseconds.</param>
  /// <param name="iTicksPerQuarterNote">The given number of ticks per quarter notes.</param>
  /// <param name="iTempo">The given tempo.</param>
  /// <returns>A number of ticks matching the given duration.</returns>
  static uint16_t duration2ticks(uint16_t iDurationMs, uint16_t iTicksPerQuarterNote, uint32_t iTempo);

  /// <summary>Converts an amount of ticks to a duration.</summary>
  /// <param name="iTicks">The number of ticks to convert.</param>
  /// <param name="iTicksPerQuarterNote">The given number of ticks per quarter notes.</param>
  /// <param name="iTempo">The given tempo.</param>
  /// <returns>A duration in milliseconds matching the given number of ticks.</returns>
  static uint16_t ticks2duration(uint16_t iTicks, uint16_t iTicksPerQuarterNote, uint32_t iTempo);

  /// <summary>Finds a MIDI intrument id by name.</summary>
  /// <param name="iName">The given name of the intrument.</param>
  /// <returns>An identifier that matches the given instrument name.
  /// Returns a default instrument id if the name is unknown, NULL, or empty.
  /// </returns>
  static int8_t findInstrument(const char * iName);

  /// <summary>Get the instrument name that matches the given intrument id.</summary>
  /// <param name="iInstrument">The instrument id to search for.</param>
  /// <returns>Returns the instrument name matching the given id. Returns the name of the default instrument on invalid intrument id.</returns>
  static const char * getInstrumentName(int8_t iInstrument);

private:
  //private methods

  /// <summary>Converts a note duration to a number of ticks based on the melody's ticks per quarter note and tempo.</summary>
  /// <param name="iDurationMs">The duration to convert in milliseconds.</param>
  /// <returns>A number of ticks matching the given duration.</returns>
  uint16_t duration2ticks(uint16_t iDurationMs);

  /// <summary>Converts an amount of ticks to a duration based on the melody's ticks per quarter note and tempo.</summary>
  /// <param name="iTicks">The number of ticks to convert.</param>
  /// <returns>A duration in milliseconds matching the given number of ticks.</returns>
  uint16_t ticks2duration(uint16_t iTicks);

private:
  //private attributes
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
  int8_t mInstrument; //from 0x00 to 0x7f
  TRACK_ENDING_PREFERENCE mTrackEndingPreference;
  MIDI_TYPE mType;
};

#endif //LIBMIDI_H