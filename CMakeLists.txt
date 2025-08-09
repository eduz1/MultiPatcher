cmake_minimum_required(VERSION 3.15)
project(MultiPatcher)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
cmake_policy(SET CMP0135 NEW)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(ips src/IPS.cpp)
add_library(bps src/BPS.cpp)

target_include_directories(ips PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(bps PUBLIC "${PROJECT_SOURCE_DIR}/include")
add_executable(MultiPatcher src/main.cpp)
target_include_directories(MultiPatcher PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Link to the appropriate library (choose one or both as needed)
target_link_libraries(MultiPatcher ips bps)

enable_testing()

add_executable(
  main_test
  tests/main_test.cc
)
target_link_libraries(
  main_test
  GTest::gtest_main
  ips
)

include(GoogleTest)
gtest_discover_tests(main_test)
