#ifndef AlienManager_hpp
#define AlienManager_hpp

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
#include "AlienShip.h"
#include "BulletManager.h"
#include "AsteroidManager.h"
#include "PlayerShip.h"

class PlayerShip;
class BulletManager;
class AsteroidManager;
class AlienShip;
class AlienShipManager
{
public:
	std::vector<AlienShip *> alienshipList;
	std::vector<glm::vec3> _small_alienship_vertices;
	std::vector<glm::vec2> _small_alienship_uvs;
	std::vector<glm::vec3> _small_alienship_normals;

	std::vector<glm::vec3> _large_alienship_vertices;
	std::vector<glm::vec2> _large_alienship_uvs;
	std::vector<glm::vec3> _large_alienship_normals;

	bool generateAlienShip = false;
	int frameCount = 0;

	int _max_num_alienship = 5;

public:
	AlienShipManager(std::vector<glm::vec3> _small_alienship_vertices,
	std::vector<glm::vec2> _small_alienship_uvs,
	std::vector<glm::vec3> _small_alienship_normals,

	std::vector<glm::vec3> _large_alienship_vertices,
	std::vector<glm::vec2> _large_alienship_uvs,
	std::vector<glm::vec3> _large_alienship_normals);
	void updateAlienShips(BulletManager *bulletmanager, AsteroidManager *asteroidManager, PlayerShip *p);
	void displayAlienShips();
	void addAlienShip();
	void addLargeAlienShip();
};

#endif
