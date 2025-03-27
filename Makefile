# Makefile for olcPixelgameEngine on Mac (THANK YOU REDDIT!)
CXX = clang++
CXXFLAGS = -std=c++17 -mmacosx-version-min=10.15 -Wall
LDFLAGS = -framework OpenGL -framework GLUT -framework Cocoa
LDLIBS = -lpng

INCLUDE_DIR = -Isrc -I/opt/homebrew/include
LIB_DIR = -L/opt/homebrew/lib

TARGET = MineSweeper
SRC = MineSweeper.cpp


all: $(TARGET)

$(TARGET): ms_grid.o ms_input_controller.o MineSweeper.o
	$(CXX) $(CXXFLAGS) $(LIB_DIR) $^ -o $@ $(LDFLAGS) $(LDLIBS)

ms_grid.o: ms_grid.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c $< -o $@

ms_input_controller.o: ms_input_controller.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c $< -o $@

MineSweeper.o: MineSweeper.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c $< -o $@

clean:
	$(RM) $(TARGET) ms_grid.o main.o ms_input_controller.o