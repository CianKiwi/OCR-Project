#include <fstream>
#include <vector>
#include "Collision.h"
#include <iostream>
class Room{
private:
public:
	std::vector<Collider> walls;
	std::vector<Collider> doors;
	
	Room(const char* source, Vec2 tileDim);
};