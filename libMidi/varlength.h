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
size_t fwriteVariableLength(const T & iValue, int iMinOutputSize, FILE * f)
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