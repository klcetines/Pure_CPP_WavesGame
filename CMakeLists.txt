cmake_minimum_required(VERSION 3.16)
project(Pure_CPP_WavesGame)

set(CMAKE_CXX_STANDARD 17)

# Buscar SFML automáticamente
find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)

# Buscar todos los .cpp en src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Incluir tu carpeta "include"
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Vincular SFML
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
