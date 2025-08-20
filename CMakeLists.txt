cmake_minimum_required(VERSION 3.15)
project(MultiPatcher VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add library
add_library(IPS STATIC src/ips.cpp)
target_include_directories(IPS PUBLIC include)

# Enable testing
enable_testing()

# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)

# Add test executable
add_executable(MultiPatcher_tests tests/tests.cpp)
target_link_libraries(MultiPatcher_tests PRIVATE IPS gtest_main)

# Add executable for release/demo
add_executable(MultiPatcherApp app/main.cpp)
target_link_libraries(MultiPatcherApp PRIVATE IPS)

# Register tests
include(GoogleTest)
gtest_discover_tests(MultiPatcher_tests)
