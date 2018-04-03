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

#include "CPU.h"


CPU::CPU()
        : _i(0), _pc(TEXT_SEG), _v(REGS_NUM, 0),
          _delay_timer(0), _sound_timer(0), _draw_flag(false),
          _stack() {

    /* Initiate the keypad, memory and graphics buffer with zero's. */
    std::fill_n(_key, KEYS_NUM, 0);
    std::memset(_memory, 0, MEM_SIZE);
    std::memset(_gfx, 0, WIN_WIDTH * WIN_HEIGHT);

    /* Initiate the font set. */
    for (int i = 0; i < 80; i++)
        _memory[i] = chip8_font_set[i];

    /* Set the random seed */
    std::srand(std::time(nullptr));
}

/**
 * Load a rom into the emulator's memory.
 *
 * @param path The path of the rom.
 */
void CPU::load_game(std::string &path) {
    std::ifstream game_ifs(path, std::ios::binary);

    if (game_ifs.fail()) {
        std::cout << "Can not open: " << path << std::endl;
        exit(1);
    }

    std::vector<byte> buffer(std::istreambuf_iterator<char>(game_ifs),
                             (std::istreambuf_iterator<char>()));

    if ((MEM_SIZE - TEXT_SEG) < buffer.size()) {
        std::cout << "This rom is too heavy for this emulator." << std::endl;
        exit(2);
    }

    std::copy(buffer.begin(), buffer.end(), _memory + TEXT_SEG);
}

/**
 * The main cycle:
 *      - Fetch operation code
 *      - Decode the operation code
 *      - Execute the operation code
 */
void CPU::instruction_cycle() {

    // Fetch Operation Code.
    opcode_t opcode = _memory[_pc] << 8 | _memory[_pc + 1];

    word x = (opcode >> 8) & 0x000F // Lower 4 bits of the high byte
    , y = (opcode >> 4) & 0x000F // Upper 4 bits of the low byte
    , n = opcode & 0x000F // Lowest 4 bits
    , kk = opcode & 0x00FF // Lowest 8 bits
    , nnn = opcode & 0x0FFF; // Lowest 12 bits


#ifndef NDEBUG
    std::cout << "Fetching: " << std::hex << opcode << std::endl;
#endif


    switch (opcode & 0xF000) {
        case 0x0000:
            switch (kk) {
                case 0xE0: // CLS
                    memset(_gfx, 0, WIN_WIDTH * WIN_HEIGHT);
                    _draw_flag = true;
                    _pc += 2;
                    break;
                case 0xEE: // RET
                    _pc = _stack.top();
                    _stack.pop();
                    _pc += 2;
                    break;
                default:
                    unknown_opcode(opcode);
                    break;
            }
            break;
        case 0x1000: // JP addr: jump to nnn
            _pc = nnn;
            break;
        case 0x2000: // Call addr: call subroutine at nnn
            _stack.push(_pc);
            _pc = nnn;
            break;
        case 0x3000: // SE: skip next instruction if Vx = kk
            _pc += _v[x] == kk ? 4 : 2;
            break;
        case 0x4000: // SNE: skip next instruction if Vx != kk
            _pc += _v[x] != kk ? 4 : 2;
            break;
        case 0x5000: // SE: skip next instruction if Vx = Vy
            _pc += _v[x] == _v[y] ? 4 : 2;
            break;
        case 0x6000: // LD: set Vx = kk
            _v[x] = kk;
            _pc += 2;
            break;
        case 0x7000: // ADD:  set Vx = Vx + kk
            _v[x] += kk;
            _pc += 2;
            break;
        case 0x8000:
            switch (n) {
                case 0: // LD: set Vx = Vy
                    _v[x] = _v[y];
                    break;
                case 1: // OR: set Vx = Vx OR Vy
                    _v[x] |= _v[y];
                    break;
                case 2: // AND: set Vx = Vx AND Vy
                    _v[x] &= _v[y];
                    break;
                case 3: // XOR: set Vx = Vx XOR Vy
                    _v[x] ^= _v[y];
                    break;
                case 4: // ADD: set Vx = Vx + Vy, set VF = carry
                    _v[x] += _v[y];
                    _v[CARRY_FLAG] = _v[y] > (0xFF - _v[x]);
                    break;
                case 5: // SUB: set Vx = Vx - Vy, set VF = NOT borrow
                    _v[CARRY_FLAG] = _v[x] > _v[y];
                    _v[x] -= _v[y];
                    break;
                case 6: // SHR: set Vx = Vx SHR 1
                    _v[CARRY_FLAG] = _v[x] & 0x1;
                    _v[x] >>= 1;
                    break;
                case 7: // SUBN: set Vx = Vy - Vx, set VF = NOT borrow
                    _v[CARRY_FLAG] = _v[y] >= _v[x];
                    _v[x] = _v[y] - _v[x];
                    break;
                case 0xE: // SHL: set Vx = Vx SHL 1
                    _v[CARRY_FLAG] = (_v[x] >> 7);
                    _v[x] <<= 1;
                    break;
                default:
                    unknown_opcode(opcode);
                    break;
            }
            _pc += 2;
            break;
        case 0x9000:
            switch (n) {
                case 0:
                    _pc += (_v[x] != _v[y]) ? 4 : 2;
                    break;
                default:
                    unknown_opcode(opcode);
                    break;
            }
            break;
        case 0xA000: // LD: set I = nnn
            _i = nnn;
            _pc += 2;
            break;
        case 0xB000: // JP: jump to location nnn + V0
            _pc = nnn + _v[0];
            break;
        case 0xC000: // RND: set Vx = random byte AND kk
            _v[x] = rand_byte() & kk;
            _pc += 2;
            break;
        case 0xD000: // DRW: display n-byte sprite starting at memory location I at, set VF = collision
            handle_sprite(_v[x], _v[y], n);
            _pc += 2;
            break;
        case 0xE000: // Key-Pad handler
            switch (kk) {
                case 0x9E: // SKP: skip next instruction if key num Vx pressed
                    _pc += _key[_v[x]] ? 4 : 2;
                    break;
                case 0xA1: // SKNP: skip next instruction if key num Vx is not pressed
                    _pc += _key[_v[x]] ? 2 : 4;
                    break;
                default:
                    unknown_opcode(opcode);
                    break;
            }
            break;
        case 0xF000:
            switch (kk) {
                case 0x07: // LD:  set Vx = delay timer value
                    _v[x] = _delay_timer;
                    _pc += 2;
                    break;
                case 0x0A:  // LD: wait for a key press, store the value of the key in Vx
                    while (true) {
                        for (byte i = 0; i < KEYS_NUM; i++) {
                            if (!_key[i]) continue;

                            _v[x] = i;
                            goto got_key_press;
                        }
                    }
                got_key_press:
                    _pc += 2;
                    break;
                case 0x15: // LD: set delay timer = Vx
                    _delay_timer = _v[x];
                    _pc += 2;
                    break;
                case 0x18: // LD: set sound timer = Vx
                    _sound_timer = _v[x];
                    _pc += 2;
                    break;
                case 0x1E: // ADD: set I = I + Vx
                    _v[0xF] = (((int) _i) + _v[x]) > 0xfff;
                    _i += _v[x];
                    _pc += 2;
                    break;
                case 0x29: // LD: set I = location of sprite for digit Vx
                    _i = 5 * _v[x];
                    _pc += 2;
                    break;
                case 0x33: // LD: store BCD representation of Vx in memory locations I, I+1, and I+2
                    _memory[_i + 2] = (_v[x] % 100) % 10;
                    _memory[_i + 1] = (_v[x] / 10) % 10;
                    _memory[_i] = _v[x] / 100;
                    _pc += 2;
                    break;
                case 0x55: // LD: store registers V0 through Vx in memory starting at location I
                    for (byte i = 0; i <= x; i++)
                        _memory[_i + i] = _v[i];

                    _i += x + 1;
                    _pc += 2;
                    break;
                case 0x65: // LD: read registers V0 through Vx from memory starting at location I
                    for (byte i = 0; i <= x; i++)
                        _v[i] = _memory[_i + i];

                    _i += x + 1;
                    _pc += 2;
                    break;
                default:
                    unknown_opcode(opcode);
                    break;
            }
            break;
        default:
            unknown_opcode(opcode);
            break;
    }

    this->tick();

}


