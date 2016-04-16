// testMidi.cpp : Defines the entry point for the console application.
//

//#include <stdio.h>
//#include <tchar.h>
#include "libMidi.h"
#include "miditypes.h"
#include "varlength.h"

#include "gtesthelper.h"
#include <vector>

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
  gTestHelper & helper = gTestHelper::getInstance();
}

void TestMidiFile::TearDown()
{
}

TEST_F(TestMidiFile, testCDE)
{
  static const char * outputFile = "testCDE.output.mid";

  MidiFile f;
  f.addNote(262, 100); //C4
  f.addNote(294, 100); //D4
  f.addNote(330, 100); //E4
  bool saved = f.save(outputFile);
  ASSERT_TRUE(saved);
}

TEST_F(TestMidiFile, testMario1Up)
{
  static const char * outputFile = "testMario1Up.output.mid";

  MidiFile f;
  f.setName("mario1up");
  f.addNote(1319,125);
  f.addNote(1568,125);
  f.addNote(2637,125);
  f.addNote(2093,125);
  f.addNote(2349,125);
  f.addNote(3136,125);
  bool saved = f.save(outputFile);
  ASSERT_TRUE(saved);
}

TEST_F(TestMidiFile, test1Second)
{
  static const char * outputFile = "test1Second.output.mid";

  MidiFile f;
  f.setBeatsPerMinute(90);
  f.setName("1second");
  f.addNote(131, 1000); // C3 instead of C4 which is 262
  bool saved = f.save(outputFile);
  ASSERT_TRUE(saved);
}

TEST_F(TestMidiFile, testBuzzer)
{
  static const char * outputFile = "testBuzzer.output.mid";
  gTestHelper & helper = gTestHelper::getInstance();

  MidiFile f;
  f.setMidiType(MidiFile::MIDI_TYPE_0);
  f.setTempo(0x051615);
  f.setName("buzzer");
  f.setVolume(0x64);
  MidiFile::TRACK_ENDING_PREFERENCE preferences = (MidiFile::TRACK_ENDING_PREFERENCE)(MidiFile::STOP_PREVIOUS_NOTE | MidiFile::TRACK_FOOTER_TICKS);
  f.setTrackEndingPreference(preferences);
  for(int i=0; i<10; i++)
  {
    f.addNote(131, 125); // C3 instead of C4 which is 262
    f.addDelay(125);
  }
  bool saved = f.save(outputFile);
  ASSERT_TRUE(saved);

  ASSERT_TRUE( helper.isFileEquals(outputFile, "testFiles\\buzzer.mid") );
}

TEST_F(TestMidiFile, testVolume)
{
  static const char * outputFile = "testVolume.output.mid";

  MidiFile f;
  f.setMidiType(MidiFile::MIDI_TYPE_0);
  f.setTempo(0x051615);
  f.setName("volume");
  MidiFile::TRACK_ENDING_PREFERENCE preferences = (MidiFile::TRACK_ENDING_PREFERENCE)(MidiFile::STOP_PREVIOUS_NOTE | MidiFile::TRACK_FOOTER_TICKS);
  f.setTrackEndingPreference(preferences);

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

TEST_F(TestMidiFile, testVariableLength)
{
  gTestHelper & helper = gTestHelper::getInstance();

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
