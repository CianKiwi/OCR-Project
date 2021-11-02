#include <fstream>
#include <vector>
#include "Collision.h"

class Room{
private:
public:
	std::vector<Collider> walls;
	std::vector<Collider> doors;
	
	Room(const char* source);
};