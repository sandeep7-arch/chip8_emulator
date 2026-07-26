#include "chip8.hpp"
#include "graphics.hpp"
#include <raylib.h>
#include <iostream>

void handle_input(Chip8& emulator) {
    static const int keymap[16] = {
        KEY_X, KEY_ONE, KEY_TWO, KEY_THREE,
        KEY_Q, KEY_W,   KEY_E,   KEY_A,
        KEY_S, KEY_D,   KEY_Z,   KEY_C,
        KEY_FOUR, KEY_R, KEY_F, KEY_V
    };

    for (int i = 0; i < 16; ++i) {
        emulator.set_key(i, IsKeyDown(keymap[i]));
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <rom>\n";
        return 1;
    }

    Chip8 emulator;
    if (!emulator.load_rom(argv[1])) {
        std::cerr << "Failed to load ROM: " << argv[1] << '\n';
        return 1;
    }

    // Initialize display manager
    DisplayManager display;
    display.init();

    // ~9 cycles per 60Hz frame gives an accurate ~540Hz CPU clock
    constexpr int CYCLES_PER_FRAME = 9;

    while (!WindowShouldClose()) {
        handle_input(emulator);

        // 1. Run CPU instructions for this frame
        for (int i = 0; i < CYCLES_PER_FRAME; ++i) {
            emulator.emulate_cycle();
        }

        // 2. Decrement delay & sound timers at 60Hz
        emulator.update_timers();

        // 3. Render frame directly to Raylib window (prevents mid-loop flicker)
        display.render(emulator.get_gfx().data(), true); 
    }

    display.close();
    return 0;
}