/**
 * Radnomizes a byte.
 *
 * @return A random byte.
 */
byte CPU::rand_byte() const {
    return (byte) (std::rand() % 0x100);
}

void CPU::handle_sprite(word x, word y, word height) {
    word pixel = 0;
    _v[0xF] = 0;

    for (word y_line = 0; y_line < height; ++y_line) {
        pixel = _memory[_i + y_line];

        for (word x_line = 0; x_line < 8; x_line++) {
            if (!(pixel & (0x80 >> x_line))) continue;

            _v[0xF] = _gfx[x + x_line + ((y + y_line) * WIN_WIDTH)]; // Graphical collision.
            _gfx[x + x_line + ((y + y_line) * WIN_WIDTH)] ^= 1;
        }
    }

    _draw_flag = true;
}

/**
 * Called when unknown operation code is decoded.
 *
 * @param opcode The unknown operation code.
 */
void CPU::unknown_opcode(opcode_t opcode) const {
#ifndef NDEBUG
    std::cout << "Unknown opcode: " << std::hex << opcode << std::endl;
#endif

    std::cout << "Core panic. dieing." << std::endl;
    exit(2);
}

/**
 * Manage the cpu timers.
 */
void CPU::tick() {
    if (_delay_timer > 0)
        --_delay_timer;

    if (_sound_timer > 0) {
        if (_sound_timer == 1)
            std::cout << '\a';
        --_sound_timer;
    }
}

/**
 * Returns rather the graphics should be redrawn or not.
 *
 * @return The draw flag.
 */
bool CPU::draw_flag() const {
    return _draw_flag;
}

/**
 * Set the draw flag of the CPU.
 *
 * @param flag The value of the flag (true || false).
 */
void CPU::set_draw_flag(const bool flag) {
    _draw_flag = flag;
}

/**
 * Returns the value of the pixel_index's pixel.
 *
 * @param pixel_index The index of the pixel.
 * @return The value of the required pixel.
 */
byte CPU::get_gfx_pixel(const word pixel_index) const {
    if ((unsigned) (pixel_index) <= (sizeof(_gfx)))
        return _gfx[pixel_index];

    std::cout << "Invalid pixel index." << std::endl;
    exit(1);
}

/**
 * Sets certain value to certain key in the keypad.
 *
 * @param value The new value of the key.
 * @param index The index of the key in the keypad.
 */
void CPU::set_key(const bool value, const byte index) {
    _key[index] = value;
}
