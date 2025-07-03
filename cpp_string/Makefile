.PHONY = all clean

all: test_suite

test_suite: catch.o test_suite.o simple_string.o
	clang++-18 -g3 -gdwarf-4 -Wall --std=c++2b -o test_suite catch.o test_suite.o simple_string.o

test_suite.o: test_suite.cpp catch.hpp
	clang++-18 -g3 -gdwarf-4 -Wall --std=c++2b -c test_suite.cpp

simple_string.o: simple_string.cpp simple_string.hpp
	clang++-18 -g3 -gdwarf-4 -Wall --std=c++2b -c simple_string.cpp
	
catch.o: catch.cpp catch.hpp
	clang++-18 -g3 -gdwarf-4 -Wall --std=c++2b -c catch.cpp
clean:
	rm test_suite *.o
