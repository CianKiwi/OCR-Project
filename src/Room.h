#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "SDL.h"
#include "Collision.h"
#include "Tileset.h"

//pixel dimensions of a single tile in game
#define TILE_DIM_X 64
#define TILE_DIM_Y 64

struct tile{
	int row;
	int col;
};

class Room{
public:
	Tileset* tileset;
	tile tilemap[16][16] = {0,0};
	std::vector<Collider> walls;
	Collider doors[4];
	//Room* neighbours[4];
	
	Room(const char* i_source, Tileset* i_tileset, double start_x, double start_y);
	Render(SDL_Renderer* rend);
};