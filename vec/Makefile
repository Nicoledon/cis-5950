.PHONY = all clean

all: test_vec

test_suite: catch.o test_suite.o vec.o
	clang++-18 -g3 -gdwarf-4 -Wall --std=c++2b -o test_suite catch.o test_suite.o vec.o

vec.o: vec.cpp vec.hpp
	clang++-18 -g3 -gdwarf-4 -Wall --std=c++2b -c vec.cpp

test_vec: catch.o vec.o test_vec.o
	clang++-18 -g3 -gdwarf-4 -Wall --std=c++2b -o test_vec catch.o test_vec.o vec.o

test_vec.o :test_vec.cpp catch.hpp vec.hpp
	clang++-18 -g3 -gdwarf-4 -Wall --std=c++2b -c test_vec.cpp

test_suite.o: test_suite.cpp catch.hpp
	clang++-18 -g3 -gdwarf-4 -Wall --std=c++2b -c test_suite.cpp

catch.o: catch.cpp catch.hpp
	clang++-18 -g3 -gdwarf-4 -Wall --std=c++2b -c catch.cpp


clean:
	rm *.o test_vec