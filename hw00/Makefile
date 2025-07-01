.PHONY = all clean

all: check-time

check-time: check-time.o ctypes.o simple_string.o
	clang++ -g3 -gdwarf-4 --std=c++2b -Wall -o check-time check-time.o ctypes.o simple_string.o

check-time.o: check-time.cpp ctypes.hpp
	clang++ -g3 -gdwarf-4 --std=c++2b -Wall -c check-time.cpp

ctypes.o: ctypes.cpp ctypes.hpp
	clang++ -g3 -gdwarf-4 --std=c++2b -Wall -c ctypes.cpp

cache.o: catch.cpp catch.hpp
	clang++ -g3 -gdwarf-4 -std=c++2b -Wall  -c catch.cpp

test_simple_string.o: test_simple_string.cpp simple_string.hpp catch.hpp 
	clang++ -g3 -gdwarf-4 -std=c++2b -Wall  -c test_simple_string.cpp 

test_simple_string: test_simple_string.o simple_string.o catch.o
	clang++ -g3 -gdwarf-4 -std=c++2b -Wall  -o test_simple_string test_simple_string.o simple_string.o catch.o

simple_string.o: simple_string.cpp simple_string.hpp
	 clang++ -g3 -gdwarf-4 --std=c++2b -Wall -c simple_string.cpp
clean:
	rm check-time *.o 
clean simple_string_clean:
	rm simple_string *.o
