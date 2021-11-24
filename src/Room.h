#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Tileset.h"
#include "Collision.h"

class Room{
private:
public:
	std::vector<Collider> walls;
	std::vector<Collider> doors;
	Tilemap tilemap;
	
	Room(const char* source, Vec2 tileDim, Tileset* tileset);
};