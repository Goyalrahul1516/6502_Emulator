#include <cstdio>
#include "gtest/gtest.h"

// Normal platforms: program entry point is main, argc/argv are initialized.
GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from %s\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}