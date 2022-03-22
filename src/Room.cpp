#include "Room.h"

Room::Room(){	
}

Room::Room(const char* source){
	std::ifstream srcFile(source, std::ifstream::in);
	/*--NOTE: THIS METHOD OF IMPLEMENTING TILEMAPS IS TEMPORARY--*/
	tilemap.indices.push_back({1,0}); //wall
	tilemap.indices.push_back({1,1}); //floor
	tilemap.indices.push_back({1,2}); //door
	char c;
	Collider w({0, 0}, {0, ROOM_TILE_SIZE}, true, false, false);
	Door d;
	int mapX = 0, mapY = 0;
	
	for (int x = 0; x < TILEMAP_SIZE; x++){
		for (int y = 0; y < TILEMAP_SIZE; y++){
			tilemap.map[x][y] = 0;
		}
	}
	
	//ISSUE: tile indices are hard coded. this makes it hard to change the tileset
	while(srcFile.get(c)){
		switch(c){
			case '#':
				//wall
				w.dim.x += ROOM_TILE_SIZE;
				tilemap.map[mapX][mapY] = 1;
				mapX++;
				break;
			case '\n':
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
				if (w.dim.x > 0){
					//append wall
					walls.push_back(w);
					//reset collider
					w.pos.x += w.dim.x;
					w.dim.x = 0;
				}
				w.dim.x = ROOM_TILE_SIZE;
				//append door
				walls.push_back(w);
				//create door struct (for use when moving between rooms)
				d.position = w.pos;
				if (c == 'N'){
					d.spawnPoint = {w.pos.x + 16, w.pos.y + ROOM_TILE_SIZE + 8};
					d.facing = _SOUTH;
				}
				else if (c == 'E'){
					d.spawnPoint = {w.pos.x - ROOM_TILE_SIZE + 16, w.pos.y + 8};
					d.facing = _WEST;
				}
				else if (c == 'S'){
					d.spawnPoint = {w.pos.x + 16, w.pos.y - ROOM_TILE_SIZE + 8};
					d.facing = _NORTH;
				}
				else if (c == 'W'){
					d.spawnPoint = {w.pos.x + ROOM_TILE_SIZE + 16, w.pos.y + 8};
					d.facing = _EAST;
				}
				doors.push_back(d);
				//reset collider
				w.pos.x += w.dim.x;
				w.dim.x = 0;
				tilemap.map[mapX][mapY] = 3;
				mapX++;
				break;
			case '-':
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