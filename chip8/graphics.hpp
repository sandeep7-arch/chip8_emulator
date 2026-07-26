#pragma once
#include <raylib.h>
#include <cstdint>
#include <vector>

constexpr int CHIP8_WIDTH = 64;
constexpr int CHIP8_HEIGHT = 32;
constexpr int DISPLAY_SCALE = 20;

class DisplayManager {
public:
    DisplayManager() {
        // Pixel decay buffer to prevent flicker (0 = off, 255 = fully lit)
        decay_buffer.resize(CHIP8_WIDTH * CHIP8_HEIGHT, 0);
    }

    void init() {
        // Enable V-Sync for smooth tearing-free rendering
        SetConfigFlags(FLAG_VSYNC_HINT); 
        InitWindow(CHIP8_WIDTH * DISPLAY_SCALE, CHIP8_HEIGHT * DISPLAY_SCALE, "CHIP-8 Emulator");
        SetTargetFPS(60);
    }

    void render(const uint8_t* gfx, bool enable_flicker_reduction = true) {
        BeginDrawing();
        ClearBackground(BLACK);

        for (int y = 0; y < CHIP8_HEIGHT; ++y) {
            for (int x = 0; x < CHIP8_WIDTH; ++x) {
                int index = y * CHIP8_WIDTH + x;

                if (enable_flicker_reduction) {
                    // Update decay buffer: active pixels light up instantly; inactive pixels fade gradually
                    if (gfx[index]) {
                        decay_buffer[index] = 255;
                    } else if (decay_buffer[index] > 0) {
                        // Decay speed (adjust subtract value if you want more or less trail effect)
                        decay_buffer[index] = (decay_buffer[index] > 40) ? decay_buffer[index] - 40 : 0;
                    }

                    if (decay_buffer[index] > 0) {
                        Color pixel_color = { 255, 255, 255, decay_buffer[index] };
                        DrawRectangle(x * DISPLAY_SCALE, y * DISPLAY_SCALE, DISPLAY_SCALE, DISPLAY_SCALE, pixel_color);
                    }
                } else {
                    // Standard sharp rendering without decay
                    if (gfx[index]) {
                        DrawRectangle(x * DISPLAY_SCALE, y * DISPLAY_SCALE, DISPLAY_SCALE, DISPLAY_SCALE, WHITE);
                    }
                }
            }
        }

        EndDrawing();
    }

    void close() {
        CloseWindow();
    }

private:
    std::vector<uint8_t> decay_buffer;
};
