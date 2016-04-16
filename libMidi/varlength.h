//
//  libMidi Library - v1.0 - 04/15/2016
//  Copyright (C) 2016 Antoine Beauchamp
//  The code & updates for the library can be found on http://end2endzone.com
//
// AUTHOR/LICENSE:
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3.0 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License (LGPL-3.0) for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// DISCLAIMER:
//  This software is furnished "as is", without technical support, and with no 
//  warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
//  This libMidi library allows one to create basic midi file based on
//  tone frequency & duration.
//
// HISTORY:
// 04/15/2016 v1.0 - Initial release.
//


#pragma once

inline size_t getVariableLengthMask(int iIndex)
{
  //size_t i = 1;
  //i << ((index+1)*7);
  //i--;

  static const uint32_t MASK_TIMESTAMP_0 =  ((1<<7)-1);
  static const uint32_t MASK_TIMESTAMP_1 =  ((1<<14)-1) - MASK_TIMESTAMP_0;
  static const uint32_t MASK_TIMESTAMP_2 =  ((1<<21)-1) - MASK_TIMESTAMP_1;
  static const uint32_t MASK_TIMESTAMP_3 =  ((1<<28)-1) - MASK_TIMESTAMP_2;
  static const uint32_t masks[] = {
    MASK_TIMESTAMP_0,
    MASK_TIMESTAMP_1,
    MASK_TIMESTAMP_2,
    MASK_TIMESTAMP_3
  };
  return masks[iIndex];
}

template <typename T>
size_t fwriteVariableLength(const T & iValue, size_t iMinOutputSize, FILE * f)
{
  size_t writeSize = 0;

  //copy the input value since we will decrement it
  T copy = iValue;

  size_t inputSize = sizeof(T);

  //define output buffer
  size_t maxOutputSize = inputSize+1;
  unsigned char * outputBuffer = new unsigned char[maxOutputSize];
  memset(outputBuffer, 0, maxOutputSize);
  size_t outputSize = 1; //number of characters to output

  unsigned char * inputBuffer = (unsigned char *)&copy;
  for(size_t i=0; i<inputSize; i++)
  {
    //move the next 7 bits from inputBuffer to outputBuffer
    uint32_t mask = getVariableLengthMask(i);
    outputBuffer[i] |= ((copy & mask) >> (i*7));
    copy &= ~mask;

    //is there some bits left in the input ?
    if (copy > 0)
    {
      //yes, there will be another output character
      outputSize++;
    }
    else
    {
      //no, no need to read furthur
      break;
    }
  }

  //deal with forced output size
  if (outputSize < iMinOutputSize && iMinOutputSize <= maxOutputSize)
  {
    outputSize = iMinOutputSize;
  }

  //write all output characters
  for(int i=(int)outputSize-1; i>=0; i--)
  {
    bool isLast = (i == 0);
    unsigned char c = outputBuffer[i];
    if (!isLast)
    {
      c |= 0x80;
    }
    writeSize += fwrite(&c, 1, 1, f);
  }

  delete[] outputBuffer;

  return writeSize;
}

template <typename T>
size_t fwriteVariableLength(const T & iValue, FILE * f)
{
  return fwriteVariableLength(iValue, 0, f);
}