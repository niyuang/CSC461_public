#ifndef Bullet_hpp
#define Bullet_hpp

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include <glm/glm.hpp>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include "BulletManager.h"
#include "AlienShip.h"

class PlayerShip;
class AlienShip;

class Bullet
{
public:
	float XLocation = 0;
	float YLocation = 0;
	float direction = 0;
	float speed = 0;
	bool dieOut = false;
public:
	Bullet();
	Bullet(PlayerShip *p);
	virtual void update();
	void display();
    virtual bool isAlien() { return false; };
	float generateRandomSpeed();
};

class AlienBullet : public Bullet {
public:
	float XSpeed;
	float YSpeed;

public:
	AlienBullet(AlienShip *alienship);
	AlienBullet(AlienShip *alienship, float XDestination, float YDestination);
	void update();
	void display();
	bool isAlien() { return true; };
};

#endif