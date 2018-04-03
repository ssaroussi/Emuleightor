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
#include "SDL2/SDL.h"
#include <iostream>

#define WIN_WIDTH  (1024)
#define WIN_HEIGHT (512)
#define APP_NAME   ("Emuleightor")


class Graphics {

public:
    // Keypad keymap
    byte keymap[16] = {
            SDLK_x,
            SDLK_1,
            SDLK_2,
            SDLK_3,
            SDLK_q,
            SDLK_w,
            SDLK_e,
            SDLK_a,
            SDLK_s,
            SDLK_d,
            SDLK_z,
            SDLK_c,
            SDLK_4,
            SDLK_r,
            SDLK_f,
            SDLK_v,
    };

    Graphics();

    SDL_Renderer *get_renderer() const;

    SDL_Texture *get_sdlTexture() const;

private:
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    SDL_Texture *_sdlTexture;

};
