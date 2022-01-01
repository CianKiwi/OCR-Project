#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Tileset.h"
#include "Collision.h"
#include "Structs.h"

const int ROOM_TILE_SIZE = 64;

struct Door{
	Vec2 position; //worldspace position
	Vec2 spawnPoint; //where the player spawns when entering from another room
	CardinalBool facing; //which way the door faces in the room (Western door faces East)
};


class Room{
private:
public:
	std::vector<Collider> walls;
	std::vector<Door> doors;
	Tilemap tilemap;
	
	Room(const char* source);
	Room();
};