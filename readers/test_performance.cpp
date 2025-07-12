/*
 * Copyright ©2025 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIT 5950 for use solely during Spring Semester 2025 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <cmath>
#include <errno.h>
#include <iostream>
#include <sys/select.h>
#include <time.h> // POSIX
#include <unistd.h>

#include "./BufferedFileReader.hpp"
#include "./SimpleFileReader.hpp"
#include "./catch.hpp"

static constexpr const char *kLongFileName = "./test_files/war_and_peace.txt";

static uint64_t get_ms() {
  struct timespec spec;
  time_t seconds;
  uint64_t milli;

  clock_gettime(CLOCK_REALTIME, &spec);

  seconds = spec.tv_sec;
  milli = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
  milli += seconds * 1000;
  return milli;
}

TEST_CASE("Basic", "[Test_Performance]") {
  BufferedFileReader bf(kLongFileName);
  SimpleFileReader sf(kLongFileName);
  char c;

  uint64_t start_time = get_ms();

  do {
    c = sf.get_char();
  } while (c != static_cast<char>(EOF));

  uint64_t end_time = get_ms();

  uint64_t simple_time = end_time - start_time;

  std::cout << "Time (ms) for SimpleFileReader to read \"War and Peace\": "
            << simple_time << std::endl;

  start_time = time(nullptr);

  do {
    c = bf.get_char();
  } while (c != static_cast<char>(EOF));
  end_time = time(nullptr);

  uint64_t buffered_time = end_time - start_time;

  std::cout << "Time (ms) for BufferedFileReader to read \"War and Peace\": "
            << buffered_time << std::endl;

  REQUIRE(buffered_time * 3 < simple_time);
}
