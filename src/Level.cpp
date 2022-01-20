#include "Level.h"

Level::Level(){
	
	for (int x = 0; x < LEVELMAP_SIZE; x++){
		for (int y = 0; y < LEVELMAP_SIZE; y++){
			levelMap[x][y] = -1;
		}
	}
	
	//Acquire all available rooms that can be used in procedural generation
	std::vector<std::string> combos = {"N", "E", "S", "W", "NE", "NS", "NW", "ES", "EW", "SW", "NES", "NEW", "NSW", "ESW", "NESW"};
	//all rooms and number of variations of each type
	std::vector<std::pair<std::string, int>> LUT_availableRooms;
	
	//integer + 48 is equivalent 1-digit character
	for (int x = 0; x < combos.size(); x++){
		std::string path = "rooms/" + combos[x] + "_";
		int i = 1;
		bool completed = false;
		std::fstream f(path + (char)(i+48) + ".txt");
		if (f){
			std::cout << (path + (char)(i+48) + ".txt") << std::endl;
			LUT_availableRooms.push_back({combos[x], 1});
			i++;
		}
		else{
			completed = true;
		}
		f.close();
		while(!completed){
			f.open(path + (char)(i+48) + ".txt");
			if (f.is_open()){
				std::cout << (path + (char)(i+48) + ".txt") << std::endl;
				LUT_availableRooms[x].second++;
				i++;
			}
			else{
				completed = true;
			}
			f.close();
		}
		f.close();
	}
	
	//print out available rooms
	for (std::pair<std::string, int> p : LUT_availableRooms){
		std::cout << p.first << "|" << p.second << std::endl;
	}
	
	std::vector<Unconnected> stack;
	
	//TEST RANDOM NUMBERS

	int doorWeight1 = 90;
	int doorWeight2 = 7;
	int doorWeight3 = 3;
	
	doorWeight2 += doorWeight1;
	doorWeight3 += doorWeight2;

	std::string str = "Pasghetti Boglanase";
	std::seed_seq seed(str.begin(), str.end());
	std::default_random_engine gen(seed);
	std::uniform_int_distribution<int> distro(0, doorWeight3);
	
	for(int x = 0; x < 10; x++){
		std::cout << distro(gen) << std::endl;
	}

	//test code
	///*
	Room r = Room("rooms/NESW_1.txt");
	Room rN = Room("rooms/N_1.txt");
	Room rE = Room("rooms/E_1.txt");
	Room rS = Room("rooms/S_1.txt");
	Room rW = Room("rooms/W_1.txt");
	rooms.push_back(r);
	rooms.push_back(rN);
	rooms.push_back(rE);
	rooms.push_back(rS);
	rooms.push_back(rW);
	levelMap[LEVELMAP_SIZE/2][LEVELMAP_SIZE/2] = 0;
	levelMap[LEVELMAP_SIZE/2][LEVELMAP_SIZE/2 +1] = 1;
	levelMap[LEVELMAP_SIZE/2 -1][LEVELMAP_SIZE/2] = 2;
	levelMap[LEVELMAP_SIZE/2][LEVELMAP_SIZE/2 -1] = 3;
	levelMap[LEVELMAP_SIZE/2 +1][LEVELMAP_SIZE/2] = 4;
	//*/
}

Room* Level::get_room(int r, int c){
	if (r < LEVELMAP_SIZE && c < LEVELMAP_SIZE && r > 0 && c > 0){
		return &(rooms[levelMap[r][c]]);
	}
	else{
		std::cout << "ERROR: row and column index out of range" << std::endl;
		return &(rooms[0]); //replace this with actual thrown error
	}
}