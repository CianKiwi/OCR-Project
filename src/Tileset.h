#include <iostream>
#include "SDL.h"
#include "SDL_Image.h"

class Tileset{
public:
	SDL_Texture* atlas;
	int cols, rows;
	SDL_Point tileSize;
	Tileset(SDL_Renderer* rend, const char* source, int i_cols, int i_rows, SDL_Point i_tileSize);
	SDL_Rect get_tile(int col, int row);
};