#ifndef AsteroidManager_hpp
#define AsteroidManager_hpp

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
#include "Asteroid.h"
#include "BulletManager.h"

class Asteroid;
class AsteroidManager
{
public:
	std::vector<Asteroid *> asteroidList;

	const static int _asteroid_types = 3;
	std::vector<glm::vec3> _small_asteroid_vertices;
	std::vector<glm::vec2> _small_asteroid_uvs;
	std::vector<glm::vec3> _small_asteroid_normals;

	std::vector<glm::vec3> _medium_asteroid_vertices;
	std::vector<glm::vec2> _medium_asteroid_uvs;
	std::vector<glm::vec3> _medium_asteroid_normals;

	std::vector<glm::vec3> _large_asteroid_vertices;
	std::vector<glm::vec2> _large_asteroid_uvs;
	std::vector<glm::vec3> _large_asteroid_normals;

	float abs_explosion_distance = 5.0;
	float explosion_distance = 0.0;
	bool explosion = false;

public:
	AsteroidManager(std::vector<glm::vec3> _small_vertices,
		std::vector<glm::vec2> _small_uvs,
		std::vector<glm::vec3> _small_normals,

		std::vector<glm::vec3> _medium_vertices,
		std::vector<glm::vec2> _medium_uvs,
		std::vector<glm::vec3> _medium_normals,

		std::vector<glm::vec3> _large_vertices,
		std::vector<glm::vec2> _large_uvs,
		std::vector<glm::vec3> _large_normals);
	void generateRandomAsteroids();
	void updateAsteroids(BulletManager *bulletmanager);
	void displayAsteroids();
	void explode(float XExplosion, float YExplosion);
};

#endif