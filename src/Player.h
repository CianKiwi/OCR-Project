#pragma once
#include "SDL.h"
#include "Collision.h"

struct Keybinds{
	SDL_Scancode up;
	SDL_Scancode down;
	SDL_Scancode left;
	SDL_Scancode right;
};

class Player{
private:
	//NOTE: storing position twice is bad. remove this and just use hitbox position
	double X;
	double Y;
public:
	//const references allow access but dis-allow modification
	const double& x;
	const double& y;
	double speed;
	Collider hitbox;
	Keybinds binds;
	//NOTE: moveRestrictions may be useless depending on implementation of collision
	directionBool moveRestrictions;
	
	Player(double i_x, double i_y, double i_speed, Collider i_hitbox, Keybinds i_keybinds);
	void set_pos(double i_x, double i_y);
	void move(double i_x, double i_y);
};
