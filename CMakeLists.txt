cmake_minimum_required(VERSION 3.5.1)
project(yawiel C CXX)

# Compilation options.
option(BUILD_TESTS "Build tests." ON)

# Set C++11 standard.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Boost library.
# This list needs to be updated when new versions of Boost come out.
set(Boost_ADDITIONAL_VERSIONS
  "1.68.0" "1.68" "1.67.0"
  "1.67" "1.66.0" "1.66"
  "1.65.1" "1.65.0" "1.65"
  "1.64.1" "1.64.0" "1.64"
  "1.63.1" "1.63.0" "1.63"
  "1.62.1" "1.62.0" "1.62"
  "1.61.1" "1.61.0" "1.61"
  "1.60.1" "1.60.0" "1.60"
  "1.59.1" "1.59.0" "1.59"
  "1.58.1" "1.58.0" "1.58")
find_package( Boost 1.58 COMPONENTS unit_test_framework REQUIRED )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)

include_directories(BEFORE ${CMAKE_SOURCE_DIR}/src/)

# Build the project sources
add_subdirectory(src/yawiel)
