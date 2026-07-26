#include "chip8.hpp"
#include <fstream>
#include <iostream>

Chip8::Chip8() : rng(std::random_device{}()) {
    reset();
}

void Chip8::reset() {
    I = 0;
    pc = 0x200;
    delay_timer = 0;
    sound_timer = 0;
    sp = 0;
    draw_flag = false;

    memory.fill(0);
    V.fill(0);
    stack.fill(0);
    gfx.fill(0);
    key.fill(false);
    prev_key.fill(false);

    // Load fontset into memory (0x050 - 0x09F)
    std::copy(CHIP8_FONTSET.begin(), CHIP8_FONTSET.end(), memory.begin() + 0x50);
}

bool Chip8::load_rom(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) return false;

    const std::streamsize size = file.tellg();
    if (size > (4096 - 0x200)) return false;

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(&memory[0x200]), size);
    return true;
}

void Chip8::set_key(uint8_t key_index, bool pressed) {
    if (key_index < 16) {
        prev_key[key_index] = key[key_index];
        key[key_index] = pressed;
    }
}

void Chip8::update_timers() {
    if (delay_timer > 0) --delay_timer;
    if (sound_timer > 0) --sound_timer;
}

void Chip8::emulate_cycle() {
    if (pc >= 4094) return;

    const uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];
    const uint8_t x   = (opcode & 0x0F00) >> 8;
    const uint8_t y   = (opcode & 0x00F0) >> 4;
    const uint8_t n   = opcode & 0x000F;
    const uint8_t nn  = opcode & 0x00FF;
    const uint16_t nnn = opcode & 0x0FFF;

    pc += 2; // Advance PC by default

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode) {
                case 0x00E0:
                    gfx.fill(0);
                    draw_flag = true;
                    break;
                case 0x00EE:
                    if (sp > 0) pc = stack[--sp];
                    break;
                default: break;
            }
            break;

        case 0x1000: pc = nnn; break;
        case 0x2000:
            if (sp < 16) {
                stack[sp++] = pc;
                pc = nnn;
            }
            break;

        case 0x3000: if (V[x] == nn) pc += 2; break;
        case 0x4000: if (V[x] != nn) pc += 2; break;
        case 0x5000: if (V[x] == V[y]) pc += 2; break;
        case 0x6000: V[x] = nn; break;
        case 0x7000: V[x] += nn; break;

        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0: V[x] = V[y]; break;
                case 0x1: V[x] |= V[y]; break;
                case 0x2: V[x] &= V[y]; break;
                case 0x3: V[x] ^= V[y]; break;
                case 0x4: {
                    const uint16_t sum = V[x] + V[y];
                    V[x] = sum & 0xFF;
                    V[0xF] = (sum > 0xFF) ? 1 : 0;
                    break;
                }
                case 0x5: {
                    const uint8_t carry = (V[x] >= V[y]) ? 1 : 0;
                    V[x] -= V[y];
                    V[0xF] = carry;
                    break;
                }
                case 0x6: {
                    const uint8_t target = quirk.shift ? V[x] : V[y];
                    const uint8_t flag = target & 0x01;
                    V[x] = target >> 1;
                    V[0xF] = flag;
                    break;
                }
                case 0x7: {
                    const uint8_t carry = (V[y] >= V[x]) ? 1 : 0;
                    V[x] = V[y] - V[x];
                    V[0xF] = carry;
                    break;
                }
                case 0xE: {
                    const uint8_t target = quirk.shift ? V[x] : V[y];
                    const uint8_t flag = (target & 0x80) >> 7;
                    V[x] = target << 1;
                    V[0xF] = flag;
                    break;
                }
            }
            break;

        case 0x9000: if (V[x] != V[y]) pc += 2; break;
        case 0xA000: I = nnn; break;
        case 0xB000: pc = nnn + (quirk.jump ? V[x] : V[0]); break;
        case 0xC000: V[x] = static_cast<uint8_t>(dist(rng)) & nn; break;

        case 0xD000: {
            V[0xF] = 0;
            for (int row = 0; row < n; ++row) {
                if (I + row >= 4096) break;
                const uint8_t sprite = memory[I + row];

                for (int col = 0; col < 8; ++col) {
                    if ((sprite & (0x80 >> col)) != 0) {
                        int px = V[x] + col;
                        int py = V[y] + row;

                        if (quirk.clip) {
                            if (px >= 64 || py >= 32) continue;
                        } else {
                            px %= 64;
                            py %= 32;
                        }

                        const int index = py * 64 + px;
                        if (gfx[index] == 1) V[0xF] = 1;
                        gfx[index] ^= 1;
                    }
                }
            }
            draw_flag = true;
            break;
        }

        case 0xE000:
            switch (nn) {
                case 0x9E: if (V[x] < 16 && key[V[x]]) pc += 2; break;
                case 0xA1: if (V[x] >= 16 || !key[V[x]]) pc += 2; break;
            }
            break;

        case 0xF000:
            switch (nn) {
                case 0x07: V[x] = delay_timer; break;
                case 0x0A: {
                    bool key_pressed = false;
                    for (int i = 0; i < 16; ++i) {
                        if (key[i] && !prev_key[i]) {
                            V[x] = static_cast<uint8_t>(i);
                            key_pressed = true;
                            break;
                        }
                    }
                    if (!key_pressed) pc -= 2; // Repeat opcode until key is pressed
                    break;
                }
                case 0x15: delay_timer = V[x]; break;
                case 0x18: sound_timer = V[x]; break;
                case 0x1E: I += V[x]; break;
                case 0x29: I = 0x50 + ((V[x] & 0x0F) * 5); break;
                case 0x33:
                    memory[I]     = V[x] / 100;
                    memory[I + 1] = (V[x] / 10) % 10;
                    memory[I + 2] = V[x] % 10;
                    break;
                case 0x55:
                    for (int i = 0; i <= x; ++i) memory[(I + i) & 0xFFF] = V[i];
                    if (!quirk.loadStore) I += x + 1;
                    break;
                case 0x65:
                    for (int i = 0; i <= x; ++i) V[i] = memory[(I + i) & 0xFFF];
                    if (!quirk.loadStore) I += x + 1;
                    break;
            }
            break;
    }
}
