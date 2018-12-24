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

//
// Description:
//   Library for handling notes.
//

#include "libmidi/notes.h"
#include "libmidi/pitches.h"

#include <cstdlib>  //for abs()
#include <string>   //for NULL

struct NOTEDEF
{
  int freq;
  const char * name;
};
static NOTEDEF gNotesDefinition[] = {
  { NOTE_SILENCE , "SILENCE" },
  { NOTE_C0      , "C0" },
  { NOTE_CS0     , "CS0" },
  { NOTE_D0      , "D0" },
  { NOTE_DS0     , "DS0" },
  { NOTE_E0      , "E0" },
  { NOTE_F0      , "F0" },
  { NOTE_FS0     , "FS0" },
  { NOTE_G0      , "G0" },
  { NOTE_GS0     , "GS0" },
  { NOTE_A0      , "A0" },
  { NOTE_AS0     , "AS0" },
  { NOTE_B0      , "B0" },
  { NOTE_C1      , "C1" },
  { NOTE_CS1     , "CS1" },
  { NOTE_D1      , "D1" },
  { NOTE_DS1     , "DS1" },
  { NOTE_E1      , "E1" },
  { NOTE_F1      , "F1" },
  { NOTE_FS1     , "FS1" },
  { NOTE_G1      , "G1" },
  { NOTE_GS1     , "GS1" },
  { NOTE_A1      , "A1" },
  { NOTE_AS1     , "AS1" },
  { NOTE_B1      , "B1" },
  { NOTE_C2      , "C2" },
  { NOTE_CS2     , "CS2" },
  { NOTE_D2      , "D2" },
  { NOTE_DS2     , "DS2" },
  { NOTE_E2      , "E2" },
  { NOTE_F2      , "F2" },
  { NOTE_FS2     , "FS2" },
  { NOTE_G2      , "G2" },
  { NOTE_GS2     , "GS2" },
  { NOTE_A2      , "A2" },
  { NOTE_AS2     , "AS2" },
  { NOTE_B2      , "B2" },
  { NOTE_C3      , "C3" },
  { NOTE_CS3     , "CS3" },
  { NOTE_D3      , "D3" },
  { NOTE_DS3     , "DS3" },
  { NOTE_E3      , "E3" },
  { NOTE_F3      , "F3" },
  { NOTE_FS3     , "FS3" },
  { NOTE_G3      , "G3" },
  { NOTE_GS3     , "GS3" },
  { NOTE_A3      , "A3" },
  { NOTE_AS3     , "AS3" },
  { NOTE_B3      , "B3" },
  { NOTE_C4      , "C4" },
  { NOTE_CS4     , "CS4" },
  { NOTE_D4      , "D4" },
  { NOTE_DS4     , "DS4" },
  { NOTE_E4      , "E4" },
  { NOTE_F4      , "F4" },
  { NOTE_FS4     , "FS4" },
  { NOTE_G4      , "G4" },
  { NOTE_GS4     , "GS4" },
  { NOTE_A4      , "A4" },
  { NOTE_AS4     , "AS4" },
  { NOTE_B4      , "B4" },
  { NOTE_C5      , "C5" },
  { NOTE_CS5     , "CS5" },
  { NOTE_D5      , "D5" },
  { NOTE_DS5     , "DS5" },
  { NOTE_E5      , "E5" },
  { NOTE_F5      , "F5" },
  { NOTE_FS5     , "FS5" },
  { NOTE_G5      , "G5" },
  { NOTE_GS5     , "GS5" },
  { NOTE_A5      , "A5" },
  { NOTE_AS5     , "AS5" },
  { NOTE_B5      , "B5" },
  { NOTE_C6      , "C6" },
  { NOTE_CS6     , "CS6" },
  { NOTE_D6      , "D6" },
  { NOTE_DS6     , "DS6" },
  { NOTE_E6      , "E6" },
  { NOTE_F6      , "F6" },
  { NOTE_FS6     , "FS6" },
  { NOTE_G6      , "G6" },
  { NOTE_GS6     , "GS6" },
  { NOTE_A6      , "A6" },
  { NOTE_AS6     , "AS6" },
  { NOTE_B6      , "B6" },
  { NOTE_C7      , "C7" },
  { NOTE_CS7     , "CS7" },
  { NOTE_D7      , "D7" },
  { NOTE_DS7     , "DS7" },
  { NOTE_E7      , "E7" },
  { NOTE_F7      , "F7" },
  { NOTE_FS7     , "FS7" },
  { NOTE_G7      , "G7" },
  { NOTE_GS7     , "GS7" },
  { NOTE_A7      , "A7" },
  { NOTE_AS7     , "AS7" },
  { NOTE_B7      , "B7" },
  { NOTE_C8      , "C8" },
  { NOTE_CS8     , "CS8" },
  { NOTE_D8      , "D8" },
  { NOTE_DS8     , "DS8" },
  { NOTE_CS8     , "CS8" },
  { NOTE_D8      , "D8" },
  { NOTE_DS8     , "DS8" },
  { NOTE_E8      , "E8" },
  { NOTE_F8      , "F8" },
  { NOTE_FS8     , "FS8" },
  { NOTE_G8      , "G8" },
  { NOTE_GS8     , "GS8" },
  { NOTE_A8      , "A8" },
  { NOTE_AS8     , "AS8" },
  { NOTE_B8      , "B8" },
  { NOTE_C9      , "C9" },
  { NOTE_CS9     , "CS9" },
  { NOTE_D9      , "D9" },
  { NOTE_DS9     , "DS9" },
  { NOTE_E9      , "E9" },
  { NOTE_F9      , "F9" },
  { NOTE_FS9     , "FS9" },
  { NOTE_G9      , "G9" },
  { NOTE_GS9     , "GS9" },
  //{ NOTE_A9      , "A9" },
  //{ NOTE_AS9     , "AS9" },
  //{ NOTE_B9      , "B9" },

  { NOTE_AS0, "AS0" },
  //{ NOTE_BS0, "BS0" },
  { NOTE_CS0, "CS0" },
  { NOTE_DS0, "DS0" },
  //{ NOTE_ES0, "ES0" },
  { NOTE_FS0, "FS0" },
  { NOTE_GS0, "GS0" },
  { NOTE_AS1, "AS1" },
  //{ NOTE_BS1, "BS1" },
  { NOTE_CS1, "CS1" },
  { NOTE_DS1, "DS1" },
  //{ NOTE_ES1, "ES1" },
  { NOTE_FS1, "FS1" },
  { NOTE_GS1, "GS1" },
  { NOTE_AS2, "AS2" },
  //{ NOTE_BS2, "BS2" },
  { NOTE_CS2, "CS2" },
  { NOTE_DS2, "DS2" },
  //{ NOTE_ES2, "ES2" },
  { NOTE_FS2, "FS2" },
  { NOTE_GS2, "GS2" },
  { NOTE_AS3, "AS3" },
  //{ NOTE_BS3, "BS3" },
  { NOTE_CS3, "CS3" },
  { NOTE_DS3, "DS3" },
  //{ NOTE_ES3, "ES3" },
  { NOTE_FS3, "FS3" },
  { NOTE_GS3, "GS3" },
  { NOTE_AS4, "AS4" },
  //{ NOTE_BS4, "BS4" },
  { NOTE_CS4, "CS4" },
  { NOTE_DS4, "DS4" },
  //{ NOTE_ES4, "ES4" },
  { NOTE_FS4, "FS4" },
  { NOTE_GS4, "GS4" },
  { NOTE_AS5, "AS5" },
  //{ NOTE_BS5, "BS5" },
  { NOTE_CS5, "CS5" },
  { NOTE_DS5, "DS5" },
  //{ NOTE_ES5, "ES5" },
  { NOTE_FS5, "FS5" },
  { NOTE_GS5, "GS5" },
  { NOTE_AS6, "AS6" },
  //{ NOTE_BS6, "BS6" },
  { NOTE_CS6, "CS6" },
  { NOTE_DS6, "DS6" },
  //{ NOTE_ES6, "ES6" },
  { NOTE_FS6, "FS6" },
  { NOTE_GS6, "GS6" },
  { NOTE_AS7, "AS7" },
  //{ NOTE_BS7, "BS7" },
  { NOTE_CS7, "CS7" },
  { NOTE_DS7, "DS7" },
  //{ NOTE_ES7, "ES7" },
  { NOTE_FS7, "FS7" },
  { NOTE_GS7, "GS7" },
  { NOTE_AS8, "AS8" },
  //{ NOTE_BS8, "BS8" },
  { NOTE_CS8, "CS8" },
  { NOTE_DS8, "DS8" },
  //{ NOTE_ES8, "ES8" },
  { NOTE_FS8, "FS8" },
  { NOTE_GS8, "GS8" },
  //{ NOTE_AS9, "AS9" },
  //{ NOTE_BS9, "BS9" },
  { NOTE_CS9, "CS9" },
  { NOTE_DS9, "DS9" },
  //{ NOTE_ES9, "ES9" },
  { NOTE_FS9, "FS9" },
  { NOTE_GS9, "GS9" },
};
static const size_t gNotesDefinitionCount = sizeof(gNotesDefinition)/sizeof(gNotesDefinition[0]);

