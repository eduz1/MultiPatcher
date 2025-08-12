#include <gtest/gtest.h>
#include "../include/ips.h"

TEST(TestsIPS, NESRomDefault) 
{
  IPS ipsPatcher;
  // Test with default output (output file inferred)
  EXPECT_TRUE(ipsPatcher.PatchIPS("../../Tests/roms/Metroid.nes", "../../Tests/roms/Metroid.ips"));
}

TEST(TestsIPS, NESRomCustom) 
{
  IPS ipsPatcher;
  // Test with custom output file
  EXPECT_TRUE(ipsPatcher.PatchIPS("../../Tests/roms/Metroid.nes", "../../Tests/roms/Metroid.ips", "./Metroid_patched.nes"));
}