cmake_minimum_required(VERSION 3.16)
project(Pure_CPP_WavesGame)

set(CMAKE_CXX_STANDARD 17)

# Locate SFML
find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)

# Collect all .cpp files under src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add the include directory
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link SFML
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
