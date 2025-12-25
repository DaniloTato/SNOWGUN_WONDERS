# SNOWGUN WONDER
v0.1.0-alpha

## Build Instructions

This project is written in **C++17** and uses **SFML** and **nlohmann/json**.

```
                                 
             ############        
            ##::.......::##      
            ##-----------##      
        ******-----------##      
            **.........########  
      ********.:#:.+*******--##  
    ****  ####.....+***..####    
      ####++##.:#:.......##      
    ##++**++##...*#####++##      
  ##++**+++++=...=***##..######  
  ##**********:::......::++##  ##
##++++++#########################
##++++++#############::..::######
##****....::#########::....######
##**++%%++++%%%#+++++%%++++##    
  ##%%..******+*%##%%..******##  
  ##%%%%**....:-## %%%%**....::##      
```
---

## Requirements

### Common (Linux / macOS)
- C++ compiler supporting C++17 (`g++` or `clang++`)
- `make`
- SFML 2.6.0
- nlohmann/json (header-only)

### Installing dependencies

#### macOS (Homebrew)
```bash
brew install sfml nlohmann-json
```

#### Ubuntu / Debian
```bash
sudo apt update
sudo apt install libsfml-dev nlohmann-json3-dev
```

---

# Build & Distribution Guide — SNOWGUN WONDER

## Compile using Make (Development)

This is the **development workflow**.  
The executable runs **from the project root** and uses `./assets` and `./config`.

### Build
```bash
make
```

This produces the executable:
```bash
./snowgun
```

### Run
```bash
make run
```

### Clean (development build + objects)
```bash
make clean
```

---

## Create a Distributable Build (macOS)

This workflow creates a **self-contained folder** suitable for distribution.  
The executable runs from inside `dist/` and uses `dist/assets` and `dist/config`.

### Build the distributable binary
```bash
make dist
```

This produces:
```bash
dist/snowgun
```

### Copy runtime assets and libraries
```bash
make dist-package
```

This creates the following structure:
```bash
dist/
├── snowgun
├── assets/
├── config/
└── lib/        # SFML dynamic libraries
```

### Run the distributable build
```bash
make dist-run
```

### Clean distributable files only
```bash
make dist-clean
```

---

## Create a ZIP Release (macOS)

To generate a release archive containing everything needed to run the game:

```bash
sudo make zip
```

This produces:
```bash
snowgun-macos-[version].zip
```

The ZIP contains:
- The `snowgun` executable
- All required assets
- Configuration files
- SFML dynamic libraries

This archive can be shared and run without rebuilding the project.


## Compile manually (without Make)

```bash
g++ -std=c++17 -Wall -g \
    $(find src scripts/src blueprints/src scene_builders/src -name '*.cpp') \
    -Iinclude -Iinclude/ui -Iscripts/include -Iblueprints/include -Iscene_builders/include \
    $(pkg-config --cflags --libs sfml-graphics sfml-window sfml-audio sfml-system) \
    -o xmas_app
```

> On systems without `pkg-config`, replace the SFML flags manually.

---

## Notes
- Object files are stored under dist/obj/
- Assets and config are loaded relative to the executable
- Tested on MacOS

--

# Default Key Bindings

The game uses a configurable input system. Below are the default key bindings:

## Controls Overview

### Movement & Gameplay

| Action | Key |
|-------|-----|
| Move Left | `A` |
| Move Right | `D` |
| Move Up | `W` |
| Move Down | `S` |
| Jump | `M` |
| Shoot | `N` |
| Kick | `Left Shift` |
| Change Gun | `F` |
| Start / Confirm | `Enter` |
| Crash | `U` |

### Development & Debug Tools

| Action | Key |
|-------|-----|
| Zoom In | `O` |
| Zoom Out | `L` |
| Start Alarm | `Num 1` |
| Dramatic Zoom | `Num 2` |
| God Mode | `G` |

### Level Manager / Editor

| Action | Key |
|-------|-----|
| Tile Picker Menu | `T` |
| Create Tile | `Space` |
| Delete Tile | `Left Control` |
| Save Level | `Num 0` |

```json
{
    "bindings": {
        "left": "A",
        "right": "D",
        "up": "W",
        "down": "S",
        "jump": "M",
        "zoomIn": "O",
        "zoomOut": "L",
        "startAlarm": "Num1",
        "dramaticZoom": "Num2",
        "saveLevel": "Num0",
        "createTile": "Space",
        "deleteTile": "LControl",
        "tilePicker": "T",
        "shoot": "N",
        "crash": "U",
        "kick": "LShift",
        "godMode": "G",
        "changeGun": "F",
        "start": "Enter"
    }
}
```

These keys are set by default and can be modified through the input configuration file.
Do as you please.

```bash
    /assets/config/control_config.json
```