#pragma once

#include <gtest/gtest.h>

class TestMidiFile : public ::testing::Test
{
public:
  virtual void SetUp();
  virtual void TearDown();
};
