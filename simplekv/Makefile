.PHONY = clean all tidy-check format

CPP_SOURCE_FILES = SimpleKV.cpp
HPP_SOURCE_FILES = SimpleKV.hpp

TEST_FILES = test_basic.cpp test_stress.cpp

# define the commands we will use for compilation and library building
CC = clang-15
CXX = clang++-15

# define useful flags to cc/ld/etc.
CFLAGS += -g3 -gdwarf-4 -Wall -Werror -Wpedantic -I. -I.. -std=c2x
CXXFLAGS += -g3 -gdwarf-4 -Wall -Werror -Wpedantic -I. -I.. -std=c++2b

all: test_suite

test_suite: test_suite.o SimpleKV.o test_basic.o test_stress.o catch.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test_suite_no_stress: test_suite.o SimpleKV.o test_basic.o catch.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test_stress.o: test_stress.cpp catch.hpp SimpleKV.hpp
	$(CXX) $(CXXFLAGS) -c $<

test_basic.o: test_basic.cpp catch.hpp SimpleKV.hpp
	$(CXX) $(CXXFLAGS) -c $<

SimpleKV.o: SimpleKV.cpp SimpleKV.hpp
	$(CXX) $(CXXFLAGS) -c $<

catch.o: catch.cpp catch.hpp
	$(CXX) $(CXXFLAGS) -c $<

# Phony Targets

# Explanantion of args:
#   -extra-arg=-std=c++2b : specifies to check with C++ 23
#   -warnings-as-error= : marks all warnings as errors
#   -checks= : specifies which checks to look for
#     modernize-* turns on all modernize checks to make sure we are using
#                 modern C++ features. C++ and C are different languages
#     cert-* turns on all errors related to CERT security checks.
#     readability-* turns on all errors related to making code more readable
#     cppcoreguidelines-* turns on errors related to the cppcoreguidelines
#                         https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
#     bugprone-* turns on errors that will help identify frequent beginner bugs
#     -modernize-use-trailing-return-type disables the check that insists that a function
#                                         like `int main()` be written as `auto main() -> int`
#     -modernize-use-auto disables the check that says we should use auto during
#                         variable declaration.
#     -cppcoreguidelines-owning-memory disables the check that requires use of an external library
tidy-check: 
	clang-tidy-15 \
        --extra-arg=--std=c++2b \
        -warnings-as-errors=* \
        -header-filter=.* \
        $(CPP_SOURCE_FILES) $(HPP_SOURCE_FILES)

format:
	clang-format-15 -i --verbose --style=Chromium $(CPP_SOURCE_FILES) $(HPP_SOURCE_FILES)

clean:
	rm *.o test_suite test_suite_no_stress 

