# Copyright ©2025 Travis McGaha.  All rights reserved.  Permission is
# hereby granted to students registered for University of Pennsylvania
# CIT 5950 for use solely during Spring Semester 2025 for purposes of
# the course.  No other use, copying, distribution, or modification
# is permitted without prior written consent. Copyrights for
# third-party components of this work must be honored.  Instructors
# interested in reusing these course materials should contact the
# author.

.PHONY = clean all tidy-check format

# define the commands we will use for compilation and library building
CXX = clang++-18

# define useful flags to cc/ld/etc.
CXXFLAGS += -g3 -gdwarf-4 -Wall -Wpedantic -I. -I.. -std=c++2b -O0

# define common dependencies
OBJS = SimpleFileReader.o BufferedFileReader.o
HEADERS = SimpleFileReader.hpp BufferedFileReader.hpp BufferChecker.hpp
TESTOBJS = test_simplefilereader.o test_bufferedfilereader.o test_performance.o test_suite.o catch.o

CPP_SOURCE_FILES = SimpleFileReader.cpp BufferedFileReader.cpp
HPP_SOURCE_FILES = SimpleFileReader.hpp BufferedFileReader.hpp BufferChecker.hpp

# compile everything; this is the default rule that fires if a user
# just types "make" in the same directory as this Makefile
all: test_suite

test_suite: $(TESTOBJS)  $(OBJS)
	$(CXX) $(CFLAGS) -o test_suite $(TESTOBJS) $(OBJS)

catch.o: catch.cpp catch.hpp
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o test_suite

# Phony Targets

tidy-check: 
	clang-tidy-15 \
        --extra-arg=--std=c++2b \
        -warnings-as-errors=* \
        -header-filter=.* \
        $(CPP_SOURCE_FILES)

format:
	clang-format-15 -i --verbose --style=Chromium $(CPP_SOURCE_FILES)

