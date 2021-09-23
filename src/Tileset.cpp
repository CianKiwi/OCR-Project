#include "Tileset.h"

Tileset::Tileset(const char* source, int rows, int columns){
	atlas = IMG_Load(source);
	if(!atlas){
		std::cout << IMG_GetError() << std::endl;
	}
	spriteWidth = atlas->w / columns;
	spriteHeight = atlas->h / rows;
}

SDL_Rect Tileset::get_tile(int row, int col){
	SDL_Rect tile = {col * spriteWidth, row * spriteHeight, spriteWidth, spriteHeight};
	return tile;
}

Tileset::~Tileset(){
	SDL_FreeSurface(atlas);
}