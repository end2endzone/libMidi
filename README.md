# libMidi #
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Github Releases](https://img.shields.io/github/release/end2endzone/libmidi.svg)](https://github.com/end2endzone/libmidi/releases)

libmidi is a C++ open-source library for generating single tone melodies (monolithic melodies) in MIDI format.


## Status ##

Build:

| Service | Build | Tests |
|----|-------|-------|
| AppVeyor | [![Build status](https://img.shields.io/appveyor/ci/end2endzone/libMidi/master.svg?logo=appveyor)](https://ci.appveyor.com/project/end2endzone/libmidi) | [![Tests status](https://img.shields.io/appveyor/tests/end2endzone/libmidi/master.svg?logo=appveyor)](https://ci.appveyor.com/project/end2endzone/libmidi/branch/master/tests) |
| Travis CI | [![Build Status](https://img.shields.io/travis/end2endzone/libMidi/master.svg?logo=travis&style=flat)](https://travis-ci.org/end2endzone/libMidi) |  |

Statistics:

| AppVeyor | Travic CI |
|----------|-----------|
| [![Statistics](https://buildstats.info/appveyor/chart/end2endzone/libmidi)](https://ci.appveyor.com/project/end2endzone/libmidi/branch/master) | [![Statistics](https://buildstats.info/travisci/chart/end2endzone/libMidi)](https://travis-ci.org/end2endzone/libMidi) |




# Purpose #

When one try to generate basic sounds and tone, the [Microsoft Beep function](http://msdn.microsoft.com/en-ca/library/windows/desktop/ms679277(v=vs.85).aspx) used to be the only option available. This API is quite old and is far from accurate when dealing with multiple short duration tones (less than 100ms). Also when you make two consecutive calls to the `Beep()` function, one can ear a tick sound between each calls.

One way to solve this issue is to use the MIDI sound format and use the [Microsoft Windows API to play a MIDI file](http://msdn.microsoft.com/en-us/library/windows/desktop/dd743673(v=vs.85).aspx).

The library is develop & design to be lightweight and have a very quick learning curve. It was designed as a mean to convert melodies made in tones (frequency and duration pairs) to MIDI sound format.

The library may be used on other platforms too for generating files which can be played using the platform's own media player.

Note that libMIDI library was not written to manipulate the MIDI sound format. There is no plan in improving/updating the library in order to support MIDI file parsing or supporting multiple tracks at once.




# Features #

The main features of the library are:

* Generate notes using a tone frequency and duration.
* Supports custom delays, volumes, melody name & instruments.
* Defines multiple speed requirements :
  * Ticks (or pulses) per quarters notes.
  * Tempo (microseconds per quarter note).
  * BPM (beats per minute).
* Simple unit conversion API:
  * BPM to/from Tempo.
  * Ticks to/from microseconds.




# Usage #

The following instructions show how to use the library with multiple source code examples:



## Create a melody ##

The following example shows how to create a MIDI from multiple tones (frequency and duration pairs).

This example creates the Nintendo's Mario Bros. 1-up sound in MIDI file format.

```cpp
int create_mario_1up()
{
  libmidi::MidiFile f;
 
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
```



## Play with instruments ##

The following example select a random piano instrument and play a melody.

```cpp
int play_random_piano()
{
  //find all piano instruments
  std::vector<std::string> piano_instruments;
  for(int8_t i=0; i<=127 && i >= 0; i++)
  {
    std::string name = libmidi::getInstrumentName(i);
    if (name.find("Piano") != std::string::npos)
      piano_instruments.push_back(name);
  }

  //pick a random piano instrument.
  int num_instruments = (int)piano_instruments.size();
  int selection = getRandomInt(0, num_instruments-1);
  const char * selected_piano_instrument = piano_instruments[selection].c_str();

  //create the melody
  libmidi::MidiFile f;

  f.setInstrument(libmidi::findInstrument(selected_piano_instrument));
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
```




# Build #

Please refer to file [INSTALL.md](INSTALL.md) for details on how installing/building the application.




# Platform #

libMidi has been tested with the following platform:

*   Linux x86/x64
*   Windows x86/x64




# Versioning #

We use [Semantic Versioning 2.0.0](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/end2endzone/libMidi/tags).




# Authors #

* **Antoine Beauchamp** - *Initial work* - [end2endzone](https://github.com/end2endzone)

See also the list of [contributors](https://github.com/end2endzone/libMidi/blob/master/AUTHORS) who participated in this project.




# License #

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

