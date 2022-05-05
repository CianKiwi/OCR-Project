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

#define WINDOW_X 1920
#define WINDOW_Y 1080

SDL_Window* window;
SDL_Renderer* rend;

bool DEBUG = false;
bool SHOW_MINIMAP = false;
int MAX_WORLD_ID = 0;

SDL_Rect collider_to_rect(const Collider& c){
	//creates an SDL_Rect from a collider for use in rendering
	SDL_Rect r={(int)c.pos.x, (int)c.pos.y, (int)c.dim.x, (int)c.dim.y};
	return r;
}

void reload_colliders(std::vector<Collider*>& colliders, Room* room, Player& player){
	colliders.clear();
	colliders.push_back(&player.hitbox);
	MAX_WORLD_ID = 0;
	for (Uint64 x = 0; x < room->walls.size(); x++){
		colliders.push_back(&room->walls[x]);
	}
	for (Uint64 x = 0; x < room->enemies.size(); x++){
		colliders.push_back(&room->enemies[x].hitbox);
	}
	for (Uint64 x = 0; x < colliders.size(); x++){
		colliders[x]->world_ID = MAX_WORLD_ID+1;
		MAX_WORLD_ID++;
	}
	player.hitbox.pos = room->playerSpawn;
}

int GAME(SDL_Window* window, SDL_Renderer* rend){
	
	
	/* PROGRAM START*/
	Vec2 mouseWorldPos = {0,0};
	SDL_Texture* gameView;
	std::vector<Collider*> colliders;
	std::vector<Bullet> bullets;
	Room* currentRoom;
	Level level; //automatically generates a level
	level.generate_level();
	int levelCount = 1;
	
	//room stuff
	Index2 rIndex = {level.spawnRoom.r, level.spawnRoom.c};
	Tileset dungeonSet("tex/myTestTileset.png", 16, 16);
	currentRoom = level.get_room(rIndex.r, rIndex.c);
	
	//player stuff
	Collider hitbox({0, 0}, {32, 48}, true, true, false);
	Keybinds keys{SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_q, SDLK_LSHIFT};
	Camera camera(0, 0, 800, 450);
	Tileset dungeonMisc("tex/DungeonTilesetMisc.png", 16, 16);
	Weapon weapon("default weapon", 1, 15, 345, 10); //damage, fireRate, distance, speed
	Player player(hitbox, keys, weapon, 2.5f);
	player.hitbox.pos = currentRoom->playerSpawn;
	camera.set_pos(player.hitbox.pos.x + player.hitbox.dim.x/2, player.hitbox.pos.y + player.hitbox.dim.y);
	
	reload_colliders(colliders, currentRoom, player);
	
	//setup graphics
	gameView = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 64*64, 64*64);

	/*GAME LOOP*/
	SDL_Event e;
	SDL_PollEvent(&e);
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	double interactDelay = 100.0f;
	while(e.type != SDL_QUIT){
		//calculate delta time
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST)*100 / (double)SDL_GetPerformanceFrequency());
		
		/*--INPUT--*/
		bool quit = false;
		if (interactDelay >= 0) interactDelay -= deltaTime;
		SDL_Point mouseWindowPos;
		Uint32 mouseButtons = SDL_GetMouseState(&mouseWindowPos.x, &mouseWindowPos.y);
		mouseWorldPos = {mouseWindowPos.x, mouseWindowPos.y};
		mouseWorldPos.x *= ((double)camera.view.w / WINDOW_X);
		mouseWorldPos.y *= ((double)camera.view.h / WINDOW_Y);
		mouseWorldPos.x += camera.view.x;
		mouseWorldPos.y += camera.view.y;

		player.weapon.timeSinceFired += deltaTime;

		while(SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				quit = true;
			}
			else if(e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){
					case SDLK_h:
						DEBUG = !DEBUG; //flip on or off
						std::cout << "DEBUG: " << (DEBUG ? "ON" : "OFF") << std::endl;
						std::cout << "CURRENT ROOM: " << rIndex.r << "|" << rIndex.c << std::endl;
						break;
					case SDLK_p:
						//reload current level
						bullets.clear();
						level.generate_level();
						rIndex = {level.spawnRoom.r, level.spawnRoom.c};
						currentRoom = level.get_room(rIndex.r, rIndex.c);
						bullets.clear();
						reload_colliders(colliders, currentRoom, player);
						break;
					case SDLK_o:
						player.health = 100;
						break;
					case SDLK_m:
						SHOW_MINIMAP = !SHOW_MINIMAP;
						break;
				}
			}
			///*
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				switch (e.button.button) {
				case SDL_BUTTON_LEFT:
					Vec2 dir{ mouseWorldPos.x - (player.hitbox.pos.x + player.hitbox.dim.x/2), mouseWorldPos.y - (player.hitbox.pos.y + player.hitbox.dim.y/2) };
					
					if(!player.isSprinting && player.weapon.timeSinceFired >= player.weapon.fireRate){
						Bullet b({player.hitbox.pos.x + player.hitbox.dim.x/2, player.hitbox.pos.y + player.hitbox.dim.y/2}, dir, player.weapon.bulletSpeed, player.weapon.bulletDistance, player.weapon.damage); //use weapon speed once weapon class is implemented
						bullets.push_back(b);
						player.weapon.timeSinceFired = 0;
					}
				}
			}
			//*/
		}
		if(quit) break;

		//must use this method to achieve smooth movement
		//poll event stutters in a weird way
		SDL_PumpEvents(); 
		const Uint8* keys = SDL_GetKeyboardState(0); 

		//interact
		if (keys[SDL_GetScancodeFromKey(player.binds.interact)] && interactDelay <= 0){
			interactDelay = 100.0f;
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

				if (closest.isVictory) {
					if(levelCount <= 7){
						bullets.clear();
						level.generate_level();
						rIndex = {level.spawnRoom.r, level.spawnRoom.c};
						currentRoom = level.get_room(rIndex.r, rIndex.c);
						bullets.clear();
						reload_colliders(colliders, currentRoom, player);
						levelCount++;
					}
					else{
						return levelCount; //win the game
					}
				}
				else{
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
					bullets.clear();
					//set player position
					for (Door d : currentRoom->doors) {
						if (d.facing == moveDirection) {
							player.hitbox.pos = d.spawnPoint;
						}
					}
				}
			}
		}

		
		//camera movement
		camera.set_pos(player.hitbox.pos.x + player.hitbox.dim.x/2, player.hitbox.pos.y + player.hitbox.dim.y/2);
		//keep texture within the bounds of the game view
		SDL_Point texDim;
		SDL_QueryTexture(gameView, 0, 0, &texDim.x, &texDim.y);
		if(camera.x - camera.view.w/2 < 0) camera.set_pos(camera.view.w/2, camera.y);
		if(camera.x + camera.view.w/2 > texDim.x) camera.set_pos(texDim.x - camera.view.w/2, camera.y);
		if(camera.y - camera.view.h/2 < 0) camera.set_pos(camera.x, camera.view.h/2);
		if(camera.y + camera.view.h/2 > texDim.y) camera.set_pos(camera.x, texDim.y - camera.view.h/2);
		
		/*--COLLISION AND MOVEMENT--*/
		std::vector<int> enemiesToDelete; //used to remove from vectors AFTER iterating over them, to prevent segfaults
		std::vector<int> bulletsToDelete;
		std::vector<int> collidersToDelete;
		
		//player movement (normalised)
		player.isSprinting = keys[SDL_GetScancodeFromKey(player.binds.sprint)];
		double speed = player.moveSpeed * (player.isSprinting ? player.sprintMulti : 1);
		if (player.isSprinting){ //prevent player from shooting a moment after sprinting
			player.weapon.timeSinceFired = -35;
		}
		double playerXmov = (keys[SDL_GetScancodeFromKey(player.binds.left)] ? -1 : 0) + (keys[SDL_GetScancodeFromKey(player.binds.right)] ? 1 : 0);
		double playerYmov = (keys[SDL_GetScancodeFromKey(player.binds.up)] ? -1 : 0) + (keys[SDL_GetScancodeFromKey(player.binds.down)] ? 1 : 0);
		double playerMovMagnitude = abs(sqrt(playerXmov * playerXmov + playerYmov * playerYmov)); //pythagoras
		player.hitbox.vel.x = playerXmov * speed / ((playerMovMagnitude > 0.0f) ? playerMovMagnitude : 1);
		player.hitbox.vel.y = playerYmov * speed / ((playerMovMagnitude > 0.0f) ? playerMovMagnitude : 1);
		
		//bullets movement
		for(Uint64 i = 0; i < bullets.size(); i++){
			bullets[i].distanceTravelled += calc_distance(bullets[i].pos, {bullets[i].pos.x + bullets[i].vel.x * deltaTime, bullets[i].pos.y + bullets[i].vel.y * deltaTime});
			bullets[i].pos.x += bullets[i].vel.x * deltaTime;
			bullets[i].pos.y += bullets[i].vel.y * deltaTime;
			
		}

		//enemy movement and AI
		for (Uint64 i = 0; i < currentRoom->enemies.size(); i++){
			Enemy& e = currentRoom->enemies[i];
			e.timeSinceAttack += deltaTime;
			if (abs(calc_distance(e.hitbox.pos, player.hitbox.pos)) < e.detectionDistance){

				if(e.timeSinceAttack >= e.attackRate){
					e.moveTarget = player.hitbox.pos;
					e.hitbox.vel = normalize({e.moveTarget.x - e.hitbox.pos.x, e.moveTarget.y - e.hitbox.pos.y});
					e.hitbox.vel.x *= e.moveSpeed;
					e.hitbox.vel.y *= e.moveSpeed;
				}
				else{
					e.hitbox.vel = {0, 0};
				}
			}
			else{
				e.hitbox.vel = {0, 0};
			}
			/*
			if(abs(e.hitbox.pos.x - e.moveTarget.x) < 5 && abs(e.hitbox.pos.y - e.moveTarget.y) < 5 || e.moveTarget.x < 100 && e.moveTarget.y < 100){
				e.moveTarget = {e.hitbox.pos.x + sin(e.hitbox.pos.x), e.hitbox.pos.y + sin(e.hitbox.pos.y)};
			}
			*/
			
		}
		
		enemiesToDelete.clear();
		collidersToDelete.clear();
		//move dynamic colliders appropriately
		for(Uint64 i = 0; i < colliders.size(); i++){
			if(colliders[i]->isActive && colliders[i]->isDynamic){
				std::vector<SortingData> contacts; //check "Collision.h" for SortingData definition
				for(Uint64 n = 0; n < colliders.size(); n++){
					if(colliders[n]->isActive && !colliders[n]->isDynamic && n != i){
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
				}
				colliders[i]->pos.x += colliders[i]->vel.x * deltaTime;
				colliders[i]->pos.y += colliders[i]->vel.y * deltaTime;
			}
			
			//bullets hit enemies
			bulletsToDelete.clear();
			for (Uint64 x = 0; x < bullets.size(); x++) {
				if(bullets[x].distanceTravelled >= bullets[x].maxDistance){
					bulletsToDelete.push_back(x);
				}
				else if (colliders[i]->isActive && colliders[i]->world_ID != player.hitbox.world_ID && check_point(bullets[x].pos, colliders[i])){
					for(Uint64 y = 0; y < currentRoom->enemies.size(); y++){
						if(currentRoom->enemies[y].hitbox.world_ID == colliders[i]->world_ID){ //if the hit collider belongs to an enemy, reduce the enemy's HP
							currentRoom->enemies[y].HP -= bullets[x].damage;
							currentRoom->enemies[y].holdDamageFramesFor = 10.5f; //enemy rendered red for 150ms
							if (currentRoom->enemies[y].HP <= 0){
								currentRoom->enemies[y].hitbox.isActive = false;
							}
						}
					}
					bulletsToDelete.push_back(x);
				}
			}

			for (Uint64 i = 0; i < bulletsToDelete.size(); i++){
				bullets.erase(bullets.begin() + bulletsToDelete[i]);
			}
			
			//enemy attacks player
			for (Uint64 i = 0; i < currentRoom->enemies.size(); i++){
				Enemy& e = currentRoom->enemies[i];
				if(player.holdDamageFramesFor <= 0 && e.hitbox.isActive && e.timeSinceAttack >= e.attackRate && check_overlap(&e.hitbox, &player.hitbox)){
					player.health -= 1;
					player.holdDamageFramesFor = 45.5f;
					Vec2 directionFromPlayer = normalize({e.hitbox.pos.x - player.hitbox.pos.x, e.hitbox.pos.y - player.hitbox.pos.y});
					e.hitbox.pos.x += directionFromPlayer.x * 20;
					e.hitbox.pos.y += directionFromPlayer.y * 20;
					e.timeSinceAttack = 0;
				}
			}
		}
		if (player.health <= 0){
			return levelCount;
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
		
		//enemies
		for (Uint64 i = 0; i < currentRoom->enemies.size(); i++) {
			Enemy& en = currentRoom->enemies[i];
			SDL_Rect enemyRect = collider_to_rect(en.hitbox);
			SDL_Rect enemySrc = {0, 0, 0, 0};
			if(en.HP <= 0){
				enemySrc = dungeonMisc.get_tile({ 13, 0 });
			}
			else{
				enemySrc = dungeonMisc.get_tile({ 12, 0 });
			}
			if(en.holdDamageFramesFor > 0){
				SDL_SetTextureColorMod(dungeonMiscTex, 255, 128, 128);
				en.holdDamageFramesFor -= deltaTime;
			}
			SDL_RenderCopy(rend, dungeonMiscTex, &enemySrc, &enemyRect);
			SDL_SetTextureColorMod(dungeonMiscTex, 255, 255, 255);
		}
		
		//player
		SDL_Rect playerRect = collider_to_rect(player.hitbox);
		SDL_Rect playerSrc = dungeonMisc.get_tile({14, 6});
		if(player.holdDamageFramesFor > 0){
				SDL_SetTextureColorMod(dungeonMiscTex, 255, 64, 64);
				player.holdDamageFramesFor -= deltaTime;
			}
		SDL_RenderCopy(rend, dungeonMiscTex, &playerSrc, &playerRect);
		SDL_SetTextureColorMod(dungeonMiscTex, 255, 255, 255);

		//bullets
		for (Bullet b : bullets) {
			SDL_Rect bulletRect = {b.pos.x-3, b.pos.y-3, 6, 6};
			SDL_SetRenderDrawColor(rend, 235, 235, 255, 255);
			SDL_RenderFillRect(rend, &bulletRect);
		}
		
		//debug
		//do last, but before UI
		if(DEBUG){
			SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
			for(Uint64 i = 0; i < colliders.size(); i++){
				if(colliders[i]->isActive){
					SDL_Rect cRect = collider_to_rect(*colliders[i]);
					SDL_SetRenderDrawColor(rend, sqrt(i)*255, cos(i)*255, sin(i)*255, 128);
					SDL_RenderFillRect(rend, &cRect);
				}
			}
			SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
			SDL_RenderDrawLine(rend, player.hitbox.pos.x + player.hitbox.dim.x/2, player.hitbox.pos.y + player.hitbox.dim.y/2, mouseWorldPos.x, mouseWorldPos.y);
			SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_NONE);
		}
		
		//health bar
		SDL_Rect heartRect = {camera.view.x + 10, camera.view.y + 10, 40, 40};
		SDL_Rect heartSrc = dungeonMisc.get_tile({13, 6});
		for(int x = 0; x < player.health; x++){
			SDL_RenderCopy(rend, dungeonMiscTex, &heartSrc, &heartRect);
			heartRect.x += heartRect.w;
		}
		
		//minimap
		SDL_Rect minimapRect = {camera.view.x + camera.view.w - camera.view.h/1.5f, camera.view.y + camera.view.h/3, camera.view.h/1.5f, camera.view.h/1.5f};
		SDL_Texture* minimapTex = SDL_CreateTextureFromSurface(rend, level.minimap);
		if(SHOW_MINIMAP){
			SDL_RenderCopy(rend, minimapTex, 0, &minimapRect);
		}
		
		SDL_DestroyTexture(minimapTex);
		SDL_DestroyTexture(dungeonMiscTex);
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
	return levelCount;
}

int main(int argc, char** argv){
	std::cout << "starting game" << std::endl;
	
	/*BOILERPLATE*/
	std::cout << "setting up required data" << std::endl;
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
	window = SDL_CreateWindow("H446 GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_X, WINDOW_Y, SDL_WINDOW_FULLSCREEN_DESKTOP);
	
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!window || !rend){
		std::cout << "error creating window or renderer" << std::endl;
		return 0;
	}
	
	int count = GAME(window, rend);
	std::cout << "you made it to level " << count << std::endl;
	std::cout << "finishing game" << std::endl;
	return 0;
}
