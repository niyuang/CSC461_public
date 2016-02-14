#ifndef BulletManager_hpp
#define BulletManager_hpp

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include <glm/glm.hpp>
#include <GL/glu.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include "PlayerShip.h"
#include "Bullet.h"

class Bullet;
class BulletManager
{
public:
	const static int _max_bullets = 4;
	PlayerShip *p;
	std::vector<Bullet *> bulletList;
	float score = 0.0;

public:
	BulletManager();
	BulletManager(PlayerShip *p);
	void fireBullet();
	void fireAlienBullet(AlienShip *alienship);
	void fireAlienBulletAt(AlienShip *alienship, float XLocation, float YLocation);
	void updateBullets();
	void displayBullets();
};

#endif