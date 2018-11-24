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

// TestMidiFile.cpp : Defines the entry point for the console application.
//

#include "libMidi/libMidi.h"
#include "varlength.h"

#include "rapidassist/gtesthelp.h"
#include "TestMidiFile.h"

typedef std::vector<unsigned char> CharSequence;

uint32_t readFileContentAs32bits(const char * iFilePath)
{
  uint32_t value = 0;
  unsigned char * buffer = (unsigned char *)&value;

  FILE * f = fopen(iFilePath, "rb");
  if (!f)
  {
    return 0;
  }

  fread(&buffer[0], 1, 1, f);
  fread(&buffer[1], 1, 1, f);
  fread(&buffer[2], 1, 1, f);
  fread(&buffer[3], 1, 1, f);

  fclose(f);

  return value;
}

CharSequence readFileContentAsArray(const char * iFilePath)
{
  CharSequence bytes;

  FILE * f = fopen(iFilePath, "rb");
  if (!f)
    return bytes;

  bool byteread = true;
  while(byteread) // !feof(f)
  {
    unsigned char c = 0;
    byteread = fread(&c, 1, 1, f) > 0;
    if (byteread)
    {
      bytes.push_back(c);
    }
  }

  fclose(f);

  return bytes;
}


void TestMidiFile::SetUp()
{
}

void TestMidiFile::TearDown()
{
}

TEST_F(TestMidiFile, testCDE)
{
  static const char * outputFile = "testCDE.output.mid";

  MidiFile f;
  f.setMidiType(MidiFile::MIDI_TYPE_1);
  f.setTicksPerQuarterNote(0x80);
  f.setVolume(0x60);
  f.setTrackEndingPreference(MidiFile::STOP_ALL_NOTES);
  f.addNote(262, 500); //C4
  f.addNote(294, 500); //D4
  f.addNote(330, 500); //E4
  bool saved = f.save(outputFile);
  ASSERT_TRUE(saved);

  //ASSERTION DISABLED. CDE.MID mixes C4, D4 and E4 notes
  //at the same time which is not supported by the library
#if 0
  //ASSERT content is **almost** identical
  CharSequence actualFileContent   = readFileContentAsArray(outputFile);
  CharSequence expectedFileContent = readFileContentAsArray("test_files\\cde1.mid");
  
  //expecting file are equals expect that for event for base file uses 0x8000 instead of 00 for timestamp (which is the same)
  actualFileContent.insert(actualFileContent.begin()+22, 0x80);
  actualFileContent[21]++; //track size

  ASSERT_EQ(expectedFileContent.size(), actualFileContent.size());
  for(size_t i=0; i<expectedFileContent.size(); i++)
  {
    unsigned char expectedByte = expectedFileContent[i];
    unsigned char actualByte = actualFileContent[i];
    ASSERT_EQ(expectedByte, actualByte);
  }
#endif
}

TEST_F(TestMidiFile, testMario1Up)
{
  static const char * outputFile = "testMario1Up.output.mid";

  MidiFile f;

  f.setInstrument(0x51);
  f.setMidiType(MidiFile::MIDI_TYPE_0);
  f.setTempo(0x051615);
  f.setName("mario1up");
  f.setVolume(0x64);

#if 0
  //mario 1-up.
  //accurate frequencies
  f.addNote(1319,125);
  f.addNote(1568,125);
  f.addNote(2637,125);
  f.addNote(2093,125);
  f.addNote(2349,125);
  f.addNote(3136,125);
#else
  //mario 1-up.
  //frequencies matching mario1up.mid
  f.addNote(659 , 125);
  f.addNote(784 , 125);
  f.addNote(1319, 125);
  f.addNote(1047, 125);
  f.addNote(1175, 125);
  f.addNote(1568, 125);
#endif

  bool saved = f.save(outputFile);
  ASSERT_TRUE(saved);

  //ASSERT content is identical
  ASSERT_TRUE( ra::gtesthelp::isFileEquals(outputFile, "test_files\\mario1up.mid") );
}

