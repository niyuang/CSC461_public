#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

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

	glPushMatrix();
	char  *txt = { "New Game" };
	draw_string(2, 2, 2, txt);
	glPopMatrix();


	glutSwapBuffers();
}