# Copyright ©2025 Travis McGaha.  All rights reserved.  Permission is
# hereby granted to students registered for University of Pennsylvania
# CIT 5950 for use solely during Spring Semester 2025 for purposes of
# the course.  No other use, copying, distribution, or modification
# is permitted without prior written consent. Copyrights for
# third-party components of this work must be honored.  Instructors
# interested in reusing these course materials should contact the
# author.

.PHONY = all tidy-check format clean

all: pipe_shell

pipe_shell: pipe_shell.cpp
	clang++-18 -g3 -gdwarf-4 -Wall -std=c++2b -o pipe_shell pipe_shell.cpp

tidy-check: 
	clang-tidy-15 \
        --extra-arg=--std=c++2b \
        -warnings-as-errors=* \
        -header-filter=.* \
        pipe_shell.cpp

format:
	clang-format-15 -i --verbose --style=Chromium pipe_shell.cpp


clean:
	rm -f *.o pipe_shell
