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

#include <stdio.h>
#include <iostream>

#include <gtest/gtest.h>

#include "rapidassist/gtesthelp.h"
#include "rapidassist/filesystem.h"

using namespace ra::gtesthelp;

const char * getTestInputFolder()
{
  return "test_files";
}
const char * getTestOutputFolder()
{
  return "test_out";
}

int main(int argc, char **argv)
{
  //check input folder
  if (!ra::filesystem::folderExists(getTestInputFolder()))
  {
    printf("Input test folder not found: '%s'.\n", getTestInputFolder() );
    return 1;
  }

  //create output folder
  if (!ra::filesystem::folderExists(getTestOutputFolder()))
  {
    bool created = ra::filesystem::createFolder(getTestOutputFolder());
    if (!created)
    {
      printf("Failed creating test output folder '%s'.\n", getTestOutputFolder() );
      return 2;
    }
  }

  //define default values for xml output report
  if (isProcessorX86())
  {
    if (isDebugCode())
      ::testing::GTEST_FLAG(output) = "xml:libmidi_unittest.x86.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:libmidi_unittest.x86.release.xml";
  }
  else if (isProcessorX64())
  {
    if (isDebugCode())
      ::testing::GTEST_FLAG(output) = "xml:libmidi_unittest.x64.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:libmidi_unittest.x64.release.xml";
  }

  ::testing::GTEST_FLAG(filter) = "*";
  ::testing::InitGoogleTest(&argc, argv);

  int wResult = RUN_ALL_TESTS(); //Find and run all tests
  return wResult; // returns 0 if all the tests are successful, or 1 otherwise
}
