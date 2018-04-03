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

#include <chrono>
#include <thread>

#include "Graphics.h"
#include "CPU.h"


#define DELAY (2000)

using namespace std;


int main(int argc, char **argv) {

    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <ROM file>" << endl;
        return -1;
    }

    CPU cpu;
    Graphics graphics;


    // Load the specified rom.
    load:
    std::string name(argv[1]);
    cpu.load_game(name);

    uint32_t pixels[2048];

    // The main loop.
    while (true) {
        cpu.instruction_cycle();

        // Process SDL events
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) exit(0);

            // Process key-down events
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    exit(0);

                if (e.key.keysym.sym == SDLK_F1)
                    goto load;

                for (byte i = 0; i < 16; ++i)
                    if (e.key.keysym.sym == graphics.keymap[i])
                        cpu.set_key(true, i);
            }

            // Process keyup events
            if (e.type == SDL_KEYUP)
                for (byte i = 0; i < 16; ++i)
                    if (e.key.keysym.sym == graphics.keymap[i])
                        cpu.set_key(false, i);
        }

        // If draw occurred, redraw SDL screen
        if (cpu.draw_flag()) {
            cpu.set_draw_flag(false);

            // Store pixels in temporary buffer
            for (word i = 0; i < 2048; ++i) {
                byte pixel = cpu.get_gfx_pixel(i);
                pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
            }

            // Update SDL texture
            SDL_UpdateTexture(graphics.get_sdlTexture(), NULL, pixels, 64 * sizeof(Uint32));

            // Clear screen and render
            SDL_RenderClear(graphics.get_renderer());
            SDL_RenderCopy(graphics.get_renderer(), graphics.get_sdlTexture(), NULL, NULL);
            SDL_RenderPresent(graphics.get_renderer());
        }

        std::this_thread::sleep_for(std::chrono::microseconds(DELAY));

    }
}