TEST_F(TestMidiFile, test1Second)
{
  static const char * outputFile = "test1Second.output.mid";

  MidiFile f;
  
  f.setInstrument(0x51);

  //BPM setting.
  //The RTTTL is encoded as 90 BPM but the conversion to
  //tempo is 1usec too low for binary match the expected file
  //using direct TEMPO setting instead.
  //f.setBeatsPerMinute(90);
  f.setTempo(0x0a2c2b);

  f.setName("1second");
  f.setVolume(0x64);

  f.addNote(131, 1001); // C3 instead of C4 which is 262

  bool saved = f.save(outputFile);
  ASSERT_TRUE(saved);

  //ASSERT content is identical
  ASSERT_TRUE( ra::gtesthelp::isFileEquals(outputFile, "test_files\\1second.mid") );
}

TEST_F(TestMidiFile, testBuzzer)
{
  static const char * outputFile = "testBuzzer.output.mid";

  MidiFile f;
  f.setInstrument(0x51);
  f.setMidiType(MidiFile::MIDI_TYPE_0);
  f.setTempo(0x051615);
  f.setName("buzzer");
  f.setVolume(0x64);
  for(int i=0; i<10; i++)
  {
    f.addNote(131, 125); // C3 instead of C4 which is 262
    f.addDelay(125);
  }
  bool saved = f.save(outputFile);
  ASSERT_TRUE(saved);

  //ASSERT content is identical
  ASSERT_TRUE( ra::gtesthelp::isFileEquals(outputFile, "test_files\\buzzer.mid") );
}

TEST_F(TestMidiFile, testBuzzerWrongFrequencies)
{
  static const char * outputFile = "testBuzzer.output.mid";

  //test with 2Hz higher than normal note
  {
    MidiFile f;
    f.setInstrument(0x51);
    f.setMidiType(MidiFile::MIDI_TYPE_0);
    f.setTempo(0x051615);
    f.setName("buzzer");
    f.setVolume(0x64);
    for(int i=0; i<10; i++)
    {
      f.addNote(133 /*instead of 131*/, 125);
      f.addDelay(125);
    }
    bool saved = f.save(outputFile);
    ASSERT_TRUE(saved);

    //ASSERT content is identical
    ASSERT_TRUE( ra::gtesthelp::isFileEquals(outputFile, "test_files\\buzzer.mid") );
  }

  //test with 2Hz lower than normal note
  {
    MidiFile f;
    f.setInstrument(0x51);
    f.setMidiType(MidiFile::MIDI_TYPE_0);
    f.setTempo(0x051615);
    f.setName("buzzer");
    f.setVolume(0x64);
    for(int i=0; i<10; i++)
    {
      f.addNote(129 /*instead of 131*/, 125);
      f.addDelay(125);
    }
    bool saved = f.save(outputFile);
    ASSERT_TRUE(saved);

    //ASSERT content is identical
    ASSERT_TRUE( ra::gtesthelp::isFileEquals(outputFile, "test_files\\buzzer.mid") );
  }
}

TEST_F(TestMidiFile, testVolume)
{
  static const char * outputFile = "testVolume.output.mid";

  MidiFile f;
  f.setMidiType(MidiFile::MIDI_TYPE_0);
  f.setTempo(0x051615);
  f.setName("volume");

  //for ups and downs
  for(int j=0; j<3; j++)
  {
    static const int numSteps = 20;
    static const int upDownDurationMs = 1000;
    static const int volumeStep = 0x7f/numSteps;
    static const int noteDuration = upDownDurationMs/numSteps;

    int volume = 0;
    for(int i=0; i<numSteps; i++)
    {
      f.setVolume(volume);
      f.addNote(131, noteDuration); // C3 instead of C4 which is 262
      volume += volumeStep;
    }
    for(int i=0; i<numSteps; i++)
    {
      f.setVolume(volume);
      f.addNote(131, noteDuration); // C3 instead of C4 which is 262
      volume -= volumeStep;
    }
  }
  bool saved = f.save(outputFile);
  ASSERT_TRUE(saved);
}

