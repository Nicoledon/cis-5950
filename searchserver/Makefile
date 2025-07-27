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
CXXFLAGS = -g3 -gdwarf-4 -Wall -Wpedantic -std=c++2b -pthread -I. -O0

# define common dependencies
COMMON_OBJS = ThreadPool.o ServerSocket.o HttpSocket.o WordIndex.o HttpUtils.o CrawlFileTree.o

HEADERS = HttpSocket.hpp \
	      ServerSocket.hpp \
	      ThreadPool.hpp \
	      HttpUtils.hpp \
          WordIndex.hpp \
	  CrawlFileTree.hpp \
          Result.hpp

TESTOBJS = test_wordindex.o \
          	   test_crawlfiletree.o \
		    test_serversocket.o \
           test_httpsocket.o test_httputils.o \
           test_threadpool.o test_suite.o catch.o

CPP_SOURCE_FILES = HttpSocket.cpp HttpUtils.cpp ServerSocket.cpp WordIndex.cpp ThreadPool.cpp CrawlFileTree.hpp searchserver.cpp
HPP_SOURCE_FILES = WordIndex.hpp ThreadPool.hpp ServerSocket.hpp HttpSocket.hpp HttpUtils.hpp CrawlFileTree.cpp

# compile everything except our release-only "with flaws" binary; this
# is the default rule that fires if a user just types "make" in the
# same directory as this Makefile
all: searchserver test_suite

searchserver: searchserver.o $(COMMON_OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJS) $(LDFLAGS)

test_suite: $(TESTOBJS) $(COMMON_OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(TESTOBJS) $(COMMON_OBJS)

test_suite.o: test_suite.cpp catch.hpp
	$(CXX) $(CXXFLAGS) -c $<

catch.o: catch.cpp catch.hpp
	$(CXX) $(CXXFLAGS) -c $<

test_wordindex.o: test_wordindex.cpp catch.hpp WordIndex.hpp
	$(CXX) $(CXXFLAGS) -c $<

test_serversocket.o: test_serversocket.cpp catch.hpp ServerSocket.hpp
	$(CXX) $(CXXFLAGS) -c $<

test_httpsocket.o: test_httpsocket.cpp catch.hpp HttpSocket.hpp
	$(CXX) $(CXXFLAGS) -c $<

test_httputils.o: test_httputils.cpp catch.hpp HttpUtils.hpp
	$(CXX) $(CXXFLAGS) -c $<

test_threadpool.o: test_threadpool.cpp catch.hpp ThreadPool.hpp
	$(CXX) $(CXXFLAGS) -c $<

# generic .o from cpp rule
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o *~ test_suite searchserver

tidy-check: 
	clang-tidy-15 \
        --extra-arg=--std=c++2b \
        -warnings-as-errors=* \
        -header-filter=.* \
        $(CPP_SOURCE_FILES) $(HPP_SOURCE_FILES)

format:
	clang-format-15 -i --verbose --style=Chromium $(CPP_SOURCE_FILES) $(HPP_SOURCE_FILES)
