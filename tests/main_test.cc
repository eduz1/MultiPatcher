#include <gtest/gtest.h>
#include "../include/ips.h"

TEST(TestsIPS, NESRom) 
{
  EXPECT_TRUE(PatchIPS("../../Tests/roms/Metroid.nes", "../../Tests/roms/Metroid.ips"));
}