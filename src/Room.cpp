#include "Room.h"
#include "Tileset.h"

Room::Room(const char* source, Tileset* tileset){
	std::ifstream srcFile(source, std::ifstream::in);
	tilemap = Tilemap(tileset);
	/*--NOTE: THIS METHOD OF IMPLEMENTING TILEMAPS IS TEMPORARY--*/
	tilemap.indices.push_back({1,0}); //wall
	tilemap.indices.push_back({1,1}); //floor
	tilemap.indices.push_back({1,2}); //door
	char c;
	Collider w({0, 0}, {0, ROOM_TILE_SIZE}, true, false, false);
	int mapX, mapY = 0;
	
	for (int x = 0; x < TILEMAP_SIZE; x++){
		for (int y = 0; y < TILEMAP_SIZE; y++){
			tilemap.map[x][y] = 0;
		}
	}
	
	//ISSUE: tile indices are hard coded. this makes it hard to change the tileset
	while(srcFile.get(c)){
		switch(c){
			case '#':
				std::cout << "making wall" << std::endl;
				//wall
				w.dim.x += ROOM_TILE_SIZE;
				tilemap.map[mapX][mapY] = 1;
				mapX++;
				break;
			case '\n':
				std::cout << "new line" << std::endl;
				//new line
				if (w.dim.x > 0){
					//append wall
					walls.push_back(w);
					//reset collider
					w.pos.x = 0;
					w.pos.y += ROOM_TILE_SIZE;
					w.dim.x = 0;
				}
				else{
					w.pos.x = 0;
					w.pos.y += ROOM_TILE_SIZE;
				}
				mapX = 0;
				mapY++;
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
				w.dim.x = ROOM_TILE_SIZE;
				//append door
				doors.push_back(w);
				//reset collider
				w.pos.x += w.dim.x;
				w.dim.x = 0;
				tilemap.map[mapX][mapY] = 3;
				mapX++;
				break;
			case '-':
				std::cout << "making floor" << std::endl;
				//floor (and other characters)
				if (w.dim.x > 0){
					//append wall
					walls.push_back(w);
					//reset collider
					w.pos.x += w.dim.x + ROOM_TILE_SIZE;
					w.dim.x = 0;
				}
				else{
					w.pos.x += ROOM_TILE_SIZE;
				}
				tilemap.map[mapX][mapY] = 2;
				mapX++;
				break;
			default:
				std::cout << "default" << std::endl;
				//floor (and other characters)
				if (w.dim.x > 0){
					//append wall
					walls.push_back(w);
					//reset collider
					w.pos.x += w.dim.x + ROOM_TILE_SIZE;
					w.dim.x = 0;
				}
				else{
					w.pos.x += ROOM_TILE_SIZE;
				}
				tilemap.map[mapX][mapY] = 0;
				mapX++;
				break;
		}
	}
	//last collider
	if (w.dim.x > 0){
			//append wall
			walls.push_back(w);
			//reset collider
			w.pos.x += w.dim.x + ROOM_TILE_SIZE;
			w.dim.x = 0;
		}
	else{
		w.pos.x += ROOM_TILE_SIZE;
	}

	srcFile.close();
}