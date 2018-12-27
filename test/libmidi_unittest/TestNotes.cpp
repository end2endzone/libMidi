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

#include "libmidi/notes.h"
#include "libmidi/pitches.h"

#include "TestNotes.h"

using namespace libmidi;

void TestNotes::SetUp()
{
}

void TestNotes::TearDown()
{
}

TEST_F(TestNotes, testFindNoteFrequency)
{
  ASSERT_EQ(-1, findNoteFrequency(NULL) );  //invalid
  ASSERT_EQ(-1, findNoteFrequency("") );  //empty
  ASSERT_EQ(-1, findNoteFrequency("FOOBAR") ); //unknown

  ASSERT_EQ(NOTE_SILENCE, findNoteFrequency("SILENCE") );  //silence

  ASSERT_EQ(NOTE_C0 , findNoteFrequency("C0" ) );
  ASSERT_EQ(NOTE_GS9, findNoteFrequency("GS9") );
  ASSERT_EQ(NOTE_AS0, findNoteFrequency("AS0") );
  ASSERT_EQ(NOTE_GS9, findNoteFrequency("GS9") );
}

TEST_F(TestNotes, testGetNoteName)
{
  ASSERT_EQ(NULL, getNoteName(-5) );  //invalid
  ASSERT_EQ(NULL, getNoteName(1000) ); //unknown

  ASSERT_STREQ("SILENCE", getNoteName(0) );  //silence

  ASSERT_STREQ("C0",  getNoteName(NOTE_C0) );
  ASSERT_STREQ("GS9", getNoteName(NOTE_GS9) );
  ASSERT_STREQ("AS0", getNoteName(NOTE_AS0) );
  ASSERT_STREQ("GS9", getNoteName(NOTE_GS9) );
}

TEST_F(TestNotes, testGetNoteNameEpsilon)
{
  static const int HUGE_EPSILON = 10000000;
  static const int SMALL_EPSILON = 1;
  ASSERT_EQ(NULL, getNoteName(-5, HUGE_EPSILON) );  //invalid

  ASSERT_STREQ("SILENCE", getNoteName(0, HUGE_EPSILON) );  //silence

  ASSERT_STREQ("C6", getNoteName(NOTE_C6,       HUGE_EPSILON) );  //exact note frequency
  ASSERT_STREQ("C6", getNoteName(NOTE_C6 + 1,   SMALL_EPSILON) ); //frequency too high
  ASSERT_STREQ("C6", getNoteName(NOTE_C6 - 1,   SMALL_EPSILON) ); //frequency too low
  ASSERT_STREQ(NULL, getNoteName(NOTE_C6 + 10,  SMALL_EPSILON) ); //outside of epsilon
  ASSERT_STREQ("C6", getNoteName(NOTE_C6 - 5,   100) ); //epsilon matches NOTE_B5, NOTE_C6 and NOTE_CS6. NOTE_C6 is the closest.
  ASSERT_STREQ("C6", getNoteName(NOTE_C6 + 5,   100) ); //epsilon matches NOTE_B5, NOTE_C6 and NOTE_CS6. NOTE_C6 is the closest.
}

