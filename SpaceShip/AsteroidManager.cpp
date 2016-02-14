#include "AsteroidManager.h"

bool generateTwoSmallAsteroid = false;
bool generateTwoMediumAsteroid = false;

float XSplit;
float YSplit;

AsteroidManager::AsteroidManager(
std::vector<glm::vec3> _small_asteroid_vertices,
std::vector<glm::vec2> _small_asteroid_uvs,
std::vector<glm::vec3> _small_asteroid_normals,

std::vector<glm::vec3> _medium_asteroid_vertices,
std::vector<glm::vec2> _medium_asteroid_uvs,
std::vector<glm::vec3> _medium_asteroid_normals,

std::vector<glm::vec3> _large_asteroid_vertices,
std::vector<glm::vec2> _large_asteroid_uvs,
std::vector<glm::vec3> _large_asteroid_normals) {

	this->_small_asteroid_normals = _small_asteroid_normals;
	this->_small_asteroid_uvs = _small_asteroid_uvs;
	this->_small_asteroid_vertices = _small_asteroid_vertices;

	this->_medium_asteroid_normals = _medium_asteroid_normals;
	this->_medium_asteroid_uvs = _medium_asteroid_uvs;
	this->_medium_asteroid_vertices = _medium_asteroid_vertices;

	this->_large_asteroid_normals = _large_asteroid_normals;
	this->_large_asteroid_uvs = _large_asteroid_uvs;
	this->_large_asteroid_vertices = _large_asteroid_vertices;
}

void AsteroidManager::generateRandomAsteroids() {
	int flag = rand() % _asteroid_types + 1;

	if (flag == 1) {
		asteroidList.push_back(new largeAsteroid(this));
		printf("A small asteroid has been generated");
	} else if (flag == 2) {
		asteroidList.push_back(new largeAsteroid(this));
		printf("A medium asteroid has been generated");
	} else if (flag == 3) {
		asteroidList.push_back(new largeAsteroid(this));
		printf("A large asteroid has been generated");
	} else {
		printf("%d", flag);
	}
}

void AsteroidManager::updateAsteroids(BulletManager *bulletManager)
{	
	for (std::vector<Asteroid *>::iterator it = asteroidList.begin(); it != asteroidList.end(); ++it) {
		(*it)->update(bulletManager);
	}

	std::vector<Asteroid *>::iterator it = asteroidList.begin();
	while (it != asteroidList.end()) {
			if ((*it)->active == false && (*it)->explosionEnded == true) {
				it = asteroidList.erase(it);
			}
			else {
				++it;
			}
	}

	std::vector<Asteroid *>::iterator split_it = asteroidList.begin();
	while (split_it != asteroidList.end()) {

		if ((*split_it)->splitUp == true) {
			XSplit = (*split_it)->XLocation;
			YSplit = (*split_it)->YLocation;
			explosion = true;

			if (dynamic_cast<mediumAsteroid*>(*split_it)) {
					split_it = asteroidList.erase(split_it);
					generateTwoSmallAsteroid = true;
			}
			else if (dynamic_cast<largeAsteroid*>(*split_it)) {
				split_it = asteroidList.erase(split_it);
				generateTwoMediumAsteroid = true;
			}
		 
		}
		else {
			++split_it;
		}
	}

	if (generateTwoSmallAsteroid == true) {
		asteroidList.push_back(new smallAsteroid(this, XSplit, YSplit));
		asteroidList.push_back(new smallAsteroid(this, XSplit, YSplit));
		generateTwoSmallAsteroid = false;
	}

	if (generateTwoMediumAsteroid == true) {
		asteroidList.push_back(new mediumAsteroid(this, XSplit, YSplit));
		asteroidList.push_back(new mediumAsteroid(this, XSplit, YSplit));
		generateTwoMediumAsteroid = false;
	}
}

void AsteroidManager::displayAsteroids()
{
	for (std::vector<Asteroid *>::iterator it = asteroidList.begin(); it != asteroidList.end(); ++it) {
		(*it)->display(this);
	}

	if (explosion == true) {
		if (explosion_distance < abs_explosion_distance) {
			glColor3f(0.0f, 0.0f, 0.0f);
			glPointSize(3.0f);

			glLoadIdentity();

			glPushMatrix();
			glTranslatef(XSplit + explosion_distance, YSplit + explosion_distance, 0.0f);
			glBegin(GL_POINTS);
			glVertex3f(0.0f, 1.0f, 0.0f);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(XSplit - explosion_distance, YSplit + explosion_distance, 0.0f);
			glBegin(GL_POINTS);
			glVertex3f(0.0f, 1.0f, 0.0f);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(XSplit + explosion_distance, YSplit - explosion_distance, 0.0f);
			glBegin(GL_POINTS);
			glVertex3f(0.0f, 1.0f, 0.0f);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(XSplit - explosion_distance, YSplit - explosion_distance, 0.0f);
			glBegin(GL_POINTS);
			glVertex3f(0.0f, 1.0f, 0.0f);
			glEnd();
			glPopMatrix();
			explosion_distance += 0.001;
		}
		else {
			explosion = false;
			explosion_distance = 0.0;
		}
	}  
}
