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

csv2html$(X): obj obj/csvmm.o obj/csv2html.o
	@echo "==>Linking csv2html$(X)..."
	$(CXX) -o csv2html$(X) obj/csvmm.o obj/csv2html.o

csv2html_gui$(X): obj obj/csv2html_gui.o obj/csvmm.o
	@echo "==>Linking csv2html_gui$(X)..."
	$(CXX) -o csv2html_gui$(X) obj/csv2html_gui.o obj/csvmm.o $(FLTK_LDSTATIC)

obj:
	@echo "==>Creating directory for objects..."
	mkdir obj

obj/csv2html.o: src/csv2html.cpp src/csvmm.hpp
	@echo "==>Compiling src/csv2html.cpp..."
	$(CXX) -c $(CXXFLAGS) -o obj/csv2html.o src/csv2html.cpp

obj/csvmm.o: src/csvmm.cpp src/csvmm.hpp
	@echo "==>Compiling src/csvmm.cpp..."
	$(CXX) -c $(CXXFLAGS) -o obj/csvmm.o src/csvmm.cpp

obj/csv2html_gui.o: src/csv2html_gui.cpp
	@echo "==>Compiling src/csv2html_gui.cpp..."
	$(CXX) -c $(CXXFLAGS) $(FLTK_CXXFLAGS) -o obj/csv2html_gui.o src/csv2html_gui.cpp

clean:
	@echo "==>Cleaning..."
	rm -rf obj/*.o
	rm -rf csv2html$(X) csv2html_gui$(X)
