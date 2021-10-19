#include "Room.h"

Room::Room(const char* i_source, Tileset* i_tileset, double start_x, double start_y)
{
	tileset = i_tileset;
	
	std::ifstream roomSrc(i_source);
	std::string line;
	
	Collider wallPiece({start_x, start_y}, {0, TILE_DIM_Y});
	int doorIndex = 0;
	//keeps track of current tile in tilemap
	int tileCol = 0;
	int tileRow = 0;
	//keeps track of xy position in the room during construction
	double x = start_x;
	double y = start_y;
	while(getline(roomSrc, line)){
		//line.length() is type Uint64
		//if i is int, i get warnings (not fatal)
		for (Uint64 i = 0; i < line.length(); i++){
			if(line[i] == '#'){
				if(wallPiece.dim.x <= 0){
					wallPiece.pos.x = x;
				}
				//extend wall collider, set tilemap tile
				wallPiece.dim.x += TILE_DIM_X;
				tilemap[tileCol][tileRow] = {1, 0};
				tileCol++;
			}
			else{
				if (wallPiece.dim.x > 0){ //pushback wall and reset
					walls.push_back(wallPiece);
					x += wallPiece.dim.x;
					wallPiece.dim.x = 0;
					wallPiece.pos.x = x;
				}
				//other cases
				if(line[i] == '-'){
					//set tilemap tile
					tilemap[tileCol][tileRow] = {1, 1};
					tileCol++;		
					x += TILE_DIM_X;
				}
				else if(line[i] == 'N'
				|| line[i] == 'E'
				|| line[i] == 'S'
				|| line[i] == 'W'){
					//create door collider in array, set tilemap tile
					if(doorIndex < 4){
						Collider doorCollider({x, y}, {TILE_DIM_X, TILE_DIM_Y});
						doors[doorIndex] = doorCollider;
						doorIndex++;
						tilemap[tileCol][tileRow] = {1, 2};
					}
					else{
						tilemap[tileCol][tileRow] = {0, 0};
					}
					tileCol++;
					x += TILE_DIM_X;
				}
				else if(line[i] == '\n'){
					break; //goto the next line
				}
			}
		}
		y += TILE_DIM_Y;
		x = start_x;
		wallPiece.pos.x = x;
		wallPiece.pos.y = y;
		tileCol = 0;
		tileRow++;
	}
	/*
	iterate over line string
	# : wallpiece width += tile width
	not # : push wallPiece onto walls vector, reset wallPiece
	
	N,E,S,W : create door collider and put into array
	*/
	
	roomSrc.close();
}