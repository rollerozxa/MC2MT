# MC2MT
Converts a Minecraft world into a Minetest world. Compared to the `mcimport` tool written in Python it is **very fast**, and also allows for multhreading the workflow making it even faster.

This tool should be compatible with Minecraft worlds stored in Scaevolous' McRegion format as well as Anvil format, up until Minecraft 1.12 (pre-flattening). It is however recommended that if you have a world in an older version of Minecraft that you fully upgrade it to the 1.12 Anvil format for best support as that is what the program is usually tested against.

If you have a Minecraft world post-flattening (i.e. 1.13 or newer), then you can convert it to the 1.12 format using [Amulet](https://www.amuletmc.com/).

When mapping Minecraft block IDs and data values into Minetest itemstrings and param2 values, a set of mappings for Mineclonia is used which can be found at `src/conversions.h`. When you put the converted world into your worlds folder it will be visible in the main menu when you select Mineclonia (not MineClone2, not Minetest Game).

Currently, the mappings are mostly complete for basic full building blocks, as that is what my usecase for a converter program is. If you convert a world that contains more complex blocks and it gets converted wrongly then please help fixing them so it can become better.

## Usage
It is a command-line program. Call it from the terminal with two arguments, first argument is the input Minecraft world and second argument is the output Minetest world. For example:

```bash
./MC2MT ~/.minecraft/worlds/cool_world/ ~/.minetest/worlds/cool_world/
```

The program will also make a `world.mt` file as well as a worldmod that sets the mapgen to singlenode along with other things. If it detects a map database already present in the output location it will ask you before overwriting it.

## Building
There is CI in place for building on Windows and Linux, which also produce binary artifacts that can be downloaded and used. This is especially useful for Windows users, but the Linux binary may not work for you depending on what distribution you are on.

### Linux
Install the dependencies. In addition to a compiler toolchain, MC2MT requires SQLite3 and Zlib.

Debian-based distros:

```bash
sudo apt install g++ cmake ninja-build libsqlite3-dev zlib1g-dev
```

Arch-based distros:

```bash
sudo pacman -S base-devel cmake ninja sqlite zlib
```

Then generate the build files with CMake and build:

```bash
cmake . -G Ninja
ninja
```

The resulting executable can be found as `./bin/MC2MT`.

### Windows
Install [MSYS2](https://www.msys2.org/). When greeted with the UCRT64 terminal (icon with gold background), run the following command to install dependencies:

```bash
pacman -S mingw-w64-ucrt-x86_64-{gcc,cmake,ninja,zlib,sqlite3}
```

Then generate the build files with CMake and build:

```bash
cmake . -G Ninja
ninja
```

The resulting executable can be found as `./bin/MC2MT.exe`.

The executable can be run perfectly fine inside of the UCRT64 environment, but if you want to run it outside of MSYS2 (somewhy) then you will need to bundle the necessary DLL files next to it. To collect them up see the [msys2-bundledlls](https://github.com/rollerozxa/msys2-bundledlls) script, or how the CI does it to statically link the necessary libraries into the executable.

### Android
You will need to install [Termux](https://termux.dev/). When installed, install the following packages in Termux:

```bash
pkg install clang cmake ninja libsqlite zlib
```

Then build:

```bash
cmake . -G Ninja
ninja
```

The resulting executable can be found as `./bin/MC2MT`.

## Credits
The original MC2MT was written by ShadowNinja, assumedly rewriting things from the Python `mcimport` project by sofar, Ekdohibs et al.

The Mineclonia node mappings are based on the `mcimport` node mappings for MineClone2 by MysticTempest.

License: LGPLv2.1+
