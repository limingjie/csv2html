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

FLTK_CXXFLAGS  = $(shell fltk-config --cxxflags)
FLTK_LDSTATIC  = $(shell fltk-config --ldstaticflags)
CXXFLAGS = -Wall -g -O2 -std=c++0x

all: csv2html$(X) csv2html_gui$(X)

csv2html$(X): csvmm.o csv2html.o
	@echo "==>Linking csv2html$(X)..."
	$(CXX) -o csv2html$(X) csvmm.o csv2html.o

csv2html.o: csv2html.cpp csvmm.hpp
	@echo "==>Compiling csv2html.o..."
	$(CXX) -c $(CXXFLAGS) -o csv2html.o csv2html.cpp

csvmm.o: csvmm.cpp csvmm.hpp
	@echo "==>Compiling csvmm.o..."
	$(CXX) -c $(CXXFLAGS) -o csvmm.o csvmm.cpp

csv2html_gui$(X): csv2html_gui.o csvmm.o
	$(CXX) -o csv2html_gui$(X) csv2html_gui.o csvmm.o $(FLTK_LDSTATIC)

csv2html_gui.o: csv2html_gui.cpp
	$(CXX) -c $(CXXFLAGS) $(FLTK_CXXFLAGS) -o csv2html_gui.o csv2html_gui.cpp

clean:
	@echo "==>Cleaning..."
	rm -rf csv2html.o csvmm.o
	rm -rf csv2html$(X)
