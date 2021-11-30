#pragma once
#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_Image.h"
#include "Collision.h"

const int TILEMAP_SIZE = 48;

struct tile{
    int row, col;
};

class Tileset{
public:
    SDL_Surface* atlas;
    int tileWidth, tileHeight;
    int rows, cols;
	
    SDL_Rect get_tile(tile t);
    Tileset(const char* source, int rows, int cols);
};

class Tilemap{
public:
    Tileset* set;
    std::vector<tile> indices;
    int map[TILEMAP_SIZE][TILEMAP_SIZE];

    Tilemap(Tileset* i_set);
	Tilemap();
	
	tile get_tile_indices(int x, int y);
};