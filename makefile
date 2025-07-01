CXX = g++
CXXFLAGS = -Iinclude -IC:/msys64/mingw64/include
LDFLAGS = -LC:/msys64/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system
SRC = $(wildcard src/*.cpp)
OUT = bin/game.exe

all:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)
