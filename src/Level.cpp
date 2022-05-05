#include "Level.h"

Level::Level(){
	//set all squares to -1
	for (int x = 0; x < LEVELMAP_SIZE; x++){
		for (int y = 0; y < LEVELMAP_SIZE; y++){
			levelMap[x][y] = -1;
		}
	}
}

void Level::generate_level(){
	rooms.clear();
	std::random_device generator;
	
	//set all squares to -1
	for (int x = 0; x < LEVELMAP_SIZE; x++){
		for (int y = 0; y < LEVELMAP_SIZE; y++){
			levelMap[x][y] = -1;
		}
	}
	
	//probability weights
    int door1 = 15;
    int door2 = 25;
    int door3 = 45;
    int door4 = 10;    

    std::queue<Index2> queue;
    Index2 i = {LEVELMAP_SIZE/2,LEVELMAP_SIZE/2}; //start making rooms at the center
    CardinalBool map[LEVELMAP_SIZE][LEVELMAP_SIZE];
	map[i.r][i.c] = _FULL; //set first room to NESW
    queue.push({i.r-1, i.c});
    queue.push({i.r, i.c+1});
    queue.push({i.r+1, i.c});
    queue.push({i.r, i.c-1});
    
    while (!queue.empty()){
        i = queue.front();
        queue.pop();
        CardinalBool& cb = map[i.r][i.c];
        for (int x = 0; x < 4; x++){
			cb.data[x] = 1;
		}
        CardinalBool req(0,0,0,0);
        CardinalBool next(0,0,0,0);
		
		//RESTRICTIONS:
        //map boundaries
        if (i.r == 0) cb.N = 0;
        if (i.c == LEVELMAP_SIZE-1) cb.E = 0;
        if (i.r == LEVELMAP_SIZE-1) cb.S = 0;
        if (i.c == 0) cb.W = 0;
        //pre-occupied spaces
        if (cb.N && map[i.r-1][i.c] != _EMPTY){
          if (map[i.r-1][i.c].S == 0) cb.N = 0;
          else req.N = 1;
        } 
        if (cb.E && map[i.r][i.c+1] != _EMPTY){
          if(map[i.r][i.c+1].W == 0) cb.E = 0;
          else req.E = 1;
        } 
        if (cb.S && map[i.r+1][i.c] != _EMPTY){
          if(map[i.r+1][i.c].N == 0) cb.S = 0;
          else req.S = 1;
        } 
        if (cb.W && map[i.r][i.c-1] != _EMPTY){
          if(map[i.r][i.c-1].E == 0) cb.W = 0;
          else req.W = 1;
        }
        
		//determine number of doors to use
        int doorCount = 0;
        std::random_device generator;
        std::uniform_int_distribution<int> distribution(0,door1+door2+door3+door4);
        int Rnum = distribution(generator);
        if (Rnum < door1) doorCount = 1;
        else if (Rnum < door1+door2) doorCount = 2;
        else if (Rnum < door1+door2+door3) doorCount = 3;
        else if (Rnum < door1+door2+door3+door4) doorCount = 4;
		
		if (doorCount != 1) {
			door1 += 1; 
			door2 -=1; 
			door3 -= 1;
		} //increase chance of 1-door room as generation progresses
		
        int currentCount = 0;
		for (int x = 0; x < 4; x++){
			if (req.data[x]) currentCount++;
		}
        
		//generate random order to set the doors
		int order[4] = {-1, -1, -1, -1};
		std::uniform_int_distribution<int> orderDistro(0, 3);
		for (int x = 0; x < 4; x++){
			bool clear = false;
			while (!clear){
				clear = true;
				int n = orderDistro(generator);
				for (int x : order){
					if (n == x){
						clear = false;
					}
				}
				if (clear){
					order[x] = n;
				}
			}
		}
		
		//set the doors
		for (int x = 0; x < 4; x++){
			int i = order[x];
			if (currentCount < doorCount && cb.data[i] && !req.data[i]) 
			{
				cb.data[i] = 1; 
				req.data[i] = 1; 
				next.data[i] = 1; 
				currentCount++;
			}
		}
		
		for (int x = 0; x < 4; x++){
			if (cb.data[x] && !req.data[x]) cb.data[x] = 0;
		}
        
		//prepare next
        if (next.N) queue.push({i.r-1, i.c});
        if (next.E) queue.push({i.r, i.c+1});
        if (next.S) queue.push({i.r+1, i.c});
        if (next.W) queue.push({i.r, i.c-1});
    }
	
	bool placedVictoryDoor = false;
	//DETERMINE VICTORY ROOM
	Index2 victoryRoom = {0, 0};
	std::uniform_int_distribution<int> randomRoomNumber(0, 15);
	victoryRoom.r = randomRoomNumber(generator);
	victoryRoom.c = randomRoomNumber(generator);
	while (!placedVictoryDoor){
		if (map[victoryRoom.r][victoryRoom.c] != _EMPTY){
			placedVictoryDoor = true;
		}
		else{
			victoryRoom.c += 1;
			if(victoryRoom.c == LEVELMAP_SIZE){
				victoryRoom.c = 0;
				victoryRoom.r += 1;
				victoryRoom.r %= LEVELMAP_SIZE;
			}
		}
	}
	std::cout << "victory room is located at " << victoryRoom.r << "|" << victoryRoom.c << std::endl;

	bool setPlayerSpawn = false;
	//DETERMINE SPAWN ROOM
	//spawnRoom is class attribute
	spawnRoom.r = randomRoomNumber(generator);
	spawnRoom.c = randomRoomNumber(generator);
	while (!setPlayerSpawn){
		if (map[spawnRoom.r][spawnRoom.c] != _EMPTY){
			setPlayerSpawn = true;
		}
		else{
			spawnRoom.c += 1;
			if(spawnRoom.c == LEVELMAP_SIZE){
				spawnRoom.c = 0;
				spawnRoom.r += 1;
				spawnRoom.r %= LEVELMAP_SIZE;
			}
		}
	}
	std::cout << "spawn room is located at " << spawnRoom.r << "|" << spawnRoom.c << std::endl;

	//GENERATE MINIMAP TEXTURE
	minimap = SDL_CreateRGBSurface(0,LEVELMAP_SIZE*5,LEVELMAP_SIZE*5,32,0,0,0,0);
	SDL_Rect roomRect = {1, 1, 3, 3};
	SDL_Rect doorwayRect = {1, 1, 1, 1};
	Uint32 colour = 0xFFFFFF;
	for (int x = 0; x < 16; x++){
        for (int y = 0; y < 16; y++){
			//victory room is yellow, spawn room is blue, rest are white
			colour = (x == victoryRoom.r && y == victoryRoom.c) ? 0xFFFF00 : 0xFFFFFF;
			colour = (x == spawnRoom.r && y == spawnRoom.c) ? 0x00AAFF : colour;
			if (map[x][y] != _EMPTY){
				//render room as square
				SDL_FillRect(minimap, &roomRect, colour);
				//render doors around room
				if(map[x][y].N){
					doorwayRect.x = roomRect.x + 1;
					doorwayRect.y = roomRect.y - 1;
					SDL_FillRect(minimap, &doorwayRect, colour);
				}
				if(map[x][y].E){
					doorwayRect.x = roomRect.x + 3;
					doorwayRect.y = roomRect.y + 1;
					SDL_FillRect(minimap, &doorwayRect, colour);
				}
				if(map[x][y].S){
					doorwayRect.x = roomRect.x + 1;
					doorwayRect.y = roomRect.y + 3;
					SDL_FillRect(minimap, &doorwayRect, colour);
				}
				if(map[x][y].W){
					doorwayRect.x = roomRect.x - 1;
					doorwayRect.y = roomRect.y + 1;
					SDL_FillRect(minimap, &doorwayRect, colour);
				}
			}
			else{
				//no room here
				SDL_FillRect(minimap, &roomRect, 0x331111);
			}
			roomRect.x += 5;
		}
		roomRect.x = 1;
		roomRect.y += 5;
	}
	
	//generate rooms
	int index = 0;
	
	std::uniform_int_distribution<int> distribution(1, 2);
	for (int r = 0; r < LEVELMAP_SIZE; r++){
		for (int c = 0; c < LEVELMAP_SIZE; c++){
			if (map[r][c] != _EMPTY){
				std::string filename = "rooms/";
				if (map[r][c].N) filename += "N";
				if (map[r][c].E) filename += "E";
				if (map[r][c].S) filename += "S";
				if (map[r][c].W) filename += "W";
				int variation = distribution(generator);
				filename += "_";
				filename += char(variation + 48);
				filename += ".txt";
				for (int x = 0; x < 4; x++){
					std::cout << map[r][c].data[x];
				}
				std::cout << "  " << filename << std::endl;
				Room room(filename.c_str(), (r == victoryRoom.r && c == victoryRoom.c));
				rooms.push_back(room);
				levelMap[r][c] = index;
				index++;
			}
		}
	}
}

Room* Level::get_room(int r, int c){
	return &rooms[levelMap[r][c]];
}