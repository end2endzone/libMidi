// libMidi.cpp : Defines the exported functions for the DLL application.
//



//
// Description:
//   Library for building a simple songle channel MIDI file.
//   Based on the following:
//      http://kevinboone.net/javamidi.html
//      http://www.ccarh.org/courses/253/handout/smf/
//      http://www.skytopia.com/project/articles/midi.html
//      http://people.virginia.edu/~pdr4h/pitch-freq.html
//      http://www.phy.mtu.edu/~suits/notefreqs.html
//      http://stackoverflow.com/questions/5288593/how-to-convert-midi-timeline-into-the-actual-timeline-that-should-be-played/5297236#5297236
//      http://www.lastrayofhope.co.uk/2009/12/23/midi-delta-time-ticks-to-seconds/2/
//


#include "libMidi.h"
#include "miditypes.h"
#include "varlength.h"

typedef uint32_t HEADER_ID;
static const HEADER_ID MIDI_FILE_ID = 0x4d546864; //"MThd"
static const HEADER_ID MIDI_TRACK_HEADER_ID = 0x4d54726b; //"MTrk"
static const HEADER_ID MIDI_TRACK_FOOTER_ID = 0x00ff2f00;

typedef uint16_t MIDI_TYPE;
static const MIDI_TYPE MIDI_TYPE_0 = 0;
static const MIDI_TYPE MIDI_TYPE_1 = 1;

//events handling
typedef int8_t EVENT_TIMESTAMP;
static const EVENT_TIMESTAMP TIMESTAMP_MULTIPLICATOR_MASK = (char)0x80;
typedef int8_t EVENT_STATUS;
static const EVENT_STATUS NOTE_ON_CHANNEL_0 = (char)0x90;
static const EVENT_STATUS CONTROLCHANGE_B_CHANNEL_0 = (char)0xB0;
static const EVENT_STATUS EVENT_META = (char)0xFF;
typedef int8_t EVENT_PITCH; //note code
static const EVENT_PITCH ALL_NOTES_OFF = (char)0x7B;
typedef int8_t EVENT_VOLUME; //from 0 to 7F
static const EVENT_VOLUME MIN_VOLUME = (char)0x00;
static const EVENT_VOLUME MAX_VOLUME = (char)0x7F;

typedef int8_t META_TYPE;
static const META_TYPE META_SEQUENCE_NUMBER                = (char)0x00;
static const META_TYPE META_TEXT_EVENT                     = (char)0x01;
static const META_TYPE META_COPYRIGHT_NOTICE               = (char)0x02;
static const META_TYPE META_SEQUENCE_OR_TRACK_NAME         = (char)0x03;
static const META_TYPE META_INSTRUMENT_NAME                = (char)0x04;
static const META_TYPE META_LYRIC_TEXT	                   = (char)0x05;
static const META_TYPE META_MARKER_TEXT	                   = (char)0x06;
static const META_TYPE META_CUE_POINT                      = (char)0x07;
static const META_TYPE META_MIDI_CHANNEL_PREFIX_ASSIGNMENT = (char)0x20;
static const META_TYPE META_END_OF_TRACK                   = (char)0x2F;
static const META_TYPE META_TEMPO_SETTING                  = (char)0x51;
static const META_TYPE META_SMPTE_OFFSET                   = (char)0x54;
static const META_TYPE META_TIME_SIGNATURE                 = (char)0x58;
static const META_TYPE META_KEY_SIGNATURE                  = (char)0x59;
static const META_TYPE META_SEQUENCER_SPECIFIC_EVENT       = (char)0x7F;
typedef int8_t META_SIZE;

typedef uint32_t VAR_LENGTH;

