#ifndef AlienShip_hpp
#define AlienShip_hpp

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include <glm/glm.hpp>
#include <GL/glu.h>
#include <string.h>
#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <time.h>
#include "AlienShipManager.h"
#include "PlayerShip.h"

class PlayerShip;
class BulletManager;
class AsteroidManager;
class AlienShipManager;
class AlienShip {
	public :
		float XLocation = 0.0f;
		float YLocation = 0.0f;
		float XSpeed = 0.0f;
		float YSpeed = 0.0f;
		float rotateDegree = 0.0f;
		float rotateSpeed = 0.0f;
		bool active = true;
		float XExplosion;
		float YExplosion;
		float explosion_distance = 0.0;
		float _explosion_abs_distance = 3.0;
		bool explosionStarted = false;
		bool explosionEnded = false;

	public:
		AlienShip();
		AlienShip(AlienShipManager *alienshipManager);
		virtual void display(AlienShipManager *alienshipManager);
		void update(AsteroidManager *asteroidManager, BulletManager *bulletManager, PlayerShip *p);
		virtual float getRadius();
		void shotAt(float XLocation, float YLocation);
};

class largeAlienShip : public AlienShip {
	public:
		largeAlienShip();
		largeAlienShip(AlienShipManager *alienshipManager);
		void display(AlienShipManager *alienshipManager);
		void update(AsteroidManager *asteroidManager, BulletManager *bulletManager, PlayerShip *p);
		float getRadius();
};

#endif