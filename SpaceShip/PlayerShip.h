#ifndef PlayerShip_hpp
#define PlayerShip_hpp

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
#include "AsteroidManager.h"
#include <iostream> 
#include "glm/ext.hpp"
#include "AlienShip.h"
#include "AlienShipManager.h"

class AlienShipManager;
class AsteroidManager;
class BulletManager;
class PlayerShip
{
public:
	const static int _resetFirehmilliseconds = (1 / 15.0) * 100;
	const static int _default_lives = 2;

	float XLocation;
	float YLocation;
	float XSpeed = 0;
	float YSpeed = 0;
	float direction;
	float vdirection = 0;
	float PlayerShipSpeed = 0;
	static bool fire;

	bool active = true;
	bool explosionStarted = false;
	bool explosionEnded = false;
	bool gameOver = false;

	int lives = _default_lives;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> collisionVector;
	std::vector<glm::vec2> dynamic_collisionVector;


	float _explosion_abs_distance = 3.0;
	float explosion_distance = 0.0;

	float XExplosion = 0;
	float YExplosion = 0;

	float score = 0.0;
public:
	PlayerShip();
	PlayerShip(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals);
	void incrementDirection();
	void decrementDirection();
	void incrementSpeed();
	void displayPlayerShip();
	
	void setDirection(float v);

	void updatePlayerShip(AsteroidManager *asteroidManager, AlienShipManager *alienshipManager, BulletManager *bulletManager);
	void updateBullet();
	void reset();
	void respawn();
	~PlayerShip();
};

#endif