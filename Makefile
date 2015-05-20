# Check OS
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

ifneq (,$(findstring MINGW,$(uname_S)))
	X=.exe
endif

ifeq ($(OS),Windows_NT)
	X=.exe
endif

# Check CXX flag.
ifeq ($(CXX),clang++)
	CXX=clang++
else
	CXX=g++
endif

CXXFLAGS   = -Wall -g -O2 -std=c++0x
BOOSTFLAGS = -I/mingw/include

all: csv2html$(X)

csv2html.o: csv2html.cpp
	@echo "==>Compiling csv2html.o..."
	$(CXX) -c $(CXXFLAGS) $(BOOSTFLAGS) -o csv2html.o csv2html.cpp

csv2html$(X): csv2html.o
	@echo "==>Linking csv2html$(X)..."
	$(CXX) -o csv2html$(X) csv2html.o

clean:
	@echo "==>Clean Objects and Executable..."
	rm -rf csv2html.o
	rm -rf csv2html$(X)
