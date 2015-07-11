/*
 * GFX.cpp
 *
 *  Created on: 11 de jul. de 2015
 *      Author: ibon
 */

#include "GFX.hpp"
#include <SDL2/SDL.h>

GFX::GFX() {
	viewWidth = 512;
	viewHeight = 480;
}

GFX::~GFX() {
}


/*
 * Clase GFX_Sdl2
 */
GFX_Sdl2::GFX_Sdl2() : GFX() {
    // Objeto SDL de la ventana
    window = SDL_CreateWindow("Ventana", 0, 0, viewWidth, viewHeight, 0);

    // Crea una superficie para inicializar la textura
    surface = SDL_CreateRGBSurface(0, 256, 240, 32, 0, 0, 0, 0);

    // Formato del pixel tomado de la superficie inicializada automáticamente
    pixelFormat = surface->format;

    // Renderizador
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Textura que almacenará la información de los pixels
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Información de los pixeles-> Almacena los pixeles en un array líneal en el que cada
    // posición es un píxel representado por un entero de 32 bits en formato ARGB
    pixels = new unsigned int[61440];

    // Actualiza la textura con el array de pixeles-> El último parámetro es el número
    // de bytes que tiene una lñínea hortizontal (256*4=1024)
    SDL_UpdateTexture(texture, NULL, pixels, 1024);

    // Copia la textura al renderizador
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Renderiza la pantalla
    SDL_RenderPresent(renderer);
}//GFX_Sdl2()


GFX_Sdl2::~GFX_Sdl2() {
	delete []pixels;
}//~GFX_Sdl2()


void GFX_Sdl2::drawPixel(int x, int y, RGB color) {
	// Calcula la posición x,y del pixel en el array lineal de pixeles
	int p = (y << 8) | x;

	unsigned int v = 0xFF000000 | color[0] << 16 | color[1] << 8 | color[2];

	// Asigna el valos del pixel como un entero de 32 bits con formato ARGB
	pixels[p] = v;
}//drawPixel()


void GFX_Sdl2::fill(RGB color) {
    unsigned int v = 0xFF000000 | color[0] << 16 | color[1] << 8 | color[2];
    for (int p = 0; p < 61440; p++)
    	pixels[p] = v;
}//fill()


void GFX_Sdl2::clear() {
	SDL_RenderClear(renderer);
}//clear()


void GFX_Sdl2::update() {
    SDL_UpdateTexture(texture, NULL, pixels, 1024);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}//update()
