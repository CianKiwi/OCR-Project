#pragma once
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Room.h"
#include "Structs.h"

const int LEVELMAP_SIZE = 16;

struct Unconnected{
	Index2 index;
	CardinalBool doors;
};

class Level{
public:
	Room levelMap[LEVELMAP_SIZE][LEVELMAP_SIZE];
	Index2 spawnRoom;
	
	Level();
};