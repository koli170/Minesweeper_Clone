# Makefile for olcPixelgameEngine on Mac (THANK YOU REDDIT!)
CXX = clang++
CXXFLAGS = -std=c++17 -mmacosx-version-min=10.15 -Wall
LDFLAGS = -framework OpenGL -framework GLUT -framework Cocoa
LDLIBS = -lpng

INCLUDE_DIR = -Isrc -I/opt/homebrew/include
LIB_DIR = -L/opt/homebrew/lib

TARGET = MineSweeper
SRC = MineSweeper.cpp


all:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) $(LIB_DIR) $(LDFLAGS) $(LDLIBS) $(SRC) -o $(TARGET)


clean:
	$(RM) $(TARGET)