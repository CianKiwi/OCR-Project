#include "Player.h"

Player::Player(Collider i_hitbox, Keybinds i_keybinds, Weapon i_weapon, double i_moveSpeed)
	: hitbox{ i_hitbox }, binds{ i_keybinds }, weapon{i_weapon}, moveSpeed{ i_moveSpeed }{

}