#include "Bullet.h"

Bullet::Bullet() {

}

Bullet::Bullet(PlayerShip *p)
{
	XLocation = p->XLocation;
	YLocation = p->YLocation;
	direction = p->direction;

	speed = p->PlayerShipSpeed;

	dieOut = false;
}

float Bullet::generateRandomSpeed() {
	float randomSpeedValue = (rand() % 6 - 3.0) / 500;

	while (randomSpeedValue == 0) {
		randomSpeedValue = (rand() % 6 - 3.0) / 500;
	}

	return randomSpeedValue;
}

AlienBullet::AlienBullet(AlienShip *alienship) {
	XLocation = alienship->XLocation;
	YLocation = alienship->YLocation;

	float randomXSpeed = generateRandomSpeed();
	float randomYSpeed = generateRandomSpeed();

	dieOut = false;
}

AlienBullet::AlienBullet(AlienShip *alienship, float XDestination, float YDestination) {

	XLocation = alienship->XLocation;
	YLocation = alienship->YLocation;

	XSpeed =  (alienship->YSpeed + 0.02);
	YSpeed =  (alienship->XSpeed + 0.02);

	dieOut = false;
}


void Bullet::update() {
	if (XLocation < -50.0 || XLocation > 50.0 ||
		YLocation < -50.0 || YLocation > 50.0) {
		dieOut = true;
	}

	XLocation += cos(fabs(direction + 90) * 3.1415926 / 180.0) * (speed + 0.0025);
	YLocation += sin(fabs(direction + 90) * 3.1415926 / 180.0) * (speed + 0.0025);
}

void AlienBullet::update() {
	if (XLocation < -50.0 || XLocation > 50.0 ||
		YLocation < -50.0 || YLocation > 50.0) {
		dieOut = true;
	}

	XLocation += XSpeed;
	YLocation += YSpeed;
}

void Bullet::display() {
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(3.0f);

	glLoadIdentity();

	glPushMatrix();
	glTranslatef(XLocation, YLocation, 0.0f);
	glBegin(GL_POINTS);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	glPopMatrix();
}


