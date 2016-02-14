#include "Asteroid.h"

Asteroid::Asteroid() {
}

float Asteroid::generateRandomSpeed() {
	float randomSpeedValue = (rand() % 6 - 3.0) / 500;
	
	while (randomSpeedValue == 0) {
		randomSpeedValue = (rand() % 6 - 3.0) / 500;
	}

	return randomSpeedValue;
}

float Asteroid::generateRandomRotateSpeed() {
	return (rand() % 3 + 1.5) / 50;
}

float* Asteroid::generaeteRandomAxis() {
	int random_flag = rand() % 3 + 1;
	
	float x[3] = { 1.0, 0.0, 0.0 };
	float y[3] = { 1.0, 0.0, 0.0 };
	float z[3] = { 1.0, 0.0, 0.0 };

	if (random_flag == 1) {
		return x;
	}
	else if (random_flag == 2) {
		return y;
	}
	else {
		return z;
	}
}

smallAsteroid::smallAsteroid()
{

}

smallAsteroid::smallAsteroid(AsteroidManager *asteroidManager)
{
	XSpeed = generateRandomSpeed();
	YSpeed = generateRandomSpeed();

	randomAxis = generaeteRandomAxis();
	rotateSpeed = generateRandomRotateSpeed();

	int random_spawn_flag = rand() % 4 + 1;

	if (random_spawn_flag == 1) {
		XLocation = -50.0;
		YLocation = rand() % (50 * 2 + 1) - 50.0;
	}
	else if (random_spawn_flag == 2) {
		XLocation = rand() % (50 * 2 + 1) - 50.0;
		YLocation = 50.0;
	}
	else if (random_spawn_flag == 3) {
		XLocation = 50.0;
		YLocation = rand() % (50 * 2 + 1) - 50.0;
	}
	else {
		XLocation = rand() % (50 * 2 + 1) - 50.0;
		YLocation = -50.0;
	}
}

smallAsteroid::smallAsteroid(AsteroidManager *asteroidManager, float XLocation, float YLocation)
{
	this->XLocation = XLocation;
	this->YLocation = YLocation;
	XSpeed = generateRandomSpeed();
	YSpeed = generateRandomSpeed();

	randomAxis = generaeteRandomAxis();
	rotateSpeed = generateRandomRotateSpeed();
}


void smallAsteroid::display(AsteroidManager *asteroidManager) {
	if (active) {
		glLoadIdentity();

		glPushMatrix();
		glTranslatef(XLocation, YLocation, 0.0f);

		glRotatef(rotateDegree, 1.0f, 0.0f, 0.0f);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);


		glNormalPointer(GL_FLOAT, 0, &(asteroidManager->_small_asteroid_normals[0]));
		glVertexPointer(3, GL_FLOAT, 0, &(asteroidManager->_small_asteroid_vertices[0]));

		glDrawArrays(GL_TRIANGLES, 0, asteroidManager->_small_asteroid_vertices.size());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glPopMatrix();
	}
	else {
		if (explosionStarted == true && explosion_distance < _explosion_abs_distance) {
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
			explosion_distance += 0.001;
		}
		else {
			explosionEnded = true;
			explosion_distance = 0.0;
		}
	}
}

void smallAsteroid::update(BulletManager *bulletManager)
{
	if (active) {
		if (XLocation < -50.0 || XLocation > 50.0) {
			XLocation *= (-1.0);
		}

		if (YLocation < -50.0 || YLocation > 50.0) {
			YLocation *= (-1.0);
		}

		XLocation += XSpeed;
		YLocation += YSpeed;

		rotateDegree += rotateSpeed;

		for (std::vector<Bullet *>::iterator it = bulletManager->bulletList.begin(); it != bulletManager->bulletList.end(); ++it) {
			float bullet_x = (*it)->XLocation;
			float bullet_y = (*it)->YLocation;
			float circle_x = XLocation;
			float circle_y = YLocation;
			float d = (circle_x - bullet_x) * (circle_x - bullet_x) + (circle_y - bullet_y) * (circle_y - bullet_y);
			if (d < (this->radius) * (this->radius)) {

				XExplosion = XLocation;
				YExplosion = YLocation;

				active = false;
				explosionStarted = true;

				(*it)->dieOut = true;
				bulletManager->score += 100; 
			}
		}
	}
	else {
		explosionStarted = true;

		XExplosion = XLocation;
		YExplosion = YLocation;
	}
}

float smallAsteroid::getRadius() {
	return 1.3f;
}

mediumAsteroid::mediumAsteroid()
{

}

mediumAsteroid::mediumAsteroid(AsteroidManager *asteroidManager)
{
	XSpeed = generateRandomSpeed();
	YSpeed = generateRandomSpeed();

	randomAxis = generaeteRandomAxis();
	rotateSpeed = generateRandomRotateSpeed();

	int random_spawn_flag = rand() % 4 + 1;

	if (random_spawn_flag == 1) {
		XLocation = -50.0;
		YLocation = rand() % (50 * 2 + 1) - 50.0;
	}
	else if (random_spawn_flag == 2) {
		XLocation = rand() % (50 * 2 + 1) - 50.0;
		YLocation = 50.0;
	}
	else if (random_spawn_flag == 3) {
		XLocation = 50.0;
		YLocation = rand() % (50 * 2 + 1) - 50.0;
	}
	else {
		XLocation = rand() % (50 * 2 + 1) - 50.0;
		YLocation = -50.0;
	}
}

