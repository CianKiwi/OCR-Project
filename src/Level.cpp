#include "Level.h"

Level::Level(){
	
	//all rooms and number of variations of each type
	std::vector<std::pair<const char*, int>> LUT_availableRooms;
	
	//rooms with a door in given cardinal direction
	std::vector<int> LUT_North;
	std::vector<int> LUT_East;
	std::vector<int> LUT_South;
	std::vector<int> LUT_West;
	
	//rooms with n number of doors
	std::vector<int> LUT_Four;
	std::vector<int> LUT_Three;
	std::vector<int> LUT_Two;
	std::vector<int> LUT_One;
	
	std::vector<Unconnected> stack;
	
	std::srand(time(0));
	
	levelMap[LEVELMAP_SIZE/2][LEVELMAP_SIZE/2] = Room("NESW_1");
	levelMap[LEVELMAP_SIZE/2][1+(LEVELMAP_SIZE/2)] = Room("W_1");
	spawnRoom = {LEVELMAP_SIZE/2, LEVELMAP_SIZE/2};
}