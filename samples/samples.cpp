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

#include <stdio.h>
#include <iostream>

#include "libmidi/libmidi.h"
#include "rapidassist/random.h"

using namespace ra::random;

int demo_mario1up(int argc, char **argv)
{
  MidiFile f;
 
  f.setMidiType(MidiFile::MIDI_TYPE_0);
  f.setInstrument(0x51);  // "Lead 2 (sawtooth)"
  f.setTempo(0x051615);   // 333333 microseconds per quarter note
  f.setName("mario1up");
  f.setVolume(0x64);      // 100%
 
  //play mario 1-up melody.
  f.addNote(1319,125);  // E6
  f.addNote(1568,125);  // G6
  f.addNote(2637,125);  // E7
  f.addNote(2093,125);  // C7
  f.addNote(2349,125);  // D7
  f.addNote(3136,125);  // G7
 
  const char * filename = "mario1up.mid";
  bool saved = f.save(filename);
  if (!saved)
  {
    printf("Failed saving MIDI to file '%s'.\n", filename);
    return 1;
  }

  return 0;
}

int demo_piano(int argc, char **argv)
{
  //find all piano instruments
  std::vector<std::string> piano_instruments;
  for(int8_t i=0; i<=127 && i >= 0; i++)
  {
    std::string name = MidiFile::getInstrumentName(i);
    if (name.find("Piano") != std::string::npos)
      piano_instruments.push_back(name);
  }

  //find a random piano instrument.
  int num_instruments = (int)piano_instruments.size();
  int selection = getRandomInt(0, num_instruments-1);
  const char * selected_piano_instrument = piano_instruments[selection].c_str();

  //create the melody
  MidiFile f;

  f.setInstrument(MidiFile::findInstrument(selected_piano_instrument));
  f.setTicksPerQuarterNote(0x80);
  f.setTrackEndingPreference(MidiFile::STOP_ALL_NOTES);
  f.addNote(262, 500); //C4
  f.addNote(294, 500); //D4
  f.addNote(330, 500); //E4

  const char * filename = "piano.mid";
  bool saved = f.save(filename);
  if (!saved)
  {
    printf("Failed saving MIDI to file '%s'.\n", filename);
    return 1;
  }

  return 0;
}

int main(int argc, char **argv)
{
  int return_code = 0;

  return_code = demo_mario1up(argc, argv);
  if (return_code != 0)
    return return_code;

  return_code = demo_piano(argc, argv);
  if (return_code != 0)
    return return_code;

  return 0;
}