#pragma pack(push, 1) // exact fit - no padding
struct MIDI_HEADER
{
  HEADER_ID id; //see DEFAULT_FILE_HEADER
  uint32_t length;
  MIDI_TYPE type;
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
//struct META_TEMPO_EVENT
//{
//  VAR_LENGTH ticks;
//  EVENT_STATUS status;
//  META_TYPE type;
//  int8_t unknown; //always 3
//  uint64_t tempo; //6 bytes
//};
#pragma pack(pop) //back to whatever the previous packing mode was






/*************************************************
 * Notes Constants
 *************************************************/
#define NOTE_REST 0

#define NOTE_C0  16
#define NOTE_CS0 17
#define NOTE_D0  18
#define NOTE_DS0 19
#define NOTE_E0  21
#define NOTE_F0  22
#define NOTE_FS0 23
#define NOTE_G0  24
#define NOTE_GS0 26
#define NOTE_A0  28
#define NOTE_AS0 29
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define NOTE_E8  5274
#define NOTE_F8  5588
#define NOTE_FS8 5920
#define NOTE_G8  6272
#define NOTE_GS8 6645
#define NOTE_A8  7040
#define NOTE_AS8 7459
#define NOTE_B8  7902
#define NOTE_C9  8372
#define NOTE_CS9 8870
#define NOTE_D9  9397
#define NOTE_DS9 9956
#define NOTE_E9  10548
#define NOTE_F9  11175
#define NOTE_FS9 11840
#define NOTE_G9  12544
#define NOTE_GS9 13290
#define NOTE_A9   ?
#define NOTE_AS9  ?
#define NOTE_B9   ?

//Duplicated note with same frequency.
//Obtained from the following code:
//  for(char o='0'; o<='9'; o++)
//  {
//    for(char letter='A'; letter<='G'; letter++)
//    {
//      //     #define NOTE_DB7 NOTE_CS7
//      printf("#define NOTE_%cB%c NOTE_%cS%c\n", (letter+1 == 'H'? 'A' : letter+1), o, letter, o);
//    }
//  }
#define NOTE_BB0 NOTE_AS0
#define NOTE_CB0 NOTE_BS0
#define NOTE_DB0 NOTE_CS0
#define NOTE_EB0 NOTE_DS0
#define NOTE_FB0 NOTE_ES0
#define NOTE_GB0 NOTE_FS0
#define NOTE_AB0 NOTE_GS0
#define NOTE_BB1 NOTE_AS1
#define NOTE_CB1 NOTE_BS1
#define NOTE_DB1 NOTE_CS1
#define NOTE_EB1 NOTE_DS1
#define NOTE_FB1 NOTE_ES1
#define NOTE_GB1 NOTE_FS1
#define NOTE_AB1 NOTE_GS1
#define NOTE_BB2 NOTE_AS2
#define NOTE_CB2 NOTE_BS2
#define NOTE_DB2 NOTE_CS2
#define NOTE_EB2 NOTE_DS2
#define NOTE_FB2 NOTE_ES2
#define NOTE_GB2 NOTE_FS2
#define NOTE_AB2 NOTE_GS2
#define NOTE_BB3 NOTE_AS3
#define NOTE_CB3 NOTE_BS3
#define NOTE_DB3 NOTE_CS3
#define NOTE_EB3 NOTE_DS3
#define NOTE_FB3 NOTE_ES3
#define NOTE_GB3 NOTE_FS3
#define NOTE_AB3 NOTE_GS3
#define NOTE_BB4 NOTE_AS4
#define NOTE_CB4 NOTE_BS4
#define NOTE_DB4 NOTE_CS4
#define NOTE_EB4 NOTE_DS4
#define NOTE_FB4 NOTE_ES4
#define NOTE_GB4 NOTE_FS4
#define NOTE_AB4 NOTE_GS4
#define NOTE_BB5 NOTE_AS5
#define NOTE_CB5 NOTE_BS5
#define NOTE_DB5 NOTE_CS5
#define NOTE_EB5 NOTE_DS5
#define NOTE_FB5 NOTE_ES5
#define NOTE_GB5 NOTE_FS5
#define NOTE_AB5 NOTE_GS5
#define NOTE_BB6 NOTE_AS6
#define NOTE_CB6 NOTE_BS6
#define NOTE_DB6 NOTE_CS6
#define NOTE_EB6 NOTE_DS6
#define NOTE_FB6 NOTE_ES6
#define NOTE_GB6 NOTE_FS6
#define NOTE_AB6 NOTE_GS6
#define NOTE_BB7 NOTE_AS7
#define NOTE_CB7 NOTE_BS7
#define NOTE_DB7 NOTE_CS7
#define NOTE_EB7 NOTE_DS7
#define NOTE_FB7 NOTE_ES7
#define NOTE_GB7 NOTE_FS7
#define NOTE_AB7 NOTE_GS7
#define NOTE_BB8 NOTE_AS8
#define NOTE_CB8 NOTE_BS8
#define NOTE_DB8 NOTE_CS8
#define NOTE_EB8 NOTE_DS8
#define NOTE_FB8 NOTE_ES8
#define NOTE_GB8 NOTE_FS8
#define NOTE_AB8 NOTE_GS8
#define NOTE_BB9 NOTE_AS9
#define NOTE_CB9 NOTE_BS9
#define NOTE_DB9 NOTE_CS9
#define NOTE_EB9 NOTE_DS9
#define NOTE_FB9 NOTE_ES9
#define NOTE_GB9 NOTE_FS9
#define NOTE_AB9 NOTE_GS9



EVENT_PITCH findMidiPitchFromFrequency(unsigned short frequency)
{
#define DECLARE_PITCH(midiPitchId, note_frequency) \
  case note_frequency:  \
    return midiPitchId;

  switch(frequency)
  {
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
    default:
      return 0x0C;
  };
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
}

void MidiFile::addNote(uint16_t iFrequency, uint16_t iDurationMs)
{
  NOTE n;
  n.frequency = iFrequency;
  n.durationMs = iDurationMs;

  mNotes.push_back(n);
}

void MidiFile::setTicksPerQuarterNote(uint16_t iTicks)
{
  mTicksPerQuarterNote = iTicks;
}

void MidiFile::setBeatsPerMinute(uint16_t iBpm)
{
  //BPM 2 tempo
  mTempo = 1*MIN2USEC/((uint32_t)iBpm);
}

void MidiFile::setTempo(uint32_t iTempo)
{
  mTempo = iTempo;
}

void MidiFile::setName(const char * iName)
{
  mName = iName;
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

  //NOTE_EVENT tmp = e;
  
  writeSize += fwriteVariableLength(e.ticks, 1, f);

  if (!isRunningStatus)
  {
    //swap_endian(tmp.status   );
    writeSize += fwrite(&e.status, 1, sizeof(e.status), f);
  }

  //swap_endian(tmp.pitch   );
  writeSize += fwrite(&e.pitch, 1, sizeof(e.pitch), f);

  //swap_endian(tmp.volume   );
  writeSize += fwrite(&e.volume, 1, sizeof(e.volume), f);

  return writeSize;
}
size_t fswapwrite(const META_EVENT & e, FILE * f)
{
  size_t writeSize = 0;

  //META_EVENT tmp = e;

  writeSize += fwriteVariableLength(e.ticks, 1, f);
  writeSize += fwrite(&e.status, 1, sizeof(e.status), f);
  writeSize += fwrite(&e.type, 1, sizeof(e.type), f);
  writeSize += fwriteVariableLength(e.size, 1, f);

  return writeSize;
}
//size_t fswapwrite(const META_TEMPO_EVENT & e, FILE * f)
//{
//  size_t writeSize = 0;
//
//  writeSize += fwriteVariableLength(e.ticks, 1, f);
//  writeSize += fwrite(&e.status, 1, sizeof(e.status), f);
//  writeSize += fwrite(&e.type, 1, sizeof(e.type), f);
//  writeSize += fwrite(&e.unknown, 1, sizeof(e.unknown), f);
//  writeSize += fwrite(&e.tempo, 1, 6, f);
//
//  return writeSize;
//}
size_t fswapwrite(const HEADER_ID & id, FILE * f)
{
  HEADER_ID tmp = id;
  swap_endian(tmp);
  return fwrite(&tmp, 1, sizeof(tmp), f);
}

//void computeAutoTempo()
//{
//  mTempo = 0x0a2c2b;
//}

uint16_t MidiFile::computeTicks(uint16_t iDurationMs)
{
  //formula: noteticks/mTicksPerQuarterNote*tempo/1000=notedurationMs
  uint32_t noteticks = (1000*iDurationMs*mTicksPerQuarterNote)/mTempo;
  return (uint16_t)noteticks;
}

bool MidiFile::save(const char * iFile)
{
  MIDI_HEADER h;
  h.id = MIDI_FILE_ID;
  h.length = 6;
  h.type = MIDI_TYPE_1;
  h.numTracks = 1;
  h.ticksPerQuarterNote = mTicksPerQuarterNote;

  TRACK_HEADER t;
  t.id = MIDI_TRACK_HEADER_ID;
  t.length = 0; //must be computed as we write data. The value will be written again once all the file is generated. ~4*mNotes.size() + 4 /*NoteStop*/ + sizeof(MIDI_TRACK_FOOTER_ID);

  FILE * fout = fopen(iFile, "wb");
  if (!fout)
    return false;

  //write header
  fswapwrite(h, fout);

  //write header
  fswapwrite(t, fout);

  if (mName != "")
  {
    META_EVENT e;
    e.ticks = 0;
    e.status = EVENT_META;
    e.type = META_SEQUENCE_OR_TRACK_NAME;
    e.size = mName.size();

    //dump
    t.length += fswapwrite(e, fout);
    t.length += fwrite(mName.c_str(), 1, e.size, fout);
  }

  //force a TEMPO
  {
    META_EVENT e;
    e.ticks = 0;
    e.status = EVENT_META;
    e.type = META_TEMPO_SETTING;
    e.size = 3;

    //dump
    t.length += fswapwrite(e, fout);

    //dump value
    uint32_t tempo = mTempo;
    swap_endian(tempo);
    char* tempoOut = (char*)&tempo;
    t.length += fwrite(&tempoOut[1], 1, 3, fout);

    //Next to all TEMPO EVENT is the following 3 bytes which are still unknown
    static unsigned char buffer[] = {0x00, 0xC0, 0x51};
    t.length += fwrite(buffer, 1, sizeof(buffer), fout);
  }

  uint16_t previousNoteTicks = 0;
  for(size_t i=0; i<mNotes.size(); i++)
  {
    const NOTE & n = mNotes[i];

    //build an event for the note
    //if (i == 0)
    //{
    //  NOTE_EVENT e;
    //  e.ticks = noteTicks;
    //  e.status = NOTE_ON_CHANNEL_0;
    //  e.pitch = findMidiPitchFromFrequency(n.frequency);
    //  e.volume = 0x64;

    //  //dump
    //  t.length += fwriteevent(e, false, fout);
    //}
    //else
    //{
    //  NOTE_EVENT e;
    //  e.ticks = noteTicks;
    //  e.pitch = findMidiPitchFromFrequency(n.frequency);
    //  e.volume = 0x64;

    //  //dump
    //  t.length += fwriteevent(e, true, fout);
    //}
    {
      NOTE_EVENT e;
      e.ticks = previousNoteTicks;
      e.status = NOTE_ON_CHANNEL_0;
      e.pitch = findMidiPitchFromFrequency(n.frequency);
      e.volume = 0x64;

      //dump
      bool isRunningStatus = (i!=0);
      t.length += fwriteevent(e, isRunningStatus, fout);
    }

    //remember previous note duration
    previousNoteTicks = computeTicks(n.durationMs);
  }

  //silence all notes
  {
    NOTE_EVENT e;
    e.ticks = previousNoteTicks;
    e.status = CONTROLCHANGE_B_CHANNEL_0;
    e.pitch = ALL_NOTES_OFF;
    e.volume = MIN_VOLUME;

    //dump
    t.length += fwriteevent(e, false, fout);
  }

  //add track footer
  fswapwrite(MIDI_TRACK_FOOTER_ID, fout);

  //update track length
  t.length += sizeof(MIDI_TRACK_FOOTER_ID);

  //write FILE & TRACK headers again
  fseek(fout, 0, SEEK_SET);
  fswapwrite(h, fout);
  fswapwrite(t, fout);


  fclose(fout);
  return true;
}
