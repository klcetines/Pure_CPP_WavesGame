CXX = g++
CXXFLAGS = -Iinclude -IC:/msys64/mingw64/include
LDFLAGS = -LC:/msys64/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system

SRCS = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)
OBJS = $(SRCS:.cpp=.o)

bin/game.exe: $(OBJS)
	mkdir -p bin
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) bin/game.exe

.PHONY: clean