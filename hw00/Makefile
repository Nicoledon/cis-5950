.PHONY = all clean

all: check-time

check-time: check-time.o ctypes.o simple_string.o
	clang++-15 -g3 -gdwarf-4 --std=c++20 -Wall -o check-time check-time.o ctypes.o simple_string.o

check-time.o: check-time.cpp ctypes.hpp
	clang++-15 -g3 -gdwarf-4 --std=c++20 -Wall -c check-time.cpp

ctypes.o: ctypes.cpp ctypes.hpp
	clang++-15 -g3 -gdwarf-4 --std=c++20 -Wall -c ctypes.cpp
simple_string.o: simple_string.cpp simple_string.hpp
	 clang++-15 -g3 -gdwarf-4 --std=c++20 -Wall -c simple_string.cpp
catch.o : catch.cpp catch.hpp
	    clang++-15 -g -gdwarf-4 -Wall -std=c++2b -c catch.cpp
clean:
	rm check-time *.o
