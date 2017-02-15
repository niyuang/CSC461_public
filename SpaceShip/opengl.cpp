#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif


#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string.h>
#include <math.h>

#include <vector>
#include <glm/glm.hpp>
#include <stdio.h>
#include "PlayerShip.h"
#include "BulletManager.h"
#include "AsteroidManager.h"
#include "AlienShipManager.h"

#define GLEW_STATIC

//  Module global variables
int shaderProgram;

int _refreshmilliseconds = (1 / 15.0) * 1000;
int toggleRotate = 1;
int angle = 2;
int _default_asteroid_number = 5;

std::vector<glm::vec3> _spaceship_vertices;
std::vector<glm::vec2> _spaceship_uvs;
std::vector<glm::vec3> _spaceship_normals;

std::vector<glm::vec3> _small_alienship_vertices;
std::vector<glm::vec2> _small_alienship_uvs;
std::vector<glm::vec3> _small_alienship_normals;

std::vector<glm::vec3> _large_alienship_vertices;
std::vector<glm::vec2> _large_alienship_uvs;
std::vector<glm::vec3> _large_alienship_normals;

std::vector<glm::vec3> _small_asteroid_vertices;
std::vector<glm::vec2> _small_asteroid_uvs;
std::vector<glm::vec3> _small_asteroid_normals;

std::vector<glm::vec3> _medium_asteroid_vertices;
std::vector<glm::vec2> _medium_asteroid_uvs;
std::vector<glm::vec3> _medium_asteroid_normals;

std::vector<glm::vec3> _large_asteroid_vertices;
std::vector<glm::vec2> _large_asteroid_uvs;
std::vector<glm::vec3> _large_asteroid_normals;

PlayerShip *playerShip;
BulletManager *bulletManager;
AsteroidManager *asteroidManager;
AlienShipManager *alienShipManager;

char scoreToChar[10];
char livesToChar[10];

enum _game_screens_enum
{
	_game_screen_main_menu = 0,
	_game_screen_game,
	_game_screen_game_over,
	_game_screen_exit,
};

enum _main_menu_enum
{
	_new_game = 0,
	_exit_game,
};

int screen = _game_screen_main_menu;
int menu = _new_game;

void display_menu(void);
void display_obj(void);
void update_obj(void);

void input_menu(unsigned char key, int x, int y);
void specialInput_game(int key, int x, int y);

boolean load_obj(
	const char * path,
	std::vector <glm::vec3> & out_vertices,
	std::vector <glm::vec2> & out_uvs,
	std::vector <glm::vec3> & out_normals) {

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Filed to open the file! %c \n", path);
		return false;
	}

	while (1) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {
			break;
		}

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			if (matches != 9) {
				printf("File can't be read by this parser. matches equals %f", &matches);
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}


	// Indexing 
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
	}
}

