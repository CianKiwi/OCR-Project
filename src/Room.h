#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Tileset.h"
#include "Collision.h"

//TODO: integrate Tilesets and Rooms, then make functional in the program
//map between the tile type and its index in the tileset for rendering

class Room{
private:
public:
	std::vector<Collider> walls;
	std::vector<Collider> doors;

	Room(const char* source, Vec2 tileDim);
};