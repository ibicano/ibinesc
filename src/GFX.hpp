/*
 * GFX.hpp
 *
 *  Created on: 11 de jul. de 2015
 *      Author: ibon
 */

#ifndef GFX_HPP_
#define GFX_HPP_

#include <SDL2/SDL.h>
#include <SDL2/begin_code.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

struct RGB;

// Clase base abstracta para el motor gráfico
class GFX {
public:
	static const int NATIVE_WIDTH = 256;
	static const int NATIVE_HEIGHT = 240;

	GFX();
	virtual ~GFX();

	virtual void drawPixel(int x, int y, RGB color) = 0;
	virtual void fill(RGB color) = 0;
	virtual void clear() = 0;
	virtual void update() = 0;

	// Funciones para cambiar la resolución de la ventana
	virtual void setViewSize(int w = NATIVE_WIDTH, int h = NATIVE_HEIGHT) = 0;
	virtual void setZoomView(int zoom = 1) = 0;

	virtual void setFullscreen(bool f) = 0;

protected:
	int viewWidth = NATIVE_WIDTH;
	int viewHeight = NATIVE_HEIGHT;
};


// Clase que implementa un motor gráfico usando SDL2
class GFX_Sdl2 : public GFX {
public:
	GFX_Sdl2();
	virtual ~GFX_Sdl2();

	virtual void drawPixel(int x, int y, RGB color);
	virtual void fill(RGB color);
	virtual void clear();
	virtual void update();

	virtual void setViewSize(int w = NATIVE_WIDTH, int h = NATIVE_HEIGHT);
	virtual void setZoomView(int zoom);

	virtual void setFullscreen(bool f);

private:
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_PixelFormat* pixelFormat;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	unsigned int* pixels;
};

#endif /* GFX_HPP_ */