char *textFileRead(char *fn) {
	FILE *fp;
	char *content = NULL;

	int count = 0;

	if (fn != NULL) {
		fp = fopen(fn, "rt");

		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

void draw_string(float x, float y, float z, char *txt)
{
	glRasterPos3f(x, y, z);
	while (*txt != '\0') {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *txt);
		txt++;
	}
}

void setupShaders(void) {
	char *vs, *fs;

	int vertexShader, fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	vs = textFileRead("Data/vertexShader.txt");

	const char *vv = vs;
	glShaderSource(vertexShader, 1, &vv, NULL);
	free(vs);
	glCompileShader(vertexShader);

	// Fragment Shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	fs = textFileRead("Data/vertexShader.txt");

	const char *fv = fs;
	glShaderSource(fragmentShader, 1, &fv, NULL);
	free(fs);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint linked;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
	if (linked) {
		printf("Linked!");
	} 
}

void init(void)
{
	float  amb[] = { 0.2, 0.1, 0.1, 0.1 };	// Ambient material property
	float  lt_amb[] = { 0.5, 0.5, 0.3, 0.1 };	// Ambient light property
	float  lt_dif[] = { 2, 1, 1, 2 };	// Ambient light property
	float  lt_pos[] = {			// Light position
		-100, 100, 100, 0
	};
	float  lt_spc[] = { 0, 0, 0, 1 };	// Specular light property

	//  Set default ambient light in scene
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

	//  Set Light 0 position, ambient, diffuse, specular intensities
	glLightfv(GL_LIGHT0, GL_POSITION, lt_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lt_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lt_dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lt_spc);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);

	// load spaceship Object
	bool res = load_obj("Data/spaceShip.obj", _spaceship_vertices, _spaceship_uvs, _spaceship_normals);
	playerShip = new PlayerShip(_spaceship_vertices, _spaceship_uvs, _spaceship_normals);
	bulletManager = new BulletManager(playerShip);

	// load small asteroid
	bool aster_res = load_obj("Data/small_asteroid.obj", _small_asteroid_vertices, _small_asteroid_uvs, _small_asteroid_normals);

	// load medium asteroid
	bool med_aster_res = load_obj("Data/medium_asteroid.obj", _medium_asteroid_vertices, _medium_asteroid_uvs, _medium_asteroid_normals);

	// load large asteroid
	bool large_aster_res = load_obj("Data/large_asteroid.obj", _large_asteroid_vertices, _large_asteroid_uvs, _large_asteroid_normals);

	// load alienship
	bool small_alien_ship_res = load_obj("Data/small_alienShip.obj", _small_alienship_vertices, _small_alienship_uvs, _small_alienship_normals);

	// load large alienship
	bool large_alien_ship_res = load_obj("Data/large_alienShip.obj", _large_alienship_vertices, _large_alienship_uvs, _large_alienship_normals);

	asteroidManager = new AsteroidManager(_small_asteroid_vertices, _small_asteroid_uvs, _small_asteroid_normals, _medium_asteroid_vertices, _medium_asteroid_uvs, _medium_asteroid_normals, _large_asteroid_vertices, _large_asteroid_uvs, _large_asteroid_normals);
	alienShipManager = new AlienShipManager(_small_alienship_vertices, _small_alienship_uvs, _small_alienship_normals, _large_alienship_vertices, _large_alienship_uvs, _large_alienship_normals);
}

void timer(int value) {
	glutPostRedisplay();

	if (toggleRotate) {
		angle += 2;
		angle %= 360;
	}

	glutTimerFunc(_refreshmilliseconds, timer, 0);
}

void display_menu(void)

//  This routine displays a Goroud-shaded RGB cube
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0, 50.0, -50.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, -1, 0, 1, 0);

	glLoadIdentity();


	// draw Menu selector
	if (menu == _new_game) {
		glPushMatrix();
		glTranslatef(-2.0f, 6.0f, 1.0f); 

		glRotatef(angle, 0.0f, 1.0f, 0.0f);

		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		glNormalPointer(GL_FLOAT, 0, &_spaceship_normals[0]);
		glVertexPointer(3, GL_FLOAT, 0, &_spaceship_vertices[0]);

		glDrawArrays(GL_TRIANGLES, 0, _spaceship_vertices.size());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslatef(-2.0f, 3.0f, 1.0f); 

		glRotatef(angle, 0.0f, 1.0f, 0.0f);

		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		glNormalPointer(GL_FLOAT, 0, &_spaceship_normals[0]);
		glVertexPointer(3, GL_FLOAT, 0, &_spaceship_vertices[0]);

		glDrawArrays(GL_TRIANGLES, 0, _spaceship_vertices.size());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glPopMatrix();
	}

	char  *new_game_str = { "New Game" };
	draw_string(2, 5, 2, new_game_str);

	char  *exit_game_str = { "Exit Game" };
	draw_string(2, 2, 2, exit_game_str);

	glPopMatrix();
	glutSwapBuffers();
}

void specialInput_menu(int key, int x, int y) {
	switch (key)
	{
		case GLUT_KEY_UP:
		menu = _new_game;
		printf("up");
		break;
		case GLUT_KEY_DOWN:
		menu = _exit_game;
		printf("down");
		break;
	}
}


void display_obj(void)

