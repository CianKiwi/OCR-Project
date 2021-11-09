#include "Room.h"

Room::Room(const char* source, Vec2 tileDim){
	std::ifstream srcFile(source, std::ifstream::in);
	
	char c;
	Collider w({0, 0}, {0, tileDim.y}, true, false, false);
	
	//ISSUE: tile indices are hard coded. this makes it hard to change the tileset
	while(srcFile.get(c)){
		switch(c){
			case '#':
				std::cout << "making wall" << std::endl;
				//wall
				w.dim.x += tileDim.x;
				break;
			case '\n':
				std::cout << "new line" << std::endl;
				//new line
				if (w.dim.x > 0){
					//append wall
					walls.push_back(w);
					//reset collider
					w.pos.x = 0;
					w.pos.y += tileDim.y;
					w.dim.x = 0;
				}
				else{
					w.pos.x = 0;
					w.pos.y += tileDim.y;
				}
				break;
			case 'N':
			case 'E':
			case 'S':
			case 'W':
				//door
				std::cout << "making door" << std::endl;
				if (w.dim.x > 0){
					//append wall
					walls.push_back(w);
					//reset collider
					w.pos.x += w.dim.x;
					w.dim.x = 0;
				}
				w.dim.x = tileDim.x;
				//append door
				doors.push_back(w);
				//reset collider
				w.pos.x += w.dim.x;
				w.dim.x = 0;
				break;
			default:
				std::cout << "default" << std::endl;
				//floor (and other characters)
				if (w.dim.x > 0){
					//append wall
					walls.push_back(w);
					//reset collider
					w.pos.x += w.dim.x + tileDim.x;
					w.dim.x = 0;
				}
				else{
					w.pos.x += tileDim.x;
				}
				break;
		}
	}
	//last collider
	if (w.dim.x > 0){
			//append wall
			walls.push_back(w);
			//reset collider
			w.pos.x += w.dim.x + tileDim.x;
			w.dim.x = 0;
		}
	else{
		w.pos.x += tileDim.x;
	}
	
	srcFile.close();
}