#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include "Structs.h"
#include "Room.h"


const int LEVELMAP_SIZE = 16;

struct Unconnected{
	Index2 index;
	CardinalBool doors;
};

class Level{
public:
	std::vector<Room> rooms;
	int levelMap[LEVELMAP_SIZE][LEVELMAP_SIZE];
	Index2 spawnRoom;
	
	Level();
	Room* get_room(int r, int c);
};