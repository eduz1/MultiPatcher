#include <gtest/gtest.h>
#include "ips.h"
TEST(TestsIPS, NESRomDefault) {
	IPS ipsPatcher;
	// Test with default output (output file inferred)
	EXPECT_TRUE(ipsPatcher.PatchIPS("../../../Tests/roms/Metroid.nes", "../../../Tests/roms/Metroid.ips"));
}