#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_Image.h"
#include "Collision.h"

struct tile{
    int row, col;
};

class Tileset{
public:
    SDL_Surface* atlas;
    int tileWidth, tileHeight;
    int rows, cols;
    SDL_Rect get_tile(tile t);

    Tileset(const char* source, int rows, int cols);
};

class Tilemap{
    Tileset* set;
    std::vector<tile> indices;
    int map[64][64];
    Vec2 tileDim;

    Tilemap(Tileset* i_set, Vec2 i_tileDim);
};