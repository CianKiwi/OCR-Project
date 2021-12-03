#include "Tileset.h"

Tilemap::Tilemap(Tileset* i_set)
    : set{i_set}{
    indices.push_back({0,0});
}

Tilemap::Tilemap(){
	indices.push_back({0,0});
}

tile Tilemap::get_tile_indices(int x, int y){
	return indices[map[x][y]];
}

Tileset::Tileset(const char* source, int i_rows, int i_cols){
    atlas = IMG_Load(source);
    if(!atlas){
        std::cout << "ERROR: failed to load tileset. \n" << source << std::endl;
        tileWidth = 1;
        tileHeight = 1;
        rows = 1;
        cols = 1;
    }
    else{
        rows = i_rows;
        cols = i_cols;
        tileWidth = atlas->w / cols;
        tileHeight = atlas->h / rows;
    }
}

SDL_Rect Tileset::get_tile(tile t){
    SDL_Rect rect{0,0,0,0};
    if(t.row < rows && t.col < cols){
        rect.x = tileWidth * t.col;
        rect.y = tileHeight * t.row;
        rect.w = tileWidth;
        rect.h = tileHeight;
    }
    else{
        std::cout << "Warning: tile index out of range for tileset." << std::endl;
    }
    return rect;
}