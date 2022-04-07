#include "Level.h"

Level::Level(){
	
	//set all squares to -1
	for (int x = 0; x < LEVELMAP_SIZE; x++){
		for (int y = 0; y < LEVELMAP_SIZE; y++){
			levelMap[x][y] = -1;
		}
	}
	
	//probability weights
    int door1 = 15;
    int door2 = 45;
    int door3 = 15;
    int door4 = 1;    

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
	
	//PRINT MAP
    for (int x = 0; x < 16; x++){
        for (int y = 0; y < 16; y++){
            if (map[x][y] == _EMPTY) std::cout << "[----]";
            else std::cout << "[" << map[x][y].N << map[x][y].E << map[x][y].S << map[x][y].W << "]";
			//if (map[x][y] == _EMPTY) std::cout << "-";
            //else std::cout << "#";
        }
        std::cout << "\n";
    }
	
	//generate rooms
	bool placedVictoryDoor = false;
	int index = 0;
	std::random_device generator;
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
				Room room(filename.c_str(), !placedVictoryDoor);
				//place one door in the level as victory door
				if (!placedVictoryDoor) {
					std::cout << "victory door in room " << r << "|" << c << std::endl;
					placedVictoryDoor = true;
				}
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