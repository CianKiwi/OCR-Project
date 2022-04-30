#pragma once
#include <iostream>

class Weapon{
public:
	std::string name;
	int damage = 1;
	double fireRate = 0.1f;
	double timeSinceFired = 0;
	double bulletDistance = 400;
	double bulletSpeed = 15;
	
	Weapon(std::string name, int damage, double fireRate, double bulletDistance, double bulletSpeed);
	Weapon(std::string name);
};
