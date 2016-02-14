#include "AlienShip.h"

AlienShip::AlienShip()
{
}

AlienShip::AlienShip(AlienShipManager *alienshipManager)
{	
	int random_spawn_flag = rand() % 2 + 1;

	if (random_spawn_flag == 1) {
		XLocation = -50.0;
		YLocation = rand() % (50 * 2 + 1) - 50.0;
		XSpeed = (rand() % 6 + 3.0) / 250;
		YSpeed = (rand() % 6 + 3.0) / 2500;
	}
	else if (random_spawn_flag == 2) {
		XLocation = 50.0;
		YLocation = rand() % (50 * 2 + 1) - 50.0;
		XSpeed = (-1) * (rand() % 6 + 3.0) / 250;
		YSpeed = (rand() % 6 + 3.0) / 2500;
	}

	rotateSpeed = (rand() % 3 + 1.5) / 50;
}

void AlienShip::display(AlienShipManager *alienshipManager){
	if (active) {
		glLoadIdentity();

		glPushMatrix();
		glTranslatef(XLocation, YLocation, 0.0f);
		glRotatef(rotateDegree, 0.0f, 0.0f, 1.0f);

		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);


		glNormalPointer(GL_FLOAT, 0, &(alienshipManager->_small_alienship_normals[0]));
		glVertexPointer(3, GL_FLOAT, 0, &(alienshipManager->_small_alienship_vertices[0]));

		glDrawArrays(GL_TRIANGLES, 0, alienshipManager->_small_alienship_vertices.size());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glPopMatrix();
	} else {
		if (explosion_distance < _explosion_abs_distance && explosionStarted == true) {
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
		else {
			explosionEnded = true;
			explosion_distance = 0.0;
		}
	}
}

void AlienShip::update(AsteroidManager *asteroidManager, BulletManager *bulletManager, PlayerShip *p) {
	if (active) {
		XLocation += XSpeed;
		YLocation += YSpeed;

		rotateDegree += rotateSpeed;

		int flag_change_y_speed = rand() % 300 + 1;
		if (flag_change_y_speed == 1) {
			YSpeed *= (-1.0);
		}

		
		int flag_fire = rand() % 3000 + 1;
		if (flag_fire == 1) {

			int flag_target = rand() % 3 + 1;
			if (flag_target == 1) {
				// fire randomly
				bulletManager->fireAlienBullet(this);
			}
			else if (flag_target == 2) {
				// fire at an asteroid
				int flag_target_asteroid = rand() % (asteroidManager->asteroidList.size() - 1);
				bulletManager->fireAlienBulletAt(this, asteroidManager->asteroidList.at(flag_target_asteroid)->XLocation, asteroidManager->asteroidList.at(flag_target_asteroid)->YLocation);
			}
			else if (flag_target == 3) {
				bulletManager->fireAlienBulletAt(this, p->XLocation, p->YLocation);

			}
		} 

		// Collision between asteroid
		for (std::vector<Asteroid *>::iterator it = asteroidManager->asteroidList.begin(); it != asteroidManager->asteroidList.end(); ++it) {

			float d = sqrt(pow((*it)->XLocation - this->XLocation, 2) + pow((*it)->YLocation - this->YLocation, 2));
			if (d < abs((*it)->getRadius() + this->getRadius())) {
				active = false;
				explosionStarted = true;
				XExplosion = XLocation;
				YExplosion = YLocation;
				
				if (dynamic_cast<mediumAsteroid*>(*it) || dynamic_cast<largeAsteroid*>(*it)) {
					(*it)->splitUp = true;
				} else {
					(*it)->active = false;
				}
			}
		}

		// Collision between bullets
		for (std::vector<Bullet *>::iterator it = bulletManager->bulletList.begin(); it != bulletManager->bulletList.end(); ++it) {
			float bullet_x = (*it)->XLocation;
			float bullet_y = (*it)->YLocation;
			float circle_x = XLocation;
			float circle_y = YLocation;
			float d = (circle_x - bullet_x) * (circle_x - bullet_x) + (circle_y - bullet_y) * (circle_y - bullet_y);
			if (d < (this->getRadius()) * (this->getRadius()) && (*it)->isAlien() == false) {

				XExplosion = XLocation;
				YExplosion = YLocation;

				active = false;
				explosionStarted = true;

				(*it)->dieOut = true;
				bulletManager->score += 1000;
			}
		}
	}
}

