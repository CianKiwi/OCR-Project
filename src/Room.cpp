#include "Room.h"

Room::Room(const char* source){
	std::ifstream srcFile(source, std::ifstream::in);
	
	char c;
	Vec2 tileDim = {64, 64};
	Collider w({0, 0}, {0, tileDim.y}, false, false);
	
	while(srcFile.get(c)){
		/*do stuff*/
		switch(c){
			case '#':
				w.dim.x += tileDim.x;
				break;
			case '\n':
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
			case ('N' || 'E' || 'S' || 'W'):
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