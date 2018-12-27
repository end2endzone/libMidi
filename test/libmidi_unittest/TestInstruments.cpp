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

#include "libmidi/instruments.h"

#include "TestInstruments.h"

using namespace libmidi;

void TestInstruments::SetUp()
{
}

void TestInstruments::TearDown()
{
}

TEST_F(TestInstruments, testFindInstrument)
{
  ASSERT_EQ(0x00, findInstrument("") );
  ASSERT_EQ(0x00, findInstrument(NULL) );
  ASSERT_EQ(0x00, findInstrument("Acoustic Grand Piano") );
  ASSERT_EQ(0x01, findInstrument("Bright Acoustic Piano") );
  ASSERT_EQ(0x7f, findInstrument("Gunshot") );
}

TEST_F(TestInstruments, testGetInstrumentName)
{
  ASSERT_TRUE( std::string("Acoustic Grand Piano") == getInstrumentName(0x00) );
  ASSERT_TRUE( std::string("Acoustic Grand Piano") == getInstrumentName(-1) );
  ASSERT_TRUE( std::string("Bright Acoustic Piano") == getInstrumentName(0x01) );
  ASSERT_TRUE( std::string("Gunshot") == getInstrumentName(0x7f) );
  ASSERT_TRUE( std::string("Acoustic Grand Piano") == getInstrumentName((char)0xff) );
}
