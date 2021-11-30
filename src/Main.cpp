#define SDL_MAIN_HANDLED
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "SDL.h"
#include "SDL_image.h"
#include "Tileset.h"
#include "Camera.h"
#include "Collision.h"
#include "Player.h"
#include "Room.h"

#define WINDOW_X 1280
#define WINDOW_Y 720

SDL_Window* window;
SDL_Renderer* rend;

bool DEBUG = true;

SDL_Rect collider_to_rect(const Collider& c){
	//creates an SDL_Rect from a collider for use in rendering
	SDL_Rect r={(int)c.pos.x, (int)c.pos.y, (int)c.dim.x, (int)c.dim.y};
	return r;
}

void reload_colliders(std::vector<Collider*>& colliders, Room* room, Player& player){
	colliders.clear();
	colliders.push_back(&player.hitbox);
	for (Uint64 x = 0; x < room->walls.size(); x++){
		colliders.push_back(&room->walls[x]);
	}
	for (Uint64 x = 0; x < room->doors.size(); x++){
		colliders.push_back(&room->doors[x]);
	}
}

void GAME(){
	/*BOILERPLATE*/
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
	window = SDL_CreateWindow("H446 GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_X, WINDOW_Y, 0);
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!window || !rend){
		std::cout << "error creating window or renderer" << std::endl;
		return;
	}
	
	/* PROGRAM START*/
	SDL_Texture* gameView;
	std::vector<Collider*> colliders;
	Room* currentRoom;
	
	//player stuff
	Collider hitbox({0, 0}, {32, 48}, true, true, false);
	Keybinds keys{SDLK_w, SDLK_s, SDLK_a, SDLK_d};
	Camera camera(0, 0, WINDOW_X/2, WINDOW_Y/2);
	Player player(hitbox, keys, 3.0f);
	player.hitbox.pos = {70, 70};
	colliders.push_back(&player.hitbox);
	camera.set_pos(player.hitbox.pos.x + player.hitbox.dim.x/2, player.hitbox.pos.y + player.hitbox.dim.y);
	
	//room stuff
	Tileset dungeonSet("tex/myTestTileset.png", 16, 16);
	Room myRoom("room3.txt", &dungeonSet);
	currentRoom = &myRoom;
	for(Uint64 i = 0; i < myRoom.walls.size(); i++){
		colliders.push_back(&myRoom.walls[i]);
	}
	for(Uint64 i = 0; i < myRoom.doors.size(); i++){
		colliders.push_back(&myRoom.doors[i]);
	}
	Room myRoom2("room2.txt", &dungeonSet);
	for(Uint64 i = 0; i < myRoom.walls.size(); i++){
		colliders.push_back(&myRoom.walls[i]);
	}
	for(Uint64 i = 0; i < myRoom.doors.size(); i++){
		colliders.push_back(&myRoom.doors[i]);
	}
	//data for testing room swaps
	Room* rooms[2] = {&myRoom, &myRoom2};
	int rmCounterTest;
	
	for (int x = 0; x < TILEMAP_SIZE; x++){
		std::cout << "[";
		for (int y : currentRoom->tilemap.map[x]){
			std::cout << y << ", ";
		}
		std::cout << " ]" << std::endl;
	}
	//*/
	//test colliders
	
	Collider wallA({20, 16},{12, 12});
	Collider wallB({ 80, 190 }, { 34, 16});
	colliders.push_back(&wallA);
	colliders.push_back(&wallB);
	//*/
	
	//setup graphics
	gameView = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 64*64, 64*64);

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
		
		/*--Player Input & movement--*/
		bool quit = false;
		while(SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				quit = true;
			}
			else if(e.type == SDL_KEYDOWN){
				if(e.key.keysym.sym == 'h'){
					DEBUG = !DEBUG; //flip on or off
					std::cout << "DEBUG: " << (DEBUG ? "ON" : "OFF") << std::endl;
				}
				if(e.key.keysym.sym == 'r'){
					rmCounterTest++;
					rmCounterTest %= 2;
					currentRoom = rooms[rmCounterTest];
					reload_colliders(colliders, currentRoom, player);
				}
			}
		}
		if(quit) break;

		//must use this method to achieve smooth movement
		//poll event stutters in a weird way
		SDL_PumpEvents(); 
		const Uint8* keys = SDL_GetKeyboardState(0); 

		//movement (normalised)
		double playerXmov = (keys[SDL_GetScancodeFromKey(player.binds.left)] ? -1 : 0) + (keys[SDL_GetScancodeFromKey(player.binds.right)] ? 1 : 0);
		double playerYmov = (keys[SDL_GetScancodeFromKey(player.binds.up)] ? -1 : 0) + (keys[SDL_GetScancodeFromKey(player.binds.down)] ? 1 : 0);
		double playerMovMagnitude = abs(sqrt(playerXmov * playerXmov + playerYmov * playerYmov)); //pythagoras
		player.hitbox.vel.x = playerXmov * player.moveSpeed / ((playerMovMagnitude > 0.0f) ? playerMovMagnitude : 1);
		player.hitbox.vel.y = playerYmov * player.moveSpeed / ((playerMovMagnitude > 0.0f) ? playerMovMagnitude : 1);
		
		camera.set_pos(player.hitbox.pos.x + player.hitbox.dim.x/2, player.hitbox.pos.y + player.hitbox.dim.y/2);
		//keep texture within the bounds of the game view
		SDL_Point texDim;
		SDL_QueryTexture(gameView, 0, 0, &texDim.x, &texDim.y);
		if(camera.x - camera.view.w/2 < 0) camera.set_pos(camera.view.w/2, camera.y);
		if(camera.x + camera.view.w/2 > texDim.x) camera.set_pos(texDim.x - camera.view.w/2, camera.y);
		if(camera.y - camera.view.h/2 < 0) camera.set_pos(camera.x, camera.view.h/2);
		if(camera.y + camera.view.h/2 > texDim.y) camera.set_pos(camera.x, texDim.y - camera.view.h/2);
		
		/*--Collision--*/
		for(Uint64 i = 0; i < colliders.size(); i++){
			if(colliders[i]->isActive && colliders[i]->isDynamic){
				std::vector<SortingData> contacts; //check "Collision.h" for SortingData definition
				for(Uint64 n = 0; n < colliders.size(); n++){
					if(colliders[n]->isActive && n != i){
						RaycastData data = check_dynamic_collision(colliders[i], colliders[n]);
						if(data.contact){
							SortingData d = {(int)n, data.contactTime};
							contacts.push_back(d);
							//vv this is done later vv
							//colliders[i]->vel.x += data.contactNormal.x * abs(colliders[i]->vel.x) * (1.0f-data.contactTime);
							//colliders[i]->vel.y += data.contactNormal.y * abs(colliders[i]->vel.y) * (1.0f-data.contactTime);
						}
					}
				}
				
				//sorts the contacts array ascending by contactTime
				std::sort(contacts.begin(), contacts.end(), [](SortingData a, SortingData b){
					return a.contactTime < b.contactTime;
				});
				
				for(Uint64 z = 0; z < contacts.size(); z++){
					RaycastData data = check_dynamic_collision(colliders[i], colliders[contacts[z].index]);
					colliders[i]->vel.x += data.contactNormal.x * abs(colliders[i]->vel.x) * (1.0f-data.contactTime);
					colliders[i]->vel.y += data.contactNormal.y * abs(colliders[i]->vel.y) * (1.0f-data.contactTime);
				}
				
				colliders[i]->pos.x += colliders[i]->vel.x * deltaTime;
				colliders[i]->pos.y += colliders[i]->vel.y * deltaTime;
			}
		}
		//Render to game view
		SDL_SetRenderTarget(rend, gameView);
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);
		
		//shoddy code to render tilemap
		//--TEMPORARY--
		SDL_Texture* setTex = SDL_CreateTextureFromSurface(rend, currentRoom->tilemap.set->atlas);
		for (int x = 0; x < TILEMAP_SIZE; x++){
			for (int y = 0; y < TILEMAP_SIZE; y++){
				tile t = currentRoom->tilemap.get_tile_indices(x, y); //this line is unpleasant
				SDL_Rect src = currentRoom->tilemap.set->get_tile(t);
				SDL_Rect dst = {x * ROOM_TILE_SIZE, y * ROOM_TILE_SIZE, ROOM_TILE_SIZE, ROOM_TILE_SIZE};
				SDL_RenderCopy(rend, setTex, &src, &dst);
			}
		}
		SDL_DestroyTexture(setTex);

		//render player  
		SDL_Rect playerRect = collider_to_rect(player.hitbox);
		SDL_SetRenderDrawColor(rend, 255,255,255,255);
		SDL_RenderFillRect(rend, &playerRect);
		
		
		/*ALWAYS DO LAST*/
		if(DEBUG){
			SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
			for(Uint64 i = 0; i < colliders.size(); i++){
				if(colliders[i]->isActive){
					SDL_Rect cRect = collider_to_rect(*colliders[i]);
					SDL_SetRenderDrawColor(rend, sqrt(i)*255, cos(i)*255, sin(i)*255, 128);
					SDL_RenderFillRect(rend, &cRect);
				}
			}
			SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_NONE);
		}
		
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
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return;
}

int main(int argc, char** argv){
	GAME();
	return 0;
}
