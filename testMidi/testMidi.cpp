// testMidi.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>
#include "libMidi.h"
#include "miditypes.h"
#include "varlength.h"

void testCDE()
{
  MidiFile f;
  f.addNote(262, 100); //C4
  f.addNote(294, 100); //D4
  f.addNote(330, 100); //E4
  f.save("testCDE.output.mid");
}

void testVariableLength()
{
  //0
  {
    FILE * f = fopen("0.output.bin", "wb");
    unsigned int value = 0;
    fwriteVariableLength(value, f);
    fclose(f);
  }

  //127
  {
    FILE * f = fopen("127.output.bin", "wb");
    unsigned int value = 127; // 0x7F
    fwriteVariableLength(value, f);
    fclose(f);
  }

  //255
  {
    FILE * f = fopen("255.output.bin", "wb");
    unsigned int value = 255; // 0xFF
    fwriteVariableLength(value, f);
    fclose(f);
  }

  //32768
  {
    FILE * f = fopen("32768.output.bin", "wb");
    unsigned int value = 32768; // 0x8000
    fwriteVariableLength(value, f);
    fclose(f);
  }
}

int _tmain(int argc, _TCHAR* argv[])
{
  testVariableLength();
  testCDE();

  return 0;
}
