#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Tileset.h"
#include "Collision.h"

const int ROOM_TILE_SIZE = 64;


class Room{
private:
public:
	std::vector<Collider> walls;
	Tilemap tilemap;
	
	Room(const char* source);
	Room();
};