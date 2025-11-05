# dna-lua-loaderlua-loader

## Overview

This project is a C++ DLL-based modification tool for DNA game. It provides a Lua script overriding mechanism to customize game logic and behavior.

## Features

- **Lua Script Overriding**: Allows users to replace in-game Lua scripts with external files. By placing `.lua` files in a `Script` folder located next to the DLL, the tool will automatically load these external scripts instead of the game's original ones. This enables dynamic modification of game logic without altering game files directly.
  - **Usage**: Create a `Script` folder in the same directory as the DLL. Place your custom Lua files inside this `Script` folder, maintaining the relative path structure found in the game (e.g., if the game loads `Game/****/script/****/logic.lua`, place your custom file at `Script/****/logic.lua`). The system will automatically normalize paths (lowercase, forward slashes) for matching.
- **Early Hooking**: Implements early hooking mechanisms for critical game functions.
- **Logging**: Provides detailed logging for debugging and monitoring purposes.

## Usage

1.  Compile the project using Visual Studio (or download from [Action]())
2.  Put the compiled DLL file into the game folder.
3.  Start the game. The Lua script overriding will be active automatically.
4.  (Optional) If a UI is implemented, press F12 to show the panel.

## Disclaimer

This tool is for educational and research purposes only. Use responsibly and in accordance with the game's terms of service.

## Special Thanks

- [Unlua](https://github.com/Tencent/UnLua)
