# Pure C++ Waves Game

A wave-survival action game written **from scratch in modern C++ (C++17)** with [SFML](https://www.sfml-dev.org/) for windowing and rendering only — **no game engine**. Every system (collision, effects, combat, enemy waves, shop, UI) is hand-built.

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)
![SFML](https://img.shields.io/badge/SFML-2.6-8CC445)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-064F8C?logo=cmake&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey)

![Gameplay](docs/gameplay.gif)

---

## Why this project

I built this without an engine on purpose: to implement the systems an engine usually hides, and to understand them end to end. The result is a small but complete game whose codebase is organized like production software — interfaces, components, factories, and a clear separation between logic, events, and rendering.

## Engineering highlights

- **Custom collision system** — A single `CollisionShape` type supporting **circles, rectangles and capsules**. The public `intersects()` is the only entry point and delegates one-way to private per-shape resolvers (`circleIntersections`, `rectangleIntersects`, `capsuleIntersections`), which keeps the dispatch logic in one place and avoids recursive cross-calls. Capsule tests are backed by hand-written 2D math (point-to-segment distance, segment intersection, 2D cross product).
- **Registry-based effect factory** — `EffectFactory` maps an integer id to a `std::function` creator and returns `std::unique_ptr<IProjectileEffect>`. New projectile effects register themselves without the factory needing to know their concrete types (open/closed in practice).
- **Composable projectile & status effects** — A strategy-style hierarchy (`Fire`, `Ice`, `Bleed`, `Homing`, `Piercing`, `Shotgun`, `Laser`, `Spinning`) layered through interfaces, so effects can be combined on a single projectile.
- **Component-based actors** — `Character` is split into focused components (`CharacterCombat`, `CharacterGraphics`, `CharacterStats`) behind an `IActor` interface, favouring composition over a deep inheritance tree.
- **Manager-driven game loop** — Dedicated `EnemiesManager`, `ProjectilesManager`, and a `GameSession` / `GameState` / `GameStatistics` split that keeps the main loop thin.
- **Logic / events / rendering separation** — Subsystems like the shop and the wave rearrangement are each split into core, `*Events`, and `*Render` units, so gameplay rules don't bleed into draw code.
- **Modern C++ & tooling** — C++17, RAII with `std::unique_ptr`, cross-platform `CMake` build for Windows (MSYS2/MinGW64) and Linux.

## Project structure

```
include/ , src/        # headers mirror sources
├── Character/         # Character + Combat / Graphics / Stats components
├── Enemies/           # Enemy + EnemiesManager (wave spawning)
├── Projectiles/       # Projectile, PlayerProjectile, ProjectilesManager
├── Effects/
│   ├── ProjectileEffects/   # IProjectileEffect + EffectFactory + effect strategies
│   └── CharacterEffects/    # status effects (Fire/Ice/Bleed) via ActorEffectComponent
├── GameManagers/      # GameSession / GameState / GameStatistics
├── Shop/              # Shop + ShopEvents + ShopRender
├── Rearrangement/     # wave rearrangement (core / events / render)
└── Utils/             # IActor, CollisionShape, SpriteLoader, UI, Background
```

## Build & run

### Requirements
- **Windows (MSYS2 + MinGW64)** or **Linux**
- **CMake ≥ 3.16**, **SFML ≥ 2.6**, **Git**

### Install dependencies
**Windows (MSYS2):**
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-sfml git
```
**Linux (Debian/Ubuntu):**
```bash
sudo apt update && sudo apt install g++ cmake libsfml-dev git
```

### Clone & compile
```bash
git clone https://github.com/klcetines/Pure_CPP_WavesGame
cd Pure_CPP_WavesGame
mkdir build && cd build
cmake .. -G "MinGW Makefiles"   # Windows
# cmake ..                      # Linux
mingw32-make                    # Windows  (make on Linux)
```

### Run
> The game must be launched from the project root so it can find the `assets/` folder.
```bash
cd ..
./build/Pure_CPP_WavesGame.exe   # Windows (MSYS2)
./build/Pure_CPP_WavesGame       # Linux
```

## Credits
- **Programming:** [Bernat Valero Fontana](https://www.linkedin.com/in/bernatvafo/)
- **Art:** [Aymara Elorriaga](https://www.linkedin.com/in/aymara-elorriaga-criee-9b29022aa/)

## License
<!-- TODO: add a LICENSE file (MIT is a common, permissive choice for portfolio repos) and state it here. -->