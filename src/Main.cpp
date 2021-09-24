#define SDL_MAIN_HANDLED
#include <iostream>
#include <cmath>
#include "SDL.h"
#include "SDL_image.h"
#include "Camera.h"
#include "Collision.h"
#include "Player.h"
#include "Room.h"
#include "Tileset.h"

#define WINDOW_X 1280
#define WINDOW_Y 720

SDL_Window* window;
SDL_Renderer* rend;

int main(){
	/*BOILERPLATE*/
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
	window = SDL_CreateWindow("H446 GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_X, WINDOW_Y, 0);
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!window || !rend){
		std::cout << "error creating window or renderer" << std::endl;
		return -1;
	}
	
	/* PROGRAM START*/
	Tileset dungeonSet("tex/myTestTileset.png", 16, 16);
	SDL_Texture* dungeonSetTex;
	dungeonSetTex = SDL_CreateTextureFromSurface(rend, dungeonSet.atlas);
	Room firstRoom("room1.txt", &dungeonSet, WINDOW_X/2, WINDOW_Y/2);
	//Room secondRoom(&dungeonSet);
	SDL_Texture* gameView;
	SDL_Texture* roomTex;
	
	
	//player stuff
	Collider hitbox(0, 0, 32, 48);
	Keybinds keys{SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D};
	Camera camera(0, 0, WINDOW_X, WINDOW_Y);
	Player player(0, 0, 1.5f, hitbox, keys);
	player.moveRestrictions = {false, false, false, false};
	player.set_pos(1130, 720);
	camera.set_pos(player.x + player.hitbox.width/2, player.y + player.hitbox.height);
	
	
	
	gameView = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, camera.view.w + 16 * TILE_DIM_X, camera.view.h + 16 * TILE_DIM_Y);
	//render the room to a texture to save on computations later
	roomTex = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, camera.view.w + 16 * TILE_DIM_X, camera.view.h + 16 * TILE_DIM_Y);
	SDL_SetRenderTarget(rend, roomTex);
	for(int y = 0; y < 16; y++){
		for(int x = 0; x < 16; x++){
			SDL_Rect src = dungeonSet.get_tile(firstRoom.tilemap[x][y].row, firstRoom.tilemap[x][y].col);
			//the tiles are offset to stop the texture from stretching when the camera goes over the edge
			SDL_Rect dst = {x*TILE_DIM_X + camera.view.w/2, y*TILE_DIM_Y + camera.view.h/2, TILE_DIM_X, TILE_DIM_Y};
			SDL_RenderCopy(rend, dungeonSetTex,
				&src, //source rect from room tilemap
				&dst); //destination rect
			SDL_RenderPresent(rend);
		}
	}
	
	/*GAME LOOP*/
	SDL_Event e;
	SDL_PollEvent(&e);
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	while(e.type != SDL_QUIT){
		//calculate delta time
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST)*100 / (double)SDL_GetPerformanceFrequency());
		
		/*--Collision--*/
		CollisionData data;
		player.moveRestrictions = {false,false,false,false};
		for (int x = 0; x < firstRoom.walls.size(); x++){
			data = check_collision(&player.hitbox, &firstRoom.walls[x]);
			if (data.collision){
				/*do some funky stuff*/
				std::cout << "COLLISION" << std::endl;
				player.moveRestrictions.up |= data.direction.up;
				player.moveRestrictions.down |= data.direction.down;
				player.moveRestrictions.left |= data.direction.left;
				player.moveRestrictions.right |= data.direction.right;
			}
		}
		
		/*--Player Input & movement--*/
		bool quit = false;
		while(SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				quit = true;
			}
		}
		if(quit) break;
		
		//must use this method to achieve smooth movement
		//poll event stutters in a weird way
		SDL_PumpEvents(); 
		const Uint8* keys = SDL_GetKeyboardState(0); 

		//movement (normalised)
		double playerXmov = (keys[player.binds.left] ? -1 : 0) + (keys[player.binds.right] ? 1 : 0);
		double playerYmov = (keys[player.binds.up] ? -1 : 0) + (keys[player.binds.down] ? 1 : 0);
		double playerMovMagnitude = sqrt(playerXmov * playerXmov + playerYmov * playerYmov); //pythagoras
		player.move(playerXmov * player.speed * deltaTime / playerMovMagnitude, playerYmov * player.speed * deltaTime / playerMovMagnitude);
		
		//Other Variables
		camera.set_pos(player.x + player.hitbox.width/2, player.y + player.hitbox.height/2);
		
		//Render to game view
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_SetRenderTarget(rend, gameView);
		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, roomTex, 0, 0);
		SDL_Rect playerRect = {(int)player.hitbox.x, (int)player.hitbox.y, (int)player.hitbox.width, (int)player.hitbox.height};
		SDL_SetRenderDrawColor(rend, 255, 196, 196, 255);
		SDL_RenderFillRect(rend, &playerRect);
		
		
		//render colliders (for debug)
		SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
		for (int x = 0; x < firstRoom.walls.size(); x++){
			SDL_SetRenderDrawColor(rend, sin(firstRoom.walls[x].x*firstRoom.walls[x].y)*255, sin(firstRoom.walls[x].y/firstRoom.walls[x].x)*255, 128, 200);
			SDL_Rect rect = {(int)firstRoom.walls[x].x, (int)firstRoom.walls[x].y, (int)firstRoom.walls[x].width, (int)firstRoom.walls[x].height};
			SDL_RenderFillRect(rend, &rect);
		}
		//render door colliders (for debug)
		for (Collider c : firstRoom.doors){
			SDL_SetRenderDrawColor(rend, 255, 255, 255, 200);
			SDL_Rect rect = {(int)c.x, (int)c.y, (int)c.width, (int)c.height};
			SDL_RenderFillRect(rend, &rect);
		}
		SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_NONE);
		
		SDL_RenderPresent(rend);
		
		//render to window
		SDL_SetRenderTarget(rend, 0);
		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, gameView, &camera.view, 0);
		SDL_RenderPresent(rend);
	}
	//end of game loop

	/*PROGRAM END*/
	SDL_DestroyTexture(gameView);
	SDL_DestroyTexture(roomTex);
	SDL_DestroyTexture(dungeonSetTex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}