int findNoteFrequency(const char * iName)
{
  if (iName == NULL)
    return -1;
  std::string name = iName;
  for(size_t i=0; i<gNotesDefinitionCount; i++)
  {
    const NOTEDEF & note = gNotesDefinition[i];
    if (name == note.name)
      return note.freq;
  }
  return -1;
}

const char * getNoteName(int iFreq)
{
  if (iFreq < 0)
    return NULL;
  for(size_t i=0; i<gNotesDefinitionCount; i++)
  {
    const NOTEDEF & note = gNotesDefinition[i];
    if (iFreq == note.freq)
      return note.name;
  }
  return NULL;
}

const char * getNoteName(int iFreq, int iEpsilon)
{
  const char * best_name = NULL;
  int best_frequency = -1;
  int best_diff = -1;

  if (iFreq < 0)
    return NULL;
  for(size_t i=0; i<gNotesDefinitionCount; i++)
  {
    const NOTEDEF & note = gNotesDefinition[i];
    int diff = abs(note.freq - iFreq);
    if (diff <= iEpsilon)
    {
      //that is a good candidate

      if (best_name == NULL || best_diff > diff)
      {
        //that is a better candidate that previous 'best'.
        best_name = note.name;
        best_diff = diff;
        best_frequency = note.freq;
      }
    }

    //if a perfect match
    if (iFreq == note.freq)
      return note.name;
  }
  return best_name;
}
