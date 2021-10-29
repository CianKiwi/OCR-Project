#define SDL_MAIN_HANDLED
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "SDL.h"
#include "SDL_image.h"
#include "Camera.h"
#include "Collision.h"
#include "Player.h"
#include "Tileset.h"

#define WINDOW_X 1280
#define WINDOW_Y 720

SDL_Window* window;
SDL_Renderer* rend;

bool DEBUG = true;

SDL_Rect collider_to_rect(const Collider& c){
	SDL_Rect r={(int)c.pos.x, (int)c.pos.y, (int)c.dim.x, (int)c.dim.y};
	return r;
}

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
	SDL_Texture* gameView;
	std::vector<Collider*> colliders;

	//player stuff
	Collider hitbox({0, 0}, {32, 48}, true, false);
	Keybinds keys{SDLK_w, SDLK_s, SDLK_a, SDLK_d};
	Camera camera(0, 0, WINDOW_X, WINDOW_Y);
	Player player(hitbox, keys, 3.0f);
	player.hitbox.pos = {1130, 720};
	colliders.push_back(&player.hitbox);
	camera.set_pos(player.hitbox.pos.x + player.hitbox.dim.x/2, player.hitbox.pos.y + player.hitbox.dim.y);
	
	//test colliders
	Collider wallA({1200, 500},{70, 70}, false, false);
	Collider wallB({900, 1050},{85, 130}, false, false);
	colliders.push_back(&wallA);
	colliders.push_back(&wallB);
	
	Collider wallC({1300, 600},{127, 45}, false, false);
	colliders.push_back(&wallC);
	Collider wallD({1000, 900}, {10, 10}, false, false);
	colliders.push_back(&wallD);
	
	Collider tileA({690, 1300}, {65, 65}, false, false);
	Collider tileB({755, 1300}, {65, 65}, false, false);
	Collider tileC({820, 1300}, {65, 65}, false, false);
	colliders.push_back(&tileA);
	colliders.push_back(&tileB);
	colliders.push_back(&tileC);
	
	//setup graphics
	gameView = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 2048, 2048);
	
	/*DEBUGGING AND TEST CODE*/
	

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
		//TODO: implement javidx9 AABB tutorial
		for(Uint64 i = 0; i < colliders.size(); i++){
			if(colliders[i]->isDynamic){
				std::vector<SortingData> contacts; //check "Collision.h" for SortingData definition
				for(Uint64 n = 0; n < colliders.size(); n++){
					if(n != i){
						RaycastData data = check_dynamic_collision(colliders[i], colliders[n]);
						if(data.contact){
							colliders[i]->vel.x += data.contactNormal.x * abs(colliders[i]->vel.x) * (1.0f-data.contactTime);
					colliders[i]->vel.y += data.contactNormal.y * abs(colliders[i]->vel.y) * (1.0f-data.contactTime);
						}
					}
				}
				
				colliders[i]->pos.x += colliders[i]->vel.x * deltaTime;
				colliders[i]->pos.y += colliders[i]->vel.y * deltaTime;
			}
		}
		//Render to game view
		SDL_SetRenderTarget(rend, gameView);
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);
		
		if(DEBUG){
			for(Uint64 i = 0; i < colliders.size(); i++){
				SDL_Rect cRect = collider_to_rect(*colliders[i]);
				SDL_SetRenderDrawColor(rend, sqrt(i)*255, cos(i)*255, sin(i)*255, 255);
				SDL_RenderFillRect(rend, &cRect);
			}
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
	return 0;
}