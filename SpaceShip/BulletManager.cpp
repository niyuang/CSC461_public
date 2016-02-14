#include "BulletManager.h"

BulletManager::BulletManager() {
}

BulletManager::BulletManager(PlayerShip *p)
{
	 this->p = p;
}

void BulletManager::fireBullet()
{	
	int playerBulletCount = 0;

	for (std::vector<Bullet *>::iterator it = bulletList.begin(); it != bulletList.end(); ++it) {
		if ((*it)->isAlien() == false) {
			playerBulletCount++;
		}
	}

	if (playerBulletCount <= _max_bullets) {
		Bullet *b = new Bullet(this->p);
		bulletList.push_back(b);
	}
}

void BulletManager::fireAlienBullet(AlienShip *alienship) {
	AlienBullet *alienBullet = new AlienBullet(alienship);
	bulletList.push_back(alienBullet);
}

void BulletManager::fireAlienBulletAt(AlienShip *alienship, float XLocation, float YLocation) {
	AlienBullet *alienBullet = new AlienBullet(alienship, XLocation, YLocation);
	bulletList.push_back(alienBullet);
}


void BulletManager::updateBullets()
{	
	for (std::vector<Bullet *>::iterator it = bulletList.begin(); it != bulletList.end(); ++it) {
			(*it)->update();
	}

	std::vector<Bullet *>::iterator it = bulletList.begin();
	while (it != bulletList.end()) {
		if ((*it)->dieOut == true) {
			it = bulletList.erase(it);
		} else {
			++it;
		}
	} 
}

void BulletManager::displayBullets()
{
	for (std::vector<Bullet *>::iterator it = bulletList.begin(); it != bulletList.end(); ++it) {
		(*it)->display();
	}
}

