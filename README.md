# Minimalist CHIP-8 Interpreter / Emulator

A lightweight, modern C++ implementation of a CHIP-8 virtual machine. This project was built as an educational exercise to learn core computer architecture concepts—such as opcode fetching and decoding, register management, program counters, memory indexing, stack execution, and graphics rendering.

Rendered using **[Raylib](https://www.raylib.com/)** with customized phosphor decay to prevent display flickering.

---

## Features & Implementation Highlights

* **Decoupled Architecture:** Core CPU emulation logic (`Chip8`) is isolated from graphics rendering (`DisplayManager`) and window input.
* **Flicker Reduction:** Custom display decay pipeline fades drawn pixels gradually across frames to eliminate sub-frame XOR flashing in games like Pong.
* **Modern C++ Standard:** Built using `std::array`, fixed-width integer types (`<cstdint>`), standard library file streams, and modern C++ PRNG (`std::mt19937`).
* **Accurate Opcode Handling:** Complete support for original CHIP-8 opcode quirks, including binary-coded decimal (BCD) conversion and flexible memory register store/load operations (`Fx55` / `Fx65`).

---

## Tested ROMs

This emulator has been verified and tested against the following ROMs:

- **`pong.rom`** — Fully functional. Frame-synchronized rendering and display decay prevent ball and paddle flickering.
- **`tetris.rom`** — Fully functional. Proper register increment handling (`Fx55`/`Fx65`) ensures piece collision detection and game termination trigger correctly at the top boundary.

---

## Project Structure

```text
.
├── chip8.hpp        # Core CPU declarations, memory maps, registers, and quirks
├── chip8.cpp        # Opcode fetch-decode-execute loop & CPU lifecycle logic
├── graphics.hpp     # Raylib display manager & flicker-suppression rendering engine
├── main.cpp         # Main application loop, frame timing (60Hz), and keymapping
└── README.md
