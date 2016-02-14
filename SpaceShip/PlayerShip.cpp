#include "PlayerShip.h"

bool PlayerShip::fire = false;
using namespace std;
PlayerShip::PlayerShip(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals)
{
	XLocation = 0.0;
	YLocation = 0.0;
	PlayerShipSpeed = 0.0;
	direction = 0;

	this->vertices = vertices;
	this->uvs = uvs;
	this->normals = normals;

	// initial static vector
	collisionVector.push_back(glm::vec2(0.02857, 4.25152));
	collisionVector.push_back(glm::vec2(-0.92209, 2.67154));
	collisionVector.push_back(glm::vec2(0.95049, 2.66193));
	collisionVector.push_back(glm::vec2(2.00000, 0.50342));
	collisionVector.push_back(glm::vec2(2.02403, -1.98827));
	collisionVector.push_back(glm::vec2(-2.01586, - 1.99987));
	collisionVector.push_back(glm::vec2(-2.00000, 0.50489));

	dynamic_collisionVector.resize(collisionVector.size());

	for (std::vector<glm::vec2>::size_type i = 0; i != collisionVector.size(); i++) {
		dynamic_collisionVector[i][0] = collisionVector[i][0] + XLocation;
		dynamic_collisionVector[i][1] = collisionVector[i][1] + YLocation;
	}
}

void PlayerShip::incrementDirection() {
	if (direction + 90 > 360) {
		direction = 0 - 90;
	}

	direction += 7;
}

void PlayerShip::decrementDirection() {
	if (direction + 90 < 0) {
		direction = 360 - 90;
	}

	direction -= 7;
}

void PlayerShip::incrementSpeed() {
	if (PlayerShipSpeed < 0.01) {
		PlayerShipSpeed += 0.005;
	}
}

void PlayerShip::displayPlayerShip() {
	if (active) {
		glLoadIdentity();

		glPushMatrix();
		glTranslatef(XLocation, YLocation, 0.0f);

		glPushMatrix();
		glRotatef(direction, 0.0f, 0.0f, 1.0f);

		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		glNormalPointer(GL_FLOAT, 0, &normals[0]);
		glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glPopMatrix();
		glPopMatrix();
	} else if (active == false && explosionStarted == true && explosion_distance < _explosion_abs_distance) {
		glColor3f(0.0f, 0.0f, 0.0f);
		glPointSize(3.0f);

		glLoadIdentity();

		glPushMatrix();
		glTranslatef(XExplosion + explosion_distance, YExplosion + explosion_distance, 0.0f);
		glBegin(GL_POINTS);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(XExplosion - explosion_distance, YExplosion + explosion_distance, 0.0f);
		glBegin(GL_POINTS);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(XExplosion + explosion_distance, YExplosion - explosion_distance, 0.0f);
		glBegin(GL_POINTS);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(XExplosion - explosion_distance, YExplosion - explosion_distance, 0.0f);
		glBegin(GL_POINTS);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glEnd();
		glPopMatrix();
		explosion_distance += 0.0005;
	}
	else if (active == false && explosionStarted == true && explosion_distance > _explosion_abs_distance) {
		explosionEnded = true;
		lives--;
	}
}

