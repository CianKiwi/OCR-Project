#include "Weapon.h"

Weapon::Weapon(std::string name, int damage, double fireRate, double bulletDistance, double bulletSpeed)
: name{name}, damage{damage}, fireRate{fireRate}, bulletDistance{bulletDistance}, bulletSpeed{bulletSpeed}{
	
}

Weapon::Weapon(std::string name) : name{name}{
}