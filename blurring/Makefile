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
CC = clang-18
CXX = clang++-18

# define useful flags to cc/ld/etc.
CFLAGS += -g3 -gdwarf-4 -Wall -Wpedantic -std=c2x -pthread -O0
CXXFLAGS += -g3 -gdwarf-4 -Wall -Wpedantic -std=c++2b -pthread -O0

# define common dependencies
OBJS_P1 = cqdbmp.o qdbmp.o
HEADERS_P1 = cqbmp.h qdbmp.h

CPP_SOURCE_FILES = blur_parallel.cpp blur_sequential.cpp negative.cpp

EXECS = negative blur_sequential blur_parallel compare_bmp

# compile everything; this is the default rule that fires if a user
# just types "make" in the same directory as this Makefile
all: $(EXECS)

# part 1
negative: $(OBJS_P1) negative.cpp
	$(CXX) $(CXXFLAGS) -o negative negative.cpp $(OBJS_P1)

blur_sequential: $(OBJS_P1) blur_sequential.cpp
	$(CXX) $(CXXFLAGS) -o blur_sequential blur_sequential.cpp $(OBJS_P1)

blur_parallel: $(OBJS_P1) blur_parallel.cpp
	$(CXX) $(CXXFLAGS) -o blur_parallel blur_parallel.cpp $(OBJS_P1)

compare_bmp: $(OBJS_P1) compare_bmp.cpp
	$(CXX) $(CXXFLAGS) -o compare_bmp compare_bmp.cpp $(OBJS_P1)

# generic
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -pthread

clean:
	rm -f *.o $(EXECS)

tidy-check: 
	clang-tidy-15 \
        --extra-arg=--std=c++2b \
        -warnings-as-errors=* \
        -header-filter=.* \
        $(CPP_SOURCE_FILES)

format:
	clang-format-15 -i --verbose --style=Chromium $(CPP_SOURCE_FILES)
