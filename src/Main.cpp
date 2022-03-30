#define SDL_MAIN_HANDLED
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include "SDL.h"
#include "SDL_image.h"
#include "Tileset.h"
#include "Camera.h"
#include "Collision.h"
#include "Player.h"
#include "Room.h"
#include "Level.h"
#include "Structs.h"
#include "Bullet.h"

#define WINDOW_X 1280
#define WINDOW_Y 720

SDL_Window* window;
SDL_Renderer* rend;

bool DEBUG = false;

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
	for (Uint64 x = 0; x < colliders.size(); x++){
		colliders[x]->world_ID = x;
	}
}


void GAME(){
	/*BOILERPLATE*/
	std::cout << "setting up required data" << std::endl;
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
	std::vector<Bullet> bullets;
	Room* currentRoom;
	
	//player stuff
	Collider hitbox({0, 0}, {32, 48}, true, true, false);
	Keybinds keys{SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_e};
	Camera camera(0, 0, WINDOW_X/2, WINDOW_Y/2);
	Tileset dungeonMisc("tex/DungeonTilesetMisc.png", 16, 16);
	Player player(hitbox, keys, 3.0f);
	player.hitbox.pos = {70, 70};
	colliders.push_back(&player.hitbox);
	camera.set_pos(player.hitbox.pos.x + player.hitbox.dim.x/2, player.hitbox.pos.y + player.hitbox.dim.y);
	
	//room stuff
	Index2 rIndex = {8,8};
	Tileset dungeonSet("tex/myTestTileset.png", 16, 16);
	Level level;
	currentRoom = level.get_room(rIndex.r, rIndex.c);
	reload_colliders(colliders, currentRoom, player);
	
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
		
		/*--INPUT--*/
		bool quit = false;
		while(SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				quit = true;
			}
			else if(e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){
					case SDLK_h:
						DEBUG = !DEBUG; //flip on or off
						std::cout << "DEBUG: " << (DEBUG ? "ON" : "OFF") << std::endl;
						break;
					case SDLK_e: //TODO: figure out how to use custom keybind here
						//which door is the player interacting with
						Door closest = currentRoom->doors[0];
						double distToDoor = calc_distance(player.hitbox.pos, closest.position);;
						double shortestDistance = distToDoor;

						for (int x = 1; x < currentRoom->doors.size(); x++){
							distToDoor = calc_distance(player.hitbox.pos, currentRoom->doors[x].position);
							
							if (distToDoor < shortestDistance){
								closest = currentRoom->doors[x];
								shortestDistance = distToDoor;
							}
						}

						if (shortestDistance < player.doorInteractDist) {

							if (closest.isVictory) return; //win the game

							//figure out which direction to move player
							CardinalBool moveDirection;
							if (closest.facing == _NORTH) {
								rIndex.r+=1; //player moves south
								moveDirection = _SOUTH;
							}
							else if (closest.facing == _EAST) {
								rIndex.c-=1; //player moves west
								moveDirection = _WEST;
							}
							else if (closest.facing == _SOUTH) {
								rIndex.r-=1; //player moves north
								moveDirection = _NORTH;
							}
							else if (closest.facing == _WEST) {
								rIndex.c+=1; //player moves east
								moveDirection = _EAST;
							}
							//change room
							currentRoom = level.get_room(rIndex.r, rIndex.c);
							reload_colliders(colliders, currentRoom, player);
							//set player position
							for (Door d : currentRoom->doors) {
								if (d.facing == moveDirection) {
									player.hitbox.pos = d.spawnPoint;
								}
							}
						}
						break;

				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				switch (e.button.button) {
				case SDL_BUTTON_LEFT:
					std::cout << "mouse click" << e.motion.x << "|" << e.motion.y << std::endl;
					Vec2 dir{ e.motion.x - player.hitbox.pos.x, e.motion.y - player.hitbox.pos.y };
					Bullet b(player.hitbox.pos, dir, true);
					bullets.push_back(b);
					colliders.push_back(&bullets[bullets.size()-1].hitbox);
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
		
		/*--COLLISION--*/
		for(Uint64 i = 0; i < colliders.size(); i++){
			if(colliders[i]->isActive && colliders[i]->isDynamic){
				std::vector<SortingData> contacts; //check "Collision.h" for SortingData definition
				for(Uint64 n = 0; n < colliders.size(); n++){
					if(colliders[n]->isActive && n != i){
						RaycastData data = check_dynamic_collision(colliders[i], colliders[n]);
						if(data.contact){
							SortingData d = {(int)n, data.contactTime};
							contacts.push_back(d);
						}
					}
				}
				
				//sorts the contacts array ascending by contactTime
				std::sort(contacts.begin(), contacts.end(), [](SortingData a, SortingData b){
					return a.contactTime < b.contactTime;
				});
				
				for(Uint64 z = 0; z < contacts.size(); z++){
					RaycastData data = check_dynamic_collision(colliders[i], colliders[contacts[z].index]);
					if (!colliders[i]->isTrigger){
						colliders[i]->vel.x += data.contactNormal.x * abs(colliders[i]->vel.x) * (1.0f-data.contactTime);
						colliders[i]->vel.y += data.contactNormal.y * abs(colliders[i]->vel.y) * (1.0f-data.contactTime);
					}
					else {
						//bullets
						std::cout << colliders[contacts[z].index]->world_ID << std::endl;
					}
				}
				
				colliders[i]->pos.x += colliders[i]->vel.x * deltaTime;
				colliders[i]->pos.y += colliders[i]->vel.y * deltaTime;
			}
		}
		
		/*--RENDER--*/
		SDL_SetRenderTarget(rend, gameView);
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);
		
		//tilemap
		SDL_Texture* setTex = SDL_CreateTextureFromSurface(rend, dungeonSet.atlas);
		for (int x = 0; x < TILEMAP_SIZE; x++){
			for (int y = 0; y < TILEMAP_SIZE; y++){
				Index2 t = currentRoom->tilemap.get_tile_indices(x, y); //this line is unpleasant
				SDL_Rect src = dungeonSet.get_tile(t);
				SDL_Rect dst = {x * ROOM_TILE_SIZE, y * ROOM_TILE_SIZE, ROOM_TILE_SIZE, ROOM_TILE_SIZE};
				SDL_RenderCopy(rend, setTex, &src, &dst);
			}
		}
		SDL_DestroyTexture(setTex);

		SDL_Texture* dungeonMiscTex = SDL_CreateTextureFromSurface(rend, dungeonMisc.atlas);
		//player
		SDL_Rect playerRect = collider_to_rect(player.hitbox);
		SDL_Rect playerSrc = dungeonMisc.get_tile({14, 6});
		SDL_RenderCopy(rend, dungeonMiscTex, &playerSrc, &playerRect);
		

		//enemies
		for (Enemy en : currentRoom->enemies) {
			SDL_Rect enemyRect = collider_to_rect(en.hitbox);
			SDL_Rect enemySrc = dungeonMisc.get_tile({ 12, 0 });
			SDL_RenderCopy(rend, dungeonMiscTex, &enemySrc, &enemyRect);
		}

		//bullets
		for (Bullet b : bullets) {
			std::cout << "bullet" << std::endl;
			SDL_Rect bulletRect = collider_to_rect(b.hitbox);
			SDL_SetRenderDrawColor(rend, 255, 0, 255, 255);
			SDL_RenderFillRect(rend, &bulletRect);
		}

		SDL_DestroyTexture(dungeonMiscTex);

		/*
		SDL_SetRenderDrawColor(rend, 255,255,255,255);
		SDL_RenderFillRect(rend, &playerRect);
		//*/
		
		//debug
		//always do last
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
	std::cout << "starting game" << std::endl;
	GAME();
	std::cout << "finishing game" << std::endl;
	return 0;
}
