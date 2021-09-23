#include "Player.h"

Player::Player(double i_x, double i_y, double i_speed, Collider i_hitbox, Keybinds i_keybinds)
	: X{i_x}, Y{i_y}, x{X}, y{Y}, speed{i_speed}, hitbox{i_hitbox}, binds{i_keybinds}{
}

void Player::set_pos(double i_x, double i_y){
	X = i_x;
	Y = i_y;
	hitbox.set_pos(X, Y);
}

void Player::move(double i_x, double i_y){
	//if moveRestrictions are enabled, the player cannot move in the designated directions
	if ((i_x < 0 && !moveRestrictions.left) || (i_x > 0 && !moveRestrictions.right)){
		X += i_x;
	}
	if ((i_y < 0 && !moveRestrictions.up) || (i_y > 0 && !moveRestrictions.down)){
		Y += i_y;
	}
	hitbox.set_pos(X, Y);
}