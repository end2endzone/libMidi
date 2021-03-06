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
//   Library for handling midi instruments.
//

#include "libmidi/instruments.h"

#include <string>   //for NULL

namespace libmidi
{

INSTRUMENT findInstrument(const char * iName)
{
  if (iName == NULL)
    return INVALID_INSTRUMENT;
  std::string name = iName;
  for(INSTRUMENT i=MIN_INSTRUMENT; i>=MIN_INSTRUMENT && i<=MAX_INSTRUMENT; i++)
  {
    const char * currentName = gInstruments[i];
    if (name == currentName)
      return i;
  }
  return INVALID_INSTRUMENT;
}

const char * getInstrumentName(INSTRUMENT iInstrument)
{
  if (iInstrument < MIN_INSTRUMENT || iInstrument > MAX_INSTRUMENT)
    return NULL;
  return gInstruments[iInstrument];
}

}; //namespace libmidi
