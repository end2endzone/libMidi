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

#ifndef VARIABLE_LENGTH_H
#define VARIABLE_LENGTH_H

/// <summary>
/// Computes a 32-bits mask for reading the nth block of 7 bits long.
/// </summary>
/// <param name="iIndex">
/// The 0-based index of the 7-bits blocks. ie:
/// index 0 creates a mask for bit  0 to bit  6
/// index 1 creates a mask for bit  7 to bit 13
/// index 2 creates a mask for bit 14 to bit 20
/// </param>
/// <returns>Returns a 32-bits mask.</returns>
inline uint32_t getVariableLengthMask(int iIndex)
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

/// <summary>
/// Writes a value as a Variable Length Quantity to a file.
/// </summary>
/// <remarks>
/// Variable Length Quantity numbers are represented as 7 bits per byte,
/// most significant bits first. All bytes except the last have bit 7
/// set, and the last byte has bit 7 clear. If the number is between 0
/// and 127, it is thus represented exactly as one byte.
/// For example:
///
///     Values    Variable Length Quantity
///     00000000  00
///     00000040  40
///     0000007F  7F
///     00000080  81 00
///     00002000  C0 00
///     00003FFF  FF 7F
///     00004000  81 80 00
///     00100000  C0 80 00
///     001FFFFF  FF FF 7F
///     00200000  81 80 80 00
///     08000000  C0 80 80 00
///     0FFFFFFF  FF FF FF 7F
/// </remarks>
/// <param name="iValue">The value to be written to file</param>
/// <param name="iMinOutputSize">The minimum output size in byte.
/// ie: Writing the value 0x00000000 is normaly written as 1 byte 0x00.
/// Forcing minimum byte to 2 result in the following bytes written:
/// 0x80 0x00 which is the same value.</param>
/// <param name="f">The FILE* handle</param>
/// <returns>Returns the number of bytes written to the file</returns>
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

/// <summary>
/// Writes a value as a Variable Length Quantity to a file.
/// </summary>
/// <remarks>
/// Variable Length Quantity numbers are represented as 7 bits per byte,
/// most significant bits first. All bytes except the last have bit 7
/// set, and the last byte has bit 7 clear. If the number is between 0
/// and 127, it is thus represented exactly as one byte.
/// For example:
///
///     Values    Variable Length Quantity
///     00000000  00
///     00000040  40
///     0000007F  7F
///     00000080  81 00
///     00002000  C0 00
///     00003FFF  FF 7F
///     00004000  81 80 00
///     00100000  C0 80 00
///     001FFFFF  FF FF 7F
///     00200000  81 80 80 00
///     08000000  C0 80 80 00
///     0FFFFFFF  FF FF FF 7F
/// </remarks>
/// <param name="iValue">The value to be written to file</param>
/// <param name="f">The FILE* handle</param>
/// <returns>Returns the number of bytes written to the file</returns>
template <typename T>
size_t fwriteVariableLength(const T & iValue, FILE * f)
{
  return fwriteVariableLength(iValue, 0, f);
}

#endif //VARIABLE_LENGTH_H