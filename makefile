CXX = g++
# Directorios de inclusión
CXXFLAGS = -Iinclude -IC:/msys64/mingw64/include -Wall

# Configuración por defecto (Debug)
# Para compilar en modo amigos usa: make release=1
ifeq ($(release), 1)
    CXXFLAGS += -O3 -DNDEBUG
    LDFLAGS = -LC:/msys64/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system -mwindows -s
else
    CXXFLAGS += -g
    LDFLAGS = -LC:/msys64/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system
endif

# Buscador de archivos fuente (soporta hasta 3 niveles de subcarpetas)
SRCS = $(wildcard src/*.cpp) \
       $(wildcard src/*/*.cpp) \
       $(wildcard src/*/*/*.cpp) \
       $(wildcard src/*/*/*/*.cpp)

OBJS = $(SRCS:.cpp=.o)

# Regla principal
bin/game.exe: $(OBJS)
	mkdir -p bin
	$(CXX) -o $@ $^ $(LDFLAGS)

# Regla de compilación de objetos
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -f $(OBJS) bin/game.exe

.PHONY: clean