TEST_F(TestMidiFile, testFindInstrument)
{
  ASSERT_EQ(0x00, MidiFile::findInstrument("") );
  ASSERT_EQ(0x00, MidiFile::findInstrument(NULL) );
  ASSERT_EQ(0x00, MidiFile::findInstrument("Acoustic Grand Piano") );
  ASSERT_EQ(0x01, MidiFile::findInstrument("Bright Acoustic Piano") );
  ASSERT_EQ(0x7f, MidiFile::findInstrument("Gunshot") );
}

TEST_F(TestMidiFile, testGetInstrumentName)
{
  ASSERT_TRUE( std::string("Acoustic Grand Piano") == MidiFile::getInstrumentName(0x00) );
  ASSERT_TRUE( std::string("Acoustic Grand Piano") == MidiFile::getInstrumentName(-1) );
  ASSERT_TRUE( std::string("Bright Acoustic Piano") == MidiFile::getInstrumentName(0x01) );
  ASSERT_TRUE( std::string("Gunshot") == MidiFile::getInstrumentName(0x7f) );
  ASSERT_TRUE( std::string("Acoustic Grand Piano") == MidiFile::getInstrumentName((char)0xff) );
}

TEST_F(TestMidiFile, testAllInstruments)
{
  MidiFile f;
  f.setMidiType(MidiFile::MIDI_TYPE_0);
  f.setTempo(0x051615);
  f.setName("allbuzzer");
  f.setVolume(0x7f);
  for(int i=0; i<10; i++)
  {
    f.addNote(131, 125); // C3 instead of C4 which is 262
    f.addDelay(125);
  }

  for(int8_t i=0; i>=0 && i<=0x7f; i++)
  {
    char outputFile[80];
    sprintf(outputFile, "testAllInstruments.%03d (0x%02x).output.mid", ((int)i)+1, i);

    f.setInstrument(i);
    bool saved = f.save(outputFile);
    ASSERT_TRUE(saved);
  }
}

TEST_F(TestMidiFile, testVariableLength)
{
  //0
  {
    static const char * filename = "0.output.bin";
    FILE * f = fopen(filename, "wb");
    unsigned int value = 0;
    fwriteVariableLength(value, f);
    fclose(f);
    CharSequence sequence = readFileContentAsArray(filename);
    ASSERT_EQ(1, sequence.size());
    ASSERT_EQ(0x00, sequence[0]);
  }

  //127
  {
    static const char * filename = "127.output.bin";
    FILE * f = fopen(filename, "wb");
    unsigned int value = 127; // 0x7F
    fwriteVariableLength(value, f);
    fclose(f);
    CharSequence sequence = readFileContentAsArray(filename);
    ASSERT_EQ(1, sequence.size());
    ASSERT_EQ(0x7f, sequence[0]);
  }

  //255
  {
    static const char * filename = "255.output.bin";
    FILE * f = fopen(filename, "wb");
    unsigned int value = 255; // 0xFF
    fwriteVariableLength(value, f);
    fclose(f);
    CharSequence sequence = readFileContentAsArray(filename);
    ASSERT_EQ(2, sequence.size());
    ASSERT_EQ(0x81, sequence[0]);
    ASSERT_EQ(0x7f, sequence[1]);
  }

  //32768
  {
    static const char * filename = "32768.output.bin";
    FILE * f = fopen(filename, "wb");
    unsigned int value = 32768; // 0x8000
    fwriteVariableLength(value, f);
    fclose(f);
    CharSequence sequence = readFileContentAsArray(filename);
    ASSERT_EQ(3, sequence.size());
    ASSERT_EQ(0x82, sequence[0]);
    ASSERT_EQ(0x80, sequence[1]);
    ASSERT_EQ(0x00, sequence[2]);
  }
}
