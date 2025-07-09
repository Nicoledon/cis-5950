.PHONY = all clean

all: test_suite

test_suite: catch.o test_suite.o
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -o test_suite catch.o test_suite.o

test_suite.o: test_suite.cpp catch.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c test_suite.cpp

catch.o: catch.cpp catch.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++2b -c catch.cpp

tidy-check: 
	clang-tidy-15 \
        --extra-arg=--std=c++2b \
        -warnings-as-errors=* \
        -header-filter=.* \
        hash_table.cpp

format:
	clang-format-15 -i --verbose --style=Chromium hash_table.cpp

clean:
	rm *.o test_suite
