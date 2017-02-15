#include "AlienShipManager.h"

AlienShipManager::AlienShipManager(std::vector<glm::vec3> _small_alienship_vertices,
	std::vector<glm::vec2> _small_alienship_uvs,
	std::vector<glm::vec3> _small_alienship_normals,

	std::vector<glm::vec3> _large_alienship_vertices,
	std::vector<glm::vec2> _large_alienship_uvs,
	std::vector<glm::vec3> _large_alienship_normals) {

	this->_small_alienship_normals = _small_alienship_normals;
	this->_small_alienship_uvs = _small_alienship_uvs;
	this->_small_alienship_vertices = _small_alienship_vertices;

	this->_large_alienship_normals = _large_alienship_normals;
	this->_large_alienship_uvs = _large_alienship_uvs;
	this->_large_alienship_vertices = _large_alienship_vertices;
}

void AlienShipManager::updateAlienShips(BulletManager *bulletmanager, AsteroidManager *asteroidManager, PlayerShip *p) {
	
	frameCount++;

	if (frameCount == 60 * 100 && alienshipList.size() < _max_num_alienship) {
		addLargeAlienShip();
	}

	if (frameCount == 60 * 100 && alienshipList.size() < _max_num_alienship && bulletmanager->score > 1000) {
		addAlienShip();
	}

	if (frameCount > 60 * 100) {
		frameCount = 0;
	}

	for (std::vector<AlienShip *>::iterator it = alienshipList.begin(); it != alienshipList.end(); ++it) {
		(*it)->update(asteroidManager, bulletmanager, p);

		if ((*it)->active == false && (*it)->explosionEnded == true) {
			it = alienshipList.erase(it);
		}
	}

	std::vector<AlienShip *>::iterator out_of_screen = alienshipList.begin();
	while (out_of_screen != alienshipList.end()) {

		if ((*out_of_screen)->XLocation < -50.0 
			|| (*out_of_screen)->XLocation > 50.0
			|| (*out_of_screen)->YLocation < -50.0
			|| (*out_of_screen)->YLocation > 50.0) {
			out_of_screen = alienshipList.erase(out_of_screen);
	} else {
		++out_of_screen;
	}
}

}

void AlienShipManager::addAlienShip() {
	if (alienshipList.size() < 5) {
		alienshipList.push_back(new AlienShip(this));
		printf("\n an alienship has been added");
	}
}

void AlienShipManager::addLargeAlienShip() {
	if (alienshipList.size() < 5) {
		alienshipList.push_back(new largeAlienShip(this));
		printf("\n a large alienship has been added");
	}
}


void AlienShipManager::displayAlienShips() {
	for (std::vector<AlienShip *>::iterator it = alienshipList.begin(); it != alienshipList.end(); ++it) {
		(*it)->display(this);
	}
}