#include "Player.h"

Player::Player(Collider i_hitbox, Keybinds i_keybinds, double i_moveSpeed)
	: hitbox{ i_hitbox }, binds{ i_keybinds }, moveSpeed{ i_moveSpeed }{

}