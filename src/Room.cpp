#include "Room.h"

Room::Room(){	
}

Room::Room(const char* source, bool isVictory){
	std::ifstream srcFile(source, std::ifstream::in);
	/*--NOTE: THIS METHOD OF IMPLEMENTING TILEMAPS IS ARBITRARY--*/
									  //empty is 0
	tilemap.indices.push_back({1,0}); //wall 1
	tilemap.indices.push_back({1,1}); //floor 2
	tilemap.indices.push_back({1,2}); //door 3
	tilemap.indices.push_back({1,3}); //victory door 4
	tilemap.indices.push_back({0,1}); //indented floor 5
	char c;
	Collider w({0, 0}, {0, ROOM_TILE_SIZE}, true, false, false);
	Collider eHitbox({ 0,0 }, { 48, 48 }, true, true, false);
	Enemy enemy(eHitbox, 3);
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
			case 'X':
				
				//floor (and other characters)
				if (w.dim.x > 0) {
					//append wall
					walls.push_back(w);
					//reset collider
					w.pos.x += w.dim.x + ROOM_TILE_SIZE;
					w.dim.x = 0;
				}
				else {
					w.pos.x += ROOM_TILE_SIZE;
				}
				//create an enemy
				enemy.hitbox.pos = { w.pos.x - 56, w.pos.y + 8 };
				enemies.push_back(enemy);

				tilemap.map[mapX][mapY] = 2;
				mapX++;
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
				d.isVictory = false;
				tilemap.map[mapX][mapY] = 3;
				
				doors.push_back(d);
				//reset collider
				w.pos.x += w.dim.x;
				w.dim.x = 0;
				
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
			case 'V':
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
				if(isVictory){
					d.position = w.pos;
					d.isVictory = true;
					doors.push_back(d);
					tilemap.map[mapX][mapY] = 4;
				}
				else{
					tilemap.map[mapX][mapY] = 5;
				}
				mapX++;
				break;
			case 'P':
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
				playerSpawn = { w.pos.x - 56, w.pos.y + 8 };
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