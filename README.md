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
- SFML 2.5+
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

## Compile using Make (recommended)

```bash
make
```

Run:
```bash
./xmas_app
```

Clean:
```bash
make clean
```

---

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
- The build system auto-discovers `.cpp` files.
- Object files are stored in the `obj/` directory.
- Tested on Linux and macOS.

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