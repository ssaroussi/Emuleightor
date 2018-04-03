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

#include "Graphics.h"

Graphics::Graphics()
        : _window(nullptr) {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << std::endl;
        exit(1);
    }

    // Create window
    _window = SDL_CreateWindow(
            APP_NAME,
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if (!_window) {
        std::cout << "Window could not be created! SDL_Error:" << SDL_GetError() << std::endl;
        exit(2);
    }

    // Create renderer
    _renderer = SDL_CreateRenderer(_window, -1, 0);
    SDL_RenderSetLogicalSize(_renderer, WIN_WIDTH, WIN_HEIGHT);

    // Create texture that stores frame buffer
    _sdlTexture = SDL_CreateTexture(_renderer,
                                    SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    64, 32);
}

/**
 * @return The renderer of the current window.
 */
SDL_Renderer *Graphics::get_renderer() const {
    return _renderer;
}

/**
 * @return The SDL texture of the current window.
 */
SDL_Texture *Graphics::get_sdlTexture() const {
    return _sdlTexture;
}
