#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <random>
#include "Structs.h"
#include "Room.h"
#include "SDL.h"

const int LEVELMAP_SIZE = 16;

class Level{
public:
	std::vector<Room> rooms;
	int levelMap[LEVELMAP_SIZE][LEVELMAP_SIZE];
	SDL_Surface* minimap;
	Index2 spawnRoom;
	Level();
	void generate_level();
	Room* get_room(int r, int c);
};