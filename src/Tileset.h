#pragma once
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"

class Tileset{
public:
	SDL_Surface* atlas;
	int spriteWidth;
	int spriteHeight;
	Tileset(const char* source, int rows, int columns);
	~Tileset();
	SDL_Rect get_tile(int row, int col);
};