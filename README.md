# Minimalist CHIP-8 Interpreter / Emulator

A lightweight, modern C++ implementation of a CHIP-8 virtual machine. This project was built as an educational exercise to learn core computer architecture concepts—such as opcode fetching and decoding, register management, program counters, memory indexing, stack execution, and graphics rendering.

Rendered using **[Raylib](https://www.raylib.com/)** with customized phosphor decay to prevent display flickering.

---

## Features & Implementation Highlights

- **Decoupled Architecture:** Core CPU emulation logic (`Chip8`) is isolated from graphics rendering (`DisplayManager`) and window input.
- **Flicker Reduction:** Custom display decay pipeline fades drawn pixels gradually across frames to eliminate sub-frame XOR flashing in games like Pong.
- **Modern C++ Standard:** Built using `std::array`, fixed-width integer types (`<cstdint>`), standard library file streams, and modern C++ PRNG (`std::mt19937`).
- **Accurate Opcode Handling:** Complete support for original CHIP-8 opcode quirks, including binary-coded decimal (BCD) conversion and flexible memory register store/load operations (`Fx55` / `Fx65`).

---

## Tested ROMs

This emulator has been verified and tested against the following ROMs:

- **`pong.rom`** — Fully functional. Frame-synchronized rendering and display decay prevent ball and paddle flickering.
- **`tetris.rom`** — Fully functional. Proper register increment handling (`Fx55` / `Fx65`) ensures piece collision detection and game termination trigger correctly at the top boundary.

---

## Project Structure

```text
.
├── chip8.hpp        # Core CPU declarations, memory maps, registers, and quirks
├── chip8.cpp        # Opcode fetch-decode-execute loop & CPU lifecycle logic
├── graphics.hpp     # Raylib display manager & flicker-suppression rendering engine
├── main.cpp         # Main application loop, frame timing (60Hz), and keymapping
└── README.md
```

---

## Key Mapping

The standard COSMAC VIP 4×4 keypad is mapped to modern keyboard keys:

```text
CHIP-8 Keypad          Keyboard Controls

+---+---+---+---+      +---+---+---+---+
| 1 | 2 | 3 | C |      | 1 | 2 | 3 | 4 |
+---+---+---+---+      +---+---+---+---+
| 4 | 5 | 6 | D |  =>  | Q | W | E | R |
+---+---+---+---+      +---+---+---+---+
| 7 | 8 | 9 | E |      | A | S | D | F |
+---+---+---+---+      +---+---+---+---+
| A | 0 | B | F |      | Z | X | C | V |
+---+---+---+---+      +---+---+---+---+
```

---

## Build & Compilation

### Prerequisites

- C++17 compliant compiler (`g++`, `clang++`, or MSVC)
- [Raylib](https://www.raylib.com/) development library

---

### Linux (GCC / Clang)

#### Install Raylib (Ubuntu / Debian)

```bash
sudo apt update
sudo apt install libraylib-dev libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
```

#### Compile with GCC

```bash
g++ -std=c++17 main.cpp chip8.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o chip8
```

#### Compile with Clang

```bash
clang++ -std=c++17 main.cpp chip8.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o chip8
```

---

### macOS (Clang)

#### Install Raylib

```bash
brew install raylib
```

#### Compile

```bash
clang++ -std=c++17 main.cpp chip8.cpp -lraylib \
-framework OpenGL \
-framework Cocoa \
-framework IOKit \
-framework CoreVideo \
-o chip8
```

---

### Windows (MinGW)

```bash
g++ -std=c++17 main.cpp chip8.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -o chip8.exe
```

---

### Windows (MSVC)

```cmd
cl /EHsc /std:c++17 main.cpp chip8.cpp ^
/I"path\to\raylib\include" ^
/link ^
/LIBPATH:"path\to\raylib\lib" ^
raylib.lib opengl32.lib gdi32.lib winmm.lib shell32.lib ^
/out:chip8.exe
```

---

## Running a ROM

After compilation, pass the ROM file as a command-line argument.

### Linux / macOS

```bash
./chip8 path/to/pong.rom
```

### Windows

```cmd
chip8.exe path\to\tetris.rom
```

---

## Technical Specifications

| Parameter | Specification |
|-----------|---------------|
| **RAM** | 4096 bytes (`0x000`–`0xFFF`) |
| **Program Start Address** | `0x200` |
| **Registers** | 16 general-purpose 8-bit registers (`V0`–`VF`) |
| **Index Register (`I`)** | 16-bit |
| **Program Counter (`PC`)** | 16-bit |
| **Stack** | 16 levels (16-bit) |
| **Display Resolution** | 64 × 32 monochrome pixels |
| **Execution Speed** | ~540 Hz (9 CPU cycles per 60 Hz frame) |
| **Timers** | Delay Timer & Sound Timer running at 60 Hz |

---

## License

This project is intended for educational purposes to explore virtual machine architecture, instruction decoding, graphics rendering, and emulator development.