void PlayerShip::updatePlayerShip(AsteroidManager *asteroidManager, AlienShipManager *alienshipManager, BulletManager *bulletManager) {

	if (lives <= 0) {
		gameOver = true;
	} 

	if (gameOver == false) {
		if (explosionEnded == true && lives > 0) {
			reset();
		}

		if (explosionStarted == false) {
			XSpeed = cos(fabs(vdirection + 90) * 3.1415926 / 180.0) * PlayerShipSpeed;
			YSpeed = sin(fabs(vdirection + 90) * 3.1415926 / 180.0) * PlayerShipSpeed;

			XLocation += XSpeed;
			YLocation += YSpeed;

			if (XLocation < -50.0 || XLocation > 50.0) {
				XLocation *= (-1.0);
			}

			if (YLocation < -50.0 || YLocation > 50.0) {
				YLocation *= (-1.0);
			}
			// autoBreak
			if (PlayerShipSpeed < 0.095 && PlayerShipSpeed > 0) {
				PlayerShipSpeed -= 0.0000001;
			}
		}

		// update collision vector location
		for (std::vector<glm::vec2>::size_type i = 0; i != collisionVector.size(); i++) {
			dynamic_collisionVector[i][0] = collisionVector[i][0] + XLocation;
			dynamic_collisionVector[i][1] = collisionVector[i][1] + YLocation;
		}

		for (std::vector<glm::vec2>::size_type i = 0; i != dynamic_collisionVector.size(); i++) {
			float collision_x = dynamic_collisionVector[i][0];
			float collision_y = dynamic_collisionVector[i][1];

			for (std::vector<Asteroid *>::iterator it = asteroidManager->asteroidList.begin(); it != asteroidManager->asteroidList.end(); ++it) {
				if ((*it)->active) {
					float circle_x = (*it)->XLocation;
					float circle_y = (*it)->YLocation;

					float d = (circle_x - collision_x) * (circle_x - collision_x) + (circle_y - collision_y) * (circle_y - collision_y);
					if (d < ((*it)->getRadius()) * ((*it)->getRadius())) {

						XExplosion = (*it)->XLocation;
						YExplosion = (*it)->YLocation;


						if (dynamic_cast<mediumAsteroid*>(*it)) {
							(*it)->splitUp = true;
						}
						else if (dynamic_cast<largeAsteroid*>(*it)) {
							(*it)->splitUp = true;
						}
						else {
							(*it)->active = false;
						}

						this->explosionStarted = true;
						this->active = false;
					}
				}
			}

			for (std::vector<AlienShip *>::iterator it = alienshipManager->alienshipList.begin(); it != alienshipManager->alienshipList.end(); ++it) {
				if ((*it)->active) {
					float circle_x = (*it)->XLocation;
					float circle_y = (*it)->YLocation;


					float d = (circle_x - collision_x) * (circle_x - collision_x) + (circle_y - collision_y) * (circle_y - collision_y);
					if (d < ((*it)->getRadius()) * ((*it)->getRadius())) {

						XExplosion = (*it)->XLocation;
						YExplosion = (*it)->YLocation;

						this->explosionStarted = true;
						this->active = false;

						(*it)->explosionStarted = true;
						(*it)->active = false;
					}
				}
			}

			for (std::vector<Bullet *>::iterator it = bulletManager->bulletList.begin(); it != bulletManager->bulletList.end(); ++it) {
				if ((*it)->dieOut == false && (*it) -> isAlien() == true) {
					float circle_x = (*it)->XLocation;
					float circle_y = (*it)->YLocation;


					float d = (circle_x - collision_x) * (circle_x - collision_x) + (circle_y - collision_y) * (circle_y - collision_y);
					if (d < (1.0)) {

						XExplosion = (*it)->XLocation;
						YExplosion = (*it)->YLocation;

						this->explosionStarted = true;
						this->active = false;

						(*it)->dieOut = true;
					}
				}
			}
		}
	}
}

void PlayerShip::setDirection(float v) {
	vdirection = v;
}

void PlayerShip::respawn() {
	XLocation = 0;
	YLocation = 0;
	XSpeed = 0;
	YSpeed = 0;
	direction = 0;
	vdirection = 0;
	PlayerShipSpeed = 0;

	active = true;
	explosionStarted = false;
	explosionEnded = false;

	explosion_distance = 0.0;

	XExplosion = 0;
	YExplosion = 0;
}

void PlayerShip::reset() {
	XLocation = 0;
	YLocation = 0;
	XSpeed = 0;
	YSpeed = 0;
	direction = 0;
	vdirection = 0;
	PlayerShipSpeed = 0;

	active = true;
	explosionStarted = false;
	explosionEnded = false;
	bool gameOver = false;

	explosion_distance = 0.0;

	XExplosion = 0;
	YExplosion = 0;
	int lives = _default_lives;
}