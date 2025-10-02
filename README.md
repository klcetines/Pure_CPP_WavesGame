# Pure C++ Waves Game 🎮

A game developed in **C++** using [SFML](https://www.sfml-dev.org/).

It includes a system for characters, enemies, projectiles, and an in-game shop.

---

## 🚀 Installation and Execution

### 🔹 Requirements
- **Windows (MSYS2 + MinGW64)** or **Linux**
- **CMake >= 3.20**
- **SFML >= 2.6**
- **Git**

On **Windows (MSYS2)**, install dependencies:
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-sfml git
On Linux (Debian/Ubuntu):

Bash

sudo apt update
sudo apt install g++ cmake libsfml-dev git
🔹 Clone and Compile
Bash

git clone [https://github.com/klcetines/Pure_CPP_WavesGame](https://github.com/klcetines/Pure_CPP_WavesGame)
cd Pure_CPP_WavesGame

mkdir build && cd build
cmake .. -G "MinGW Makefiles"   # On Windows
cmake ..                        # On Linux
mingw32-make                    # Windows
make                            # Linux
🔹 Run the Game
⚠️ Important: The game must be executed from the root directory of the project so that it can find the assets/ folder.

Bash

cd ..
./build/Pure_CPP_WavesGame.exe    # Windows (MSYS2)
# or
./build/Pure_CPP_WavesGame        # Linux
📂 Project Structure
Pure_CPP_WavesGame/
├── assets/         # Images, fonts, sounds
├── include/        # Headers
├── src/            # Source code
├── build/          # Compilation files
├── CMakeLists.txt  # CMake configuration
└── README.md
🐛 Common Issues
"Failed to load font 'assets/fonts/Circle.otf'"
→ Ensure you execute the binary from the root of the repository (./build/Pure_CPP_WavesGame.exe).

"No targets specified and no makefile found"
→ Check that you have created the build/ folder and ran cmake .. before running make.

✨ Credits
**Programming:** @klcetines

**Art:** [Aymara Elorriaga](https://www.linkedin.com/in/aymara-elorriaga-criee-9b29022aa/)

Based on pure C++ and SFML.