#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Tileset.h"
#include "Collision.h"
#include "Structs.h"
#include "Enemy.h"

const int ROOM_TILE_SIZE = 64;

struct Door{
	Vec2 position; //worldspace position
	Vec2 spawnPoint; //where the player spawns when entering from another room
	CardinalBool facing; //which way the door faces in the room (e.g., Western door faces East)
	bool isVictory;
};


class Room{
private:
public:
	std::vector<Collider> walls;
	std::vector<Door> doors;
	std::vector<Enemy> enemies;
	Vec2 playerSpawn = {70, 70};
	Vec2 maxCoords = {ROOM_TILE_SIZE, ROOM_TILE_SIZE};
	Tilemap tilemap;
	
	Room(const char* source, bool isVictory);
	Room();
};