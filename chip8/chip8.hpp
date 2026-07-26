#pragma once

#include <array>
#include <cstdint>
#include <random>
#include <string>

// Default fontset stored in memory (0x050 - 0x09F)
constexpr std::array<uint8_t, 80> CHIP8_FONTSET = { 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

struct Quirks {
    bool shift = false;
    bool loadStore = false;
    bool clip = true;
    bool jump = false;
};

class Chip8 {
public:
    Chip8();
    void reset();
    bool load_rom(const std::string& filename);
    void emulate_cycle();
    void update_timers();

    // Input & Output Accessors
    void set_key(uint8_t key_index, bool pressed);
    const std::array<uint8_t, 64 * 32>& get_gfx() const { return gfx; }
    bool needs_redraw() const { return draw_flag; }
    void clear_redraw_flag() { draw_flag = false; }

    Quirks quirk;

private:
    std::array<uint8_t, 4096> memory{};
    std::array<uint8_t, 16> V{};
    uint16_t I{0};
    uint16_t pc{0x200};

    std::array<uint8_t, 64 * 32> gfx{};
    std::array<uint16_t, 16> stack{};
    uint8_t sp{0};

    uint8_t delay_timer{0};
    uint8_t sound_timer{0};

    std::array<bool, 16> key{};
    std::array<bool, 16> prev_key{};

    bool draw_flag{false};

    // RNG Engine
    std::mt19937 rng;
    std::uniform_int_distribution<uint16_t> dist{0, 255};
};