//  This routine displays a Goroud-shaded RGB cube
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0, 50.0, -50.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, -1, 0, 1, 0);

	playerShip->displayPlayerShip();
	bulletManager->displayBullets();
	asteroidManager->displayAsteroids();
	alienShipManager->displayAlienShips();

	char  *scoreboard_str = { "Score Board: " };
	draw_string(-47, -47, 2, scoreboard_str);

	sprintf(scoreToChar,"%.2f", bulletManager->score);
	draw_string(-30, -47, 2, scoreToChar);


	char  *spaceship_remaining_str = { "Space ship remaining: " };
	draw_string(-47, -43, 2, spaceship_remaining_str);

	sprintf(livesToChar, "%d", playerShip -> lives);
	draw_string(-15, -43, 2, livesToChar);

	char *notes_str_0 = { "Generate a large alienShip every 5 seconds."};
	draw_string(-40, 45, 2, notes_str_0);

	char *notes_str_1 = { "When score are over 1,000, spawn a small alienship every 5 seconds. " };
	draw_string(-40, 40, 2, notes_str_1);

	char *notes_str_2 = { "Maxium number of alienship in screen: 5 " };
	draw_string(-40, 35, 2, notes_str_2);

	char *notes_str_3 = { "To debug: Press 'l' to spawn a large alienship" };
	draw_string(-40, 30, 2, notes_str_3);

	char *notes_str_4 = { "To debug: Press 's' to spawn a small alienship" };
	draw_string(-40, 25, 2, notes_str_4);
}					// End routine display_obj

void specialInput_game(int key, int x, int y) {
	switch (key)
	{
		case GLUT_KEY_LEFT:
		playerShip->incrementDirection();
		break;
		case GLUT_KEY_RIGHT:
		playerShip->decrementDirection();
		break;
	}
}

void input_game(unsigned char key, int x, int y) {
	switch (key)
	{
		case 'x':
		playerShip->incrementSpeed();
		playerShip->setDirection(playerShip->direction);
		break;
		case 'z':
		bulletManager->fireBullet();
		break;
		case 'l' :
		alienShipManager->addLargeAlienShip();
		break;
		case 's':
		alienShipManager->addAlienShip();
		break;
	}
}

void update_obj(void) {
	playerShip->updatePlayerShip(asteroidManager, alienShipManager, bulletManager);
	bulletManager->updateBullets();
	asteroidManager->updateAsteroids(bulletManager);
	alienShipManager->updateAlienShips(bulletManager, asteroidManager, playerShip);

	if (asteroidManager->asteroidList.size() < _default_asteroid_number) {
		asteroidManager->generateRandomAsteroids();
	}

	if (playerShip->gameOver == true) {
		glutDisplayFunc(display_menu);
		glutSpecialFunc(specialInput_menu);
		glutKeyboardFunc(input_menu);
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void input_menu(unsigned char key, int x, int y) {
	if (key == 13) {
		if (menu == _exit_game) {
			exit(0);
		}
		else if (menu == _new_game) {

			if (playerShip->gameOver) {
				playerShip = new PlayerShip(_spaceship_vertices, _spaceship_uvs, _spaceship_normals);
				bulletManager = new BulletManager(playerShip);
				asteroidManager = new AsteroidManager(_small_asteroid_vertices, _small_asteroid_uvs, _small_asteroid_normals, _medium_asteroid_vertices, _medium_asteroid_uvs, _medium_asteroid_normals, _large_asteroid_vertices, _large_asteroid_uvs, _large_asteroid_normals);
				alienShipManager = new AlienShipManager(_small_alienship_vertices, _small_alienship_uvs, _small_alienship_normals, _large_alienship_vertices, _large_alienship_uvs, _large_alienship_normals);
			}
			glutDisplayFunc(display_obj);
			glutSpecialFunc(specialInput_game);
			glutKeyboardFunc(input_game);
			glutIdleFunc(update_obj);
		}
	}
}


void main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	srand(time(NULL));
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	
	// needs to happen after createWindow
	/*
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	setupShaders();  */
	init();
	glutDisplayFunc(display_menu);
	glutSpecialFunc(specialInput_menu);
	glutKeyboardFunc(input_menu);

	glutTimerFunc(0, timer, 0);

	glutMainLoop();			// Enter GLUT main loop
}					// End mainline

