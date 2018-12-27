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

#ifndef NOTES_H
#define NOTES_H

namespace libmidi
{

/// <summary>Finds a note frequency by name.</summary>
/// <param name="iName">The given name of the note.</param>
/// <returns>An identifier that matches the given note name.
/// Returns -1 if the name is unknown, NULL, or empty.
/// </returns>
int findNoteFrequency(const char * iName);

/// <summary>Get the note name that matches the given frequency.</summary>
/// <param name="iFreq">The frequency in Hz to search for. The frequency must match an exact note.</param>
/// <returns>Returns the note name matching the frequency. Returns NULL on invalid frequency.</returns>
const char * getNoteName(int iFreq);

/// <summary>Get the note name that 'best' matches the given frequency.</summary>
/// <param name="iFreq">The frequency in Hz to search for.</param>
/// <param name="iEpsilon">The epsilon value for tolerance.</param>
/// <returns>Returns the note name matching the frequency. Returns NULL when no note matches the given frequency.</returns>
const char * getNoteName(int iFreq, int iEpsilon);

}; //namespace libmidi

#endif //NOTES_H