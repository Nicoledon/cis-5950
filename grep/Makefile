.PHONY = clean all tidy-check format

CPP_SOURCE_FILES = grep.cpp grep_sequential.cpp RequestQueue.cpp
HPP_SOURCE_FILES = grep_request.hpp RequestQueue.hpp

all: grep_sequential test_suite grep

grep_sequential: grep_sequential.o RequestQueue.o
	clang++-18 -g3 -gdwarf-4 -pthread --std=c++2b -o grep_sequential grep_sequential.o RequestQueue.o

grep_sequential.o: grep_sequential.cpp grep_request.hpp
	clang++-18 -g3 -gdwarf-4 -pthread --std=c++2b -c grep_sequential.cpp

test_suite: test_suite.o catch.o test_requestqueue.o RequestQueue.o
	clang++-18 -g3 -gdwarf-4 -pthread --std=c++2b -o test_suite test_suite.o catch.o test_requestqueue.o RequestQueue.o

test_requestqueue.o: test_requestqueue.cpp catch.hpp RequestQueue.hpp grep_request.hpp
	clang++-18 -g3 -gdwarf-4 -pthread --std=c++2b -c test_requestqueue.cpp

test_suite.o: catch.hpp test_suite.cpp
	clang++-18 -g3 -gdwarf-4 -pthread --std=c++2b -c test_suite.cpp

catch.o: catch.hpp catch.cpp
	clang++-18 -g3 -gdwarf-4 -pthread --std=c++2b -c catch.cpp

RequestQueue.o: RequestQueue.cpp grep_request.hpp
	clang++-18 -g3 -gdwarf-4 -pthread --std=c++2b -c RequestQueue.cpp

grep: grep.o RequestQueue.o
	clang++-18 -g3 -gdwarf-4 -pthread --std=c++2b -o grep grep.o RequestQueue.o

grep.o: grep.cpp grep_request.hpp RequestQueue.hpp
	clang++-18 -g3 -gdwarf-4 -pthread --std=c++2b -c grep.cpp


clean:
	rm *.o grep grep_sequential test_suite

tidy-check: 
	clang-tidy-15 \
        --extra-arg=--std=c++2b \
        -warnings-as-errors=* \
        -header-filter=.* \
        $(CPP_SOURCE_FILES) $(HPP_SOURCE_FILES)

format:
	clang-format-15 -i --verbose --style=Chromium $(CPP_SOURCE_FILES) $(HPP_SOURCE_FILES)
