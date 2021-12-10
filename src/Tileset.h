#pragma once
#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_Image.h"
#include "Collision.h"
#include "Structs.h"

const int TILEMAP_SIZE = 48;

class Tileset{
public:
    SDL_Surface* atlas;
    int tileWidth, tileHeight;
    int rows, cols;
	
    SDL_Rect get_tile(Index2 t);
    Tileset(const char* source, int rows, int cols);
};

class Tilemap{
public:
    std::vector<Index2> indices;
    int map[TILEMAP_SIZE][TILEMAP_SIZE];

	Tilemap();
	
	Index2 get_tile_indices(int x, int y);
};