/*
 * GFX.hpp
 *
 *  Created on: 11 de jul. de 2015
 *      Author: ibon
 */

#ifndef GFX_HPP_
#define GFX_HPP_


// Clase base abstracta para el motor gráfico
class GFX {
public:
	GFX();
	virtual ~GFX();

	virtual void drawPixel(int x, int y, RGB color) = 0;
	virtual void fill(RGB color) = 0;
	virtual void clear() = 0;
	virtual void update() = 0;

protected:
	int viewWidth;
	int viewHeight;
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

private:
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_PixelFormat* pixelFormat;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	unsigned int* pixels;
};

#endif /* GFX_HPP_ */