mediumAsteroid::mediumAsteroid(AsteroidManager *asteroidManager, float XLocation, float YLocation)
{
	this->XLocation = XLocation;
	this->YLocation = YLocation;
	XSpeed = generateRandomSpeed();
	YSpeed = generateRandomSpeed();

	randomAxis = generaeteRandomAxis();
	rotateSpeed = generateRandomRotateSpeed();
}



void mediumAsteroid::display(AsteroidManager *asteroidManager) {
	if (splitUp == false) {
		glLoadIdentity();

		glPushMatrix();
		glTranslatef(XLocation, YLocation, 0.0f);
		glRotatef(rotateDegree, 1.0f, 0.0f, 0.0f);

		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);


		glNormalPointer(GL_FLOAT, 0, &(asteroidManager->_medium_asteroid_normals[0]));
		glVertexPointer(3, GL_FLOAT, 0, &(asteroidManager->_medium_asteroid_vertices[0]));

		glDrawArrays(GL_TRIANGLES, 0, asteroidManager->_medium_asteroid_vertices.size());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glPopMatrix();
	}
}

void mediumAsteroid::update(BulletManager *bulletManager)
{	
	if (splitUp == false) {
		if (XLocation < -50.0 || XLocation > 50.0) {
			XLocation *= (-1.0);
		}

		if (YLocation < -50.0 || YLocation > 50.0) {
			YLocation *= (-1.0);
		}

		XLocation += XSpeed;
		YLocation += YSpeed;
		rotateDegree += rotateSpeed;

		for (std::vector<Bullet *>::iterator it = bulletManager->bulletList.begin(); it != bulletManager->bulletList.end(); ++it) {
			float bullet_x = (*it)->XLocation;
			float bullet_y = (*it)->YLocation;
			float circle_x = XLocation;
			float circle_y = YLocation;
			float d = (circle_x - bullet_x) * (circle_x - bullet_x) + (circle_y - bullet_y) * (circle_y - bullet_y);
			if (d < (this->radius) * (this->radius)) {
				(*it)->dieOut = true;
				this->splitUp = true;
				bulletManager->score += 50;
			}
		}
	}
}

float mediumAsteroid::getRadius() {
	return  2.5f;
}


largeAsteroid::largeAsteroid()
{

}

largeAsteroid::largeAsteroid(AsteroidManager *asteroidManager)
{
	XSpeed = generateRandomSpeed();
	YSpeed = generateRandomSpeed();

	randomAxis = generaeteRandomAxis();
	rotateSpeed = generateRandomRotateSpeed();

	int random_spawn_flag = rand() % 4 + 1;

	if (random_spawn_flag == 1) {
		XLocation = -50.0;
		YLocation = rand() % (50 * 2 + 1) - 50.0;
	}
	else if (random_spawn_flag == 2) {
		XLocation = rand() % (50 * 2 + 1) - 50.0;
		YLocation = 50.0;
	}
	else if (random_spawn_flag == 3) {
		XLocation = 50.0;
		YLocation = rand() % (50 * 2 + 1) - 50.0;
	}
	else {
		XLocation = rand() % (50 * 2 + 1) - 50.0;
		YLocation = -50.0;
	}
}


void largeAsteroid::display(AsteroidManager *asteroidManager) {
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(XLocation, YLocation, 0.0f);
	glRotatef(rotateDegree, 1.0f, 0.0f, 0.0f);

	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);


	glNormalPointer(GL_FLOAT, 0, &(asteroidManager->_large_asteroid_normals[0]));
	glVertexPointer(3, GL_FLOAT, 0, &(asteroidManager->_large_asteroid_vertices[0]));

	glDrawArrays(GL_TRIANGLES, 0, asteroidManager->_large_asteroid_vertices.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPopMatrix();
}

void largeAsteroid::update(BulletManager *bulletManager)
{
	if (XLocation < -50.0 || XLocation > 50.0) {
		XLocation *= (-1.0);
	}

	if (YLocation < -50.0 || YLocation > 50.0) {
		YLocation *= (-1.0);
	}

	XLocation += XSpeed;
	YLocation += YSpeed;
	rotateDegree += rotateSpeed;

	for (std::vector<Bullet *>::iterator it = bulletManager->bulletList.begin(); it != bulletManager->bulletList.end(); ++it) {
		float bullet_x = (*it)->XLocation;
		float bullet_y = (*it)->YLocation;
		float circle_x = XLocation;
		float circle_y = YLocation;
		float d = (circle_x - bullet_x) * (circle_x - bullet_x) + (circle_y - bullet_y) * (circle_y - bullet_y);
		if (d < (this->radius) * (this->radius)) {
			(*it)->dieOut = true;
			this->splitUp = true;
			bulletManager->score += 20;
		}
	}
}

float largeAsteroid::getRadius() {
	return 3.7f;
}
