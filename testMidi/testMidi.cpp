// testMidi.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>
#include "libMidi.h"

int _tmain(int argc, _TCHAR* argv[])
{
  MidiFile f;
  f.addNote(262, 100); //C4
  f.addNote(294, 100); //D4
  f.addNote(330, 100); //E4
  f.save("output.mid");
  return 0;
}

