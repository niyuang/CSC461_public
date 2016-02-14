#ifndef Asteroid_hpp
#define Asteroid_hpp

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include "AsteroidManager.h"
#include <glm/glm.hpp>
#include <GL/glu.h>
#include <string.h>
#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <time.h>
#include "BulletManager.h"

class AsteroidManager;
class BulletManager;
class Asteroid
{
public:
	// need to set up a inital value
	float XLocation = 0;
	float YLocation = 0;
	float direction;
	float XSpeed = 0;
	float YSpeed = 0;

	bool active = true;
	bool splitUp = false;

	bool explosionStarted = false;
	bool explosionEnded = false;

	float _explosion_abs_distance = 3.0;
	float explosion_distance = 0.0;

	float XExplosion = 0;
	float YExplosion = 0;

	float* randomAxis;
	float  rotateSpeed;
	float rotateDegree = 0;
public:
	Asteroid();
	Asteroid(AsteroidManager *asteroidManager);
	float generateRandomSpeed();
	// easy way to make it abstract. set it to 0.
	virtual void update(BulletManager *bulletManager) = 0;
	// easy way to make it abstract. set it to 0.
	virtual void display(AsteroidManager *asteroidManager) = 0;
	virtual float getRadius() = 0;
	float* generaeteRandomAxis();
	float generateRandomRotateSpeed();
};

class smallAsteroid : public Asteroid {
	private:
		typedef Asteroid asteroid;
		float radius = 1.3f;

	public:
		smallAsteroid();
		smallAsteroid(AsteroidManager *asteroidManager);
		smallAsteroid(AsteroidManager *asteroidManager, float XLocation, float YLocation);
		void display(AsteroidManager *asteroidManager);
		void update(BulletManager *bulletManager);
		float getRadius();
};

class mediumAsteroid : public Asteroid {
private:
	typedef Asteroid asteroid;
	float radius = 2.5f;

public:
	mediumAsteroid();
	mediumAsteroid(AsteroidManager *asteroidManager);
	mediumAsteroid(AsteroidManager *asteroidManager, float XLocation, float YLocation);
	void display(AsteroidManager *asteroidManager);
	void update(BulletManager *bulletManager);
	float getRadius();
};

class largeAsteroid : public Asteroid {
private:
	typedef Asteroid asteroid;
	float radius = 3.5f;

public:
	largeAsteroid();
	largeAsteroid(AsteroidManager *asteroidManager);
	void display(AsteroidManager *asteroidManager);
	void update(BulletManager *bulletManager);
	float getRadius();
};

#endif