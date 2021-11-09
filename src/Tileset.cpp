#include "Tileset.h"

Tileset::Tileset(SDL_Renderer* rend, const char* source, int i_cols, int i_rows, SDL_Point i_tileSize)
: cols{i_cols}, rows{i_rows}, tileSize{i_tileSize}{
	SDL_Surface* image = IMG_Load(source);
	if(!image){
		std::cout << "ERROR: image file not present for tileset.\n" << source << std::endl;
		return;
	}
	atlas = SDL_CreateTextureFromSurface(rend, image);
}

SDL_Rect Tileset::get_tile(int col, int row){
	SDL_Rect rect = {0, 0, tileSize.x, tileSize.y};
	if(col > cols || row > rows || col < 0 || row < 0){
		return rect;
	}
	else{
		rect.x = col*tileSize.x;
		rect.y = col*tileSize.y;
		return rect;
	}
}