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
#include "libmidi/pitches.h"
#include "libmidi/instruments.h"
#include "rapidassist/random.h"

using namespace ra::random;

int demo_create_mario_1up(int argc, char **argv)
{
  MidiFile f;
 
  f.setInstrument(0x51);  // "Lead 2 (sawtooth)"
  f.setTempo(0x051615);   // 333333 microseconds per quarter note
  f.setName("mario1up");
 
  //play mario 1-up melody.
  f.addNote(NOTE_E6, 125); // 1319 Hz
  f.addNote(NOTE_G6, 125); // 1568 Hz
  f.addNote(NOTE_E7, 125); // 2637 Hz
  f.addNote(NOTE_C7, 125); // 2093 Hz
  f.addNote(NOTE_D7, 125); // 2349 Hz
  f.addNote(NOTE_G7, 125); // 3136 Hz
 
  const char * filename = "mario1up.mid";
  bool saved = f.save(filename);
  if (!saved)
  {
    printf("Failed saving MIDI to file '%s'.\n", filename);
    return 1;
  }

  return 0;
}

int demo_play_random_piano(int argc, char **argv)
{
  //find all piano instruments
  std::vector<std::string> piano_instruments;
  for(int8_t i=0; i<=127 && i >= 0; i++)
  {
    std::string name = getInstrumentName(i);
    if (name.find("Piano") != std::string::npos)
      piano_instruments.push_back(name);
  }

  //pick a random piano instrument.
  int num_instruments = (int)piano_instruments.size();
  int selection = getRandomInt(0, num_instruments-1);
  const char * selected_piano_instrument = piano_instruments[selection].c_str();

  //create the melody
  MidiFile f;

  f.setInstrument(findInstrument(selected_piano_instrument));
  f.setTicksPerQuarterNote(0x80);
  f.addNote(NOTE_C4, 500); // 262 Hz
  f.addNote(NOTE_D4, 500); // 294 Hz
  f.addNote(NOTE_E4, 500); // 330 Hz

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

  //create_mario_1up
  return_code = demo_create_mario_1up(argc, argv);
  if (return_code != 0)
    return return_code;

  //play_random_piano
  return_code = demo_play_random_piano(argc, argv);
  if (return_code != 0)
    return return_code;

  return 0;
}