float AlienShip::getRadius() {
	return 1.5;
}

largeAlienShip::largeAlienShip()
{
}

largeAlienShip::largeAlienShip(AlienShipManager *alienshipManager)
{
	int random_spawn_flag = rand() % 2 + 1;

	if (random_spawn_flag == 1) {
		XLocation = -50.0;
		YLocation = rand() % (50 * 2 + 1) - 50.0;
		XSpeed = (rand() % 6 + 3.0) / 500;
		YSpeed = (rand() % 6 + 3.0) / 5000;
	}
	else if (random_spawn_flag == 2) {
		XLocation = 50.0;
		YLocation = rand() % (50 * 2 + 1) - 50.0;
		XSpeed = (-1) * (rand() % 6 + 3.0) / 500;
		YSpeed = (rand() % 6 + 3.0) / 5000;
	}

	rotateSpeed = (rand() % 3 + 1.5) / 50;
}

void largeAlienShip::display(AlienShipManager *alienshipManager) {
	if (active) {
		glLoadIdentity();

		glPushMatrix();
		glTranslatef(XLocation, YLocation, 0.0f);
		glRotatef(rotateDegree, 0.0f, 0.0f, 1.0f);

		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);


		glNormalPointer(GL_FLOAT, 0, &(alienshipManager->_large_alienship_normals[0]));
		glVertexPointer(3, GL_FLOAT, 0, &(alienshipManager->_large_alienship_vertices[0]));

		glDrawArrays(GL_TRIANGLES, 0, alienshipManager->_large_alienship_vertices.size());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glPopMatrix();
	}
	else {
		if (explosion_distance < _explosion_abs_distance && explosionStarted == true) {
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
		else {
			explosionEnded = true;
			explosion_distance = 0.0;
		}
	}
}

float largeAlienShip::getRadius() {
	return 2.5;
}

void largeAlienShip::update(AsteroidManager *asteroidManager, BulletManager *bulletManager, PlayerShip *p) {
	if (active) {
		XLocation += XSpeed;
		YLocation += YSpeed;

		rotateDegree += rotateSpeed;

		int flag_change_y_speed = rand() % 30000 + 1;
		if (flag_change_y_speed == 1 || YLocation > 49 || YLocation < -49) {
			YSpeed *= (-1.0);
		}

		int flag_fire = rand() % 30 + 1;
		if (flag_fire == 1) {
			int flag_target = rand() % 3 + 1;
			if (flag_target == 1) {
				// fire randomly
				bulletManager->fireAlienBullet(this);
			}
			else if (flag_target == 2) {
				// fire at an asteroid
				int flag_target_asteroid = rand() % (asteroidManager->asteroidList.size() - 1) + 1;
				bulletManager->fireAlienBulletAt(this, asteroidManager->asteroidList.at(flag_target_asteroid)->XLocation, asteroidManager->asteroidList.at(flag_target_asteroid)->YLocation);
				printf("\n fire at a random asteroid \n");
			}

		}
		
		for (std::vector<Asteroid *>::iterator it = asteroidManager->asteroidList.begin(); it != asteroidManager->asteroidList.end(); ++it) {
			float d = sqrt(pow((*it)->XLocation - this->XLocation, 2) + pow((*it)->YLocation - this->YLocation, 2));
			if (d < abs((*it)->getRadius() + this->getRadius())) {
				active = false;
				explosionStarted = true;
				XExplosion = XLocation;
				YExplosion = YLocation;

				if (dynamic_cast<mediumAsteroid*>(*it) || dynamic_cast<largeAsteroid*>(*it)) {
					(*it)->splitUp = true;
				}
				else {
					(*it)->active = false;
				}
			}
		}

		for (std::vector<Bullet *>::iterator it = bulletManager->bulletList.begin(); it != bulletManager->bulletList.end(); ++it) {
			float bullet_x = (*it)->XLocation;
			float bullet_y = (*it)->YLocation;
			float circle_x = XLocation;
			float circle_y = YLocation;
			float d = (circle_x - bullet_x) * (circle_x - bullet_x) + (circle_y - bullet_y) * (circle_y - bullet_y);
			if (d < (this->getRadius()) * (this->getRadius()) && (*it)->isAlien() == false) {

				XExplosion = XLocation;
				YExplosion = YLocation;

				active = false;
				explosionStarted = true;

				(*it)->dieOut = true;
				bulletManager->score += 200;
			}
		}
	}
}