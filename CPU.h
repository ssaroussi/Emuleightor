/**
 * This file is part of Emuleightor.
 *
 * Emuleightor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Emuleightor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Emuleightor.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "type.h"
#include <algorithm>
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <cstring>


#define MEM_SIZE (4092)
#define REGS_NUM (16)

#define TEXT_SEG (0x200)

#define WIN_WIDTH  (64)
#define WIN_HEIGHT (32)

#define KEYS_NUM (16)

#define CARRY_FLAG (0xF)


class CPU {

public:
    CPU();

    void load_game(std::string &path);

    void instruction_cycle();

    bool draw_flag() const;

    void set_draw_flag(bool flag);

    byte get_gfx_pixel(word pixel_index) const;

    void set_key(bool value, byte index);

private:
    inline byte rand_byte() const;

    inline void unknown_opcode(opcode_t opcode) const;

    void handle_sprite(word x, word y, word height);

    void tick();

    word _memory[MEM_SIZE];
    std::stack<word> _stack;
    std::vector<byte> _v;
    bool _key[KEYS_NUM];
    byte _gfx[WIN_HEIGHT * WIN_WIDTH];

    word _i;
    word _pc;

    byte _delay_timer;
    byte _sound_timer;


    bool _draw_flag;

    byte chip8_font_set[80] = {
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

